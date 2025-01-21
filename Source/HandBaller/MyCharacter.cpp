

#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "HAL/PlatformProcess.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"								// 計算処理のライブラリ



#define JUMP_HEIGHT (m_jumpTime * m_jumpTime * (-m_gravity) / 2 + (m_jumpTime * m_jumpPower))
#define SET_JUMP_TIME 0.5

//
//担当：海北
//
AMyCharacter::AMyCharacter()
	:m_pSpringArm(NULL)
	, m_pCamera(NULL)
	//, Camera_Shoot(NULL)
	, m_moveVec(FVector::ZeroVector)
	, m_charaMoveInput(FVector2D::ZeroVector)
	, m_cameraRotateInput(FVector2D::ZeroVector)
	, m_cameraPitchLimit(FVector2D(-40.0f, 55.0f))
	, m_bGetBeforeLocationZ(false)
	, m_bGetNowLocationZ(false)
	, m_bShouldDownhill(false)
	, m_NowDownhillTime(0)
	, m_WaitGetLoacationTime(0)
	, m_BeforeLocationZ(0.f)
	, m_NowLocationZ(0.f)
	, m_bOverlappedCPU(false)
	, m_Overlaped_CPU(NULL)
	//定数
	, GRAVITY(-100.f)
	, FALLSPEED_MAX(-3000.f)
	, MOVESPEED_MAX(1500.f)
	, THROW_POWER(2000.f)
	, MOVE_INVALIDRANGE(0.001f)
	, MOVE_SPEED_MAX(200.f)
	, MOVE_SPEED_MIDDLE(150.f)
	, MOVE_SPEED_MIN(100.f)
	, MOVE_SPEED_FALLING(50.f)
	, MOVE_SPEED_DOWNHILL(250.f)
	, MOVE_ROTATE_MAX(10.f)
	, MOVE_ROTATE_MIDDLE(5.f)
	, MOVE_ROTATE_MIN(1.f)
	, MOVE_BALL_HOLDING_RASIO(0.5f)
	, GET_LOCATION_COOLTIME(50)
	, DOWNHILL_POSSIBLE_TIME(50)
{
	//デフォルトプレイヤーとして設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;


	//スプリングアームのオブジェクトを生成
	if (m_pSpringArm == NULL)
	{
		m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));

		m_pSpringArm->SetupAttachment(GetCapsuleComponent());

		//アームの長さを設定
		m_pSpringArm->TargetArmLength = 400.f;
		// 位置と回転
		m_pSpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 100.f), FQuat(FRotator(0.f, 0.f, 0.f)));	// 位置と回転

	}

	//カメラのオブジェクと生成
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_pCamera"));
	if ((m_pCamera != NULL) && (m_pSpringArm != NULL))
	{
		//カメラをスプリングアームにアタッチさせる
		m_pCamera->SetupAttachment(m_pSpringArm, USpringArmComponent::SocketName);
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;

	//m_pSphereCompにオーバーラップした時の処理を登録
	m_pSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnSphereOverlapBegin);

	BaseTurnRate = 75.f;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bShouldDownhill)
	{
		UE_LOG(LogTemp, Log, TEXT("playDownhill"));
	}

	////移動処理
	UpdateMove(DeltaTime);
	//カメラ更新処理
	UpdateCamera(DeltaTime);
	//ジャンプ処理
	UpdateJump(DeltaTime);


	//前回と前回のZ座標を設定する
	SetBeforeNowLocationZ();

	//前回と現在のZ座標を比較する
	CompareLocationZ();

	//タックル処理
	if (m_bCanTackle != false) {
		UpdateTackle();
	}

	//フラグを下げる
	if (m_Overlaped_CPU) {
		m_Overlaped_CPU = false;
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//カメラの回転
	InputComponent->BindAxis("CameraPitch", this, &AMyCharacter::Camera_RotatePitch);
	InputComponent->BindAxis("CameraYaw", this, &AMyCharacter::Camera_RotateYaw);

	//移動
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::Chara_MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::Chara_MoveRight);

	//ジャンプ
	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::JumpStart);

	//投げる
	InputComponent->BindAction("Throw", IE_Pressed, this, &AMyCharacter::Throw);
	//タックル
	InputComponent->BindAction("Tackle", IE_Pressed, this, &AMyCharacter::TackleStart);


}

//
//担当：海北
//
void AMyCharacter::UpdateCamera(float DeltaTime)
{
	//ルートオブジェクトを中心に、スプリングアームについているカメラを回転させる
	USpringArmComponent* pSpringArm = m_pSpringArm;
	if (pSpringArm != NULL)
	{
		//現在のFPSを測定
		float fps = 1.0f / DeltaTime;

		//スローモードにする時は遅くする
		//処理落ちしても、一定速度でカメラが回るように設定
		float rotateCorrection = 60.f / fps;

		//カメラの新しい角度を求める
		//現在の角度を取得
		FRotator NewRotation = pSpringArm->GetRelativeRotation();

		//Yawは入力した分回す
		NewRotation.Yaw += m_cameraRotateInput.X * rotateCorrection;

		//Pitchに関しては、上下の制限角度の範囲内で切る
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch +
			(m_cameraRotateInput.Y * rotateCorrection), m_cameraPitchLimit.X, m_cameraPitchLimit.Y);

		//新しい角度を反映
		pSpringArm->SetRelativeRotation(NewRotation);
	}
}

//
//担当：海北
//
void AMyCharacter::UpdateMove(float DeltaTime)
{
	// 現在のFPSから補正値の設定
	float fps = 1.0f / DeltaTime;
	float CorrectionValue = 60.f / fps;

	// 移動量
	FVector NewVelocity = GetCharacterMovement()->Velocity;
	float RotateVelocity = MOVE_ROTATE_MIDDLE;
	float movement = 0.f;

	// 移動方向の確定
	FVector RightVec = m_pCamera->GetRightVector();
	FVector ForwardVec = m_pCamera->GetForwardVector();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	ForwardVec.Z = 0.f;
	ForwardVec.Normalize();

	if (!m_bCanControl)
	{
		return;
	}

	// 上下左右移動の反映
	if (m_bIsHolding)
	{
		movement = m_moveVec.Size() / MOVE_BALL_HOLDING_RASIO; 
	}
	else
	{
		movement = m_moveVec.Size();
	}

	// ジャンプ中はスティックの移動量を抑える
	if (GetCharacterMovement()->IsFalling() || m_bJumping && movement >= 0.9f)
	{
		RotateVelocity = MOVE_ROTATE_MIDDLE;
		NewVelocity += RightVec * m_moveVec.X * MOVE_SPEED_FALLING * CorrectionValue;
		NewVelocity += ForwardVec * m_moveVec.Y * MOVE_SPEED_FALLING * CorrectionValue;
		float CurSpeed = NewVelocity.Size2D();

		if (CurSpeed > MOVESPEED_MAX)
		{
			NewVelocity.X /= CurSpeed;
			NewVelocity.X *= MOVESPEED_MAX;
			NewVelocity.Y /= CurSpeed;
			NewVelocity.Y *= MOVESPEED_MAX;
		}
	}
	//坂を下っている時 かつ ボールを投げていない場合は加速する 
	if (m_bShouldDownhill && !m_bThrowAnim)
	{

		//アニメーションを再生
		AnimInstance->Montage_Play(DownhillMontage);
		GetCharacterMovement()->GravityScale = 100.f;
		NewVelocity += RightVec * m_moveVec.X * MOVE_SPEED_DOWNHILL * CorrectionValue;
		NewVelocity += ForwardVec * m_moveVec.Y * MOVE_SPEED_DOWNHILL * CorrectionValue;
		RotateVelocity = MOVE_ROTATE_MAX;

		ManageDownhill();

	}
	// 接地時
	else if (movement >= 0.9f)
	{
		NewVelocity += RightVec * m_moveVec.X * MOVE_SPEED_MAX * CorrectionValue;
		NewVelocity += ForwardVec * m_moveVec.Y * MOVE_SPEED_MAX * CorrectionValue;
		RotateVelocity = MOVE_ROTATE_MAX;

	}
	else if (movement >= 0.5f)
	{
		NewVelocity += RightVec * m_moveVec.X * MOVE_SPEED_MIDDLE * CorrectionValue;
		NewVelocity += ForwardVec * m_moveVec.Y * MOVE_SPEED_MIDDLE * CorrectionValue;
		RotateVelocity = MOVE_ROTATE_MIDDLE;

	}
	else
	{
		NewVelocity += RightVec * m_moveVec.X * MOVE_SPEED_MIN * CorrectionValue;
		NewVelocity += ForwardVec * m_moveVec.Y * MOVE_SPEED_MIN * CorrectionValue;
		RotateVelocity = MOVE_ROTATE_MIN;
	}	




	// メッシュの回転
	USkeletalMeshComponent* pMeshComp = GetMesh();
	FRotator rot = pMeshComp->GetRelativeRotation();


	// 移動している場合
	if (movement > 0.5f)
	{
		//アークタンジェントを使ってコントローラーの入力方向がなす角度を求める
		float moveAngleDeg = atan2(m_moveVec.X, m_moveVec.Y);
		//Radian値をDegreeに変換
		float angleDeg = FMath::RadiansToDegrees(moveAngleDeg);

		//入力した角度 + メッシュの回転角度 + Actorに対して回転しているSpringArmの相対角度
		float newYaw = angleDeg + GetBaseRotationOffsetRotator().Yaw + m_pSpringArm->GetRelativeRotation().Yaw + 180.0;

		float difference = newYaw - rot.Yaw;

		// 補正
		if (difference < 0.f)
		{
			difference += 360.f;
		}

		// 最大回転速度を上回るスピードで回転しようとする場合、制限する
		if (RotateVelocity < difference && difference <= 180.f)
		{
			newYaw = rot.Yaw + RotateVelocity * CorrectionValue;
		}
		else if (180.f < difference && difference < 360.f - RotateVelocity)
		{
			newYaw = rot.Yaw - RotateVelocity * CorrectionValue;
		}

		// 反映
		pMeshComp->SetRelativeRotation(FRotator(rot.Pitch, newYaw, rot.Roll));

		// 値の反映
		GetCharacterMovement()->Velocity = NewVelocity;

	}
}

//
//担当：海北
//
void AMyCharacter::UpdateJump(float _deltaTime)
{
	if (!m_bCanControl)
	{
		return;
	}

	//ジャンプ中フラグを確認してから
	if (m_bJumping)
	{
		//ジャンプ量を計算
		m_nowJumpHeight = JUMP_HEIGHT;

		//ジャンプ時間を増加
		m_jumpTime += _deltaTime;

		//Actorの現在の座標を取得
		FVector nowPos = GetActorLocation();

		////着地時（=ジャンプ量がマイナスに転じた時）ジャンプ前状態に戻す
		if (m_jumpTime > SET_JUMP_TIME && GetCharacterMovement()->IsFalling() == false)
		{
			m_bJumping = false;
			m_jumpTime = 0.0f;

		}
		else
		{
			//現在の座標にジャンプ量を足す
			SetActorLocation(FVector(nowPos.X, nowPos.Y, m_posBeforeJump.Z + m_nowJumpHeight), true);

			//ジャンプ量を保持
			m_prevJumpHeight = m_nowJumpHeight;
		}
	}
}

//
//担当：海北
//
//ボールを投げるInputイベントが呼ばれた時の処理
void AMyCharacter::Throw()
{
	//ボールを持っていないなら以降の処理を行わない
	if (!m_bIsHolding){
		return;
	}
	else if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance()){
		//投げるアニメーションを再生
		m_bThrowAnim = true;

		AnimInstance->Montage_Play(ThrowBallMontage);
	}
}


//
//担当：海北
//
//ボールを投げる処理
void AMyCharacter::ThrowBall()
{
	FVector fThrowVector = UKismetMathLibrary::GetForwardVector(m_pSpringArm->GetComponentRotation());
	
	FVector fVector = FVector(fThrowVector.X, fThrowVector.Y, 0.35f);

	fVector *= THROW_POWER;


	m_pBall->m_pStaticMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	m_pBall->ProjectileMovementComponent->SetActive(true);

	m_pBall->m_pStaticMeshComp->SetSimulatePhysics(true);
	m_pBall->m_pSphereComp->SetSimulatePhysics(false);

	m_pBall->ProjectileMovementComponent->SetUpdatedComponent(m_pBall->GetRootComponent());
	m_pBall->m_pStaticMeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);

	m_pBall->m_pStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	m_pBall->m_pStaticMeshComp->AddImpulse(fVector);
	
	m_bIsHolding = false;
}


//担当：海北
//
//ボールを投げた後のフラグ変更処理
void AMyCharacter::ChangeBallFlag()
{
	m_bCanTackle = true;
	m_bThrowAnim = false;
	m_bCanHold = true;


	m_pBall->m_pSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}


//
//担当：海北
//
//スフィアコンポーネントにオーバーラップ接触し始めた時に呼ばれるイベント関数を登録
void AMyCharacter::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	//ボール保持時とタックル中は、以下の処理を行わない
	if (m_bIsHolding && m_bTackle){
		return;
	}

	//オーバーラップした際に実行したいイベント
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//オーバーラップしたのがCPUのときのみ反応させたい
		if (OtherActor->ActorHasTag("Ball")) {
			FAttachmentTransformRules attachmentTransformRules = FAttachmentTransformRules::KeepRelativeTransform;

			attachmentTransformRules.LocationRule = EAttachmentRule::SnapToTarget;
			attachmentTransformRules.RotationRule = EAttachmentRule::SnapToTarget;
			attachmentTransformRules.ScaleRule = EAttachmentRule::KeepWorld;
			attachmentTransformRules.bWeldSimulatedBodies = true;

			//Ballを取得
			m_pBall = Cast<ABall>(OtherActor);
			m_pBall->AttachToComponent(GetMesh(), attachmentTransformRules, "BallCatch");

			//ボールの挙動、コリジョンを変更
			m_pBall->ProjectileMovementComponent->SetActive(false);
			m_pBall->m_pStaticMeshComp->SetSimulatePhysics(false);
			m_pBall->m_pSphereComp->SetSimulatePhysics(false);
			m_pBall->m_pSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			m_pBall->m_pStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);


			//フラグを設定
			m_bIsHolding = true;
			m_bCanHold = false;
			m_bCanTackle = false;
		}
	}
}

//担当：海北
//
//前回と前回のZ座標を設定する
void AMyCharacter::SetBeforeNowLocationZ()
{

	if (!m_bJumping)
	{
		//ジャンプ中ではない
		if (m_WaitGetLoacationTime == 1)
		{
			//Z座標を取得する
			if (m_bGetBeforeLocationZ)
			{
				//現在の座標を取得する
				m_NowLocationZ = GetActorLocation().Z;
				m_bGetNowLocationZ = true;
			}
			else
			{
				//前回座標を取得する
				m_BeforeLocationZ = GetActorLocation().Z;
				m_bGetBeforeLocationZ = true;
			}
		}
		else
		{
			if (m_WaitGetLoacationTime > GET_LOCATION_COOLTIME)
			{
				//タイマーリセット
				m_WaitGetLoacationTime = 0;
			}
		}
		m_WaitGetLoacationTime++;
	}

}

//担当：海北
//
//前回と現在のZ座標を比較する
void AMyCharacter::CompareLocationZ()
{
	//前回と現在のZ座標を取得しているか
	if (m_bGetBeforeLocationZ && m_bGetNowLocationZ)
	{
		//前回のZ座標より現在のZ座標の方が低いか
		if (m_BeforeLocationZ > m_NowLocationZ)
		{
			m_bShouldDownhill = true;
		}
		else
		{
			//値をリセット
			m_NowLocationZ = 0.f;
			m_bGetNowLocationZ = false;
			m_BeforeLocationZ = 0.f;
			m_bGetBeforeLocationZ = false;
		}
	}
}


//担当：海北
//
//前回と現在のZ座標を比較する
void AMyCharacter::ManageDownhill()
{

	while (m_NowDownhillTime < DOWNHILL_POSSIBLE_TIME)
	{
		m_NowDownhillTime++;
	}

	m_bShouldDownhill = false;
	//値をリセット
	m_NowLocationZ = 0.f;
	m_bGetNowLocationZ = false;
	m_BeforeLocationZ = 0.f;
	m_bGetBeforeLocationZ = false;
	m_NowDownhillTime = 0;
}


//
//担当：海北
//
//入力バインド カメラ回転：Pitch(Y軸)
void AMyCharacter::Camera_RotatePitch(float _axisValue)
{
	m_cameraRotateInput.Y = -_axisValue * 3.0f;
}

//
//担当：海北
//
//入力バインド カメラ回転：Yaw(Z軸)
void AMyCharacter::Camera_RotateYaw(float _axisValue)
{
	m_cameraRotateInput.X = _axisValue * 3.0f;
}

//
//担当：海北
//
//入力バインド キャラ移動：前後
void AMyCharacter::Chara_MoveForward(const float _axisValue)
{
	//コントール可能な場合のみ
	if (m_bCanControl == false)
	{
		m_moveVec.Y = 0.0f;
		return; 
	}

	if ((Controller != NULL) && (_axisValue != 0.0f))
	{
		// デッドゾーンは無効
		bool isDeadZone = -MOVE_INVALIDRANGE <= _axisValue && _axisValue <= MOVE_INVALIDRANGE;
		m_moveVec.Y = isDeadZone ? 0.f : _axisValue;

		//ボール保持時は速度低下
		if (m_bIsHolding)
		{
			m_moveVec.Y = m_moveVec.Y * MOVE_BALL_HOLDING_RASIO;
		}
	}
}


//
//担当：海北
//
//入力バインド キャラ移動：左右
void AMyCharacter::Chara_MoveRight(float _axisValue)
{
	//コントール可能な場合のみ
	if (m_bCanControl == false)
	{
		m_moveVec.X = 0.0f;
		return;
	}

	if ((Controller != NULL) && (_axisValue != 0.0f))
	{
		// デッドゾーンは無効
		bool isDeadZone = -MOVE_INVALIDRANGE <= _axisValue && _axisValue <= MOVE_INVALIDRANGE;
		m_moveVec.X = isDeadZone ? 0.f : _axisValue;

		//ボール保持時は速度低下
		if (m_bIsHolding)
		{
			m_moveVec.X = m_moveVec.X * MOVE_BALL_HOLDING_RASIO;
		}
	}
}

//
//担当：海北
//
//入力バインド　ジャンプ
void AMyCharacter::JumpStart()
{
	//コントール可能な場合のみ
	if (m_bCanControl == false) { return; }

	//ジャンプ中ではない場合
	if (m_bJumping == false)
	{
		//ジャンプ中状態にする
		m_bJumping = true;

		//ジャンプ前のActor座標を保持
		m_posBeforeJump = GetActorLocation();
	}
}


//
//担当：坂本
//
//入力バインド タックル
void AMyCharacter::TackleStart() {
	////コントール可能な場合のみ
	//if (m_bCanTackle != false) { return; }

	//if (m_bTackle == false) {
	//	m_bTackle = true;
	//	m_bCanTackle = false;
	//}
}



//
//担当：坂本
//
//オーバーラップ接触し始めた時に呼ばれるイベント関数を登録
void AMyCharacter::ActorOnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//オーバーラップした際に実行したいイベント
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//オーバーラップしたのがCPUのときのみ反応させたい
		if (OtherActor->ActorHasTag("CPU")) {
			//接触フラグを立てる
			m_bOverlappedCPU = true;
			//CPUを取得
			m_Overlaped_CPU = OtherActor;
		}
	}
}