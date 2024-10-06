

#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameUtility.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#define JUMP_HEIGHT (m_jumpTime * m_jumpTime * (-m_gravity) / 2 + (m_jumpTime * m_jumpPower))
#define SET_JUMP_TIME 0.5

//
//担当：海北
//
AMyCharacter::AMyCharacter()
	:m_pSpringArm(NULL)
	, m_pCamera(NULL)
	//, Camera_Shoot(NULL)
	, m_charaMoveInput(FVector2D::ZeroVector)
	, m_cameraRotateInput(FVector2D::ZeroVector)
	, m_cameraPitchLimit(FVector2D(-40.0f, 55.0f))
	, m_bCanControl(true)
	, m_bOverlappedCPU(false)
	, m_Overlaped_CPU(NULL)
{
	//デフォルトプレイヤーとして設定
	AutoPossessPlayer = EAutoReceiveInput::Player0;


	//スプリングアームのオブジェクトを生成
	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));
	if (m_pSpringArm != NULL)
	{
		m_pSpringArm->SetupAttachment(RootComponent);
		//アームの長さを設定
		//カメラのコリジョンテストを行うか設定
		m_pSpringArm->bDoCollisionTest = false;
		////カメラ追従ラグを使うかを設定
		//m_pSpringArm->bEnableCameraLag = true;
		////カメラ追従ラグの速度を設定
		//m_pSpringArm->CameraLagSpeed = 10.f;
		////カメラ回転ラグを使うかを設定
		//m_pSpringArm->bEnableCameraRotationLag = true;
		////カメラ回転ラグの速度を設定
		//m_pSpringArm->CameraRotationLagSpeed = 10.f;
	}

	//カメラのオブジェクと生成
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_pCamera"));
	if ((m_pCamera != NULL) && (m_pSpringArm != NULL))
	{
		//カメラをスプリングアームにアタッチさせる
		m_pCamera->SetupAttachment(m_pSpringArm, USpringArmComponent::SocketName);
	}


	BaseTurnRate = 75.f;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	////移動処理
	UpdateMove(DeltaTime);

	//カメラ更新処理
	UpdateCamera(DeltaTime);
	//ジャンプ処理
	UpdateJump(DeltaTime);

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

	//シュート
	InputComponent->BindAction("shoot", IE_Pressed, this, &AMyCharacter::Shoot);
	//タックル
	InputComponent->BindAction("Tackle", IE_Pressed, this, &AMyCharacter::TackleStart);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);

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
	//移動入力がある場合
	if (!m_charaMoveInput.IsZero() && (DeltaTime != 0.0f))
	{
		//入力に合わせて、Actorを左右前後に移動
		USpringArmComponent* pSpringArm = m_pSpringArm;
		if (pSpringArm != NULL)
		{
			FVector NewLocation = GetActorLocation();

			//キャラクターの移動
			{
				//SpringArmが動く方向に、入力による移動量をPawnMoveComponentに渡す
				FVector forwardVec = pSpringArm->GetForwardVector();
				AddMovementInput(forwardVec, m_charaMoveInput.Y * m_moveSpeed);
				FVector rightVec = pSpringArm->GetRightVector();
				AddMovementInput(rightVec, m_charaMoveInput.X * m_moveSpeed);

			}

			//移動するキャラクターの回転
			{
				//角度はDegreeの角度の範囲で表記(-180 ~ 180)
				USkeletalMeshComponent* pMeshComp = GetMesh();
				if (pMeshComp != NULL)
				{
					//アークタンジェントを使ってコントローラーの入力方向がなす角度を求める
					float angle = atan2(m_charaMoveInput.X, m_charaMoveInput.Y);
					//Radian値をDegreeに変換
					float angleDeg = FMath::RadiansToDegrees(angle);

					//入力した角度 + メッシュの回転角度 + Actorに対して回転しているSpringArmの相対角度
					float newYaw = angleDeg + GetBaseRotationOffsetRotator().Yaw + pSpringArm->GetRelativeRotation().Yaw;

					pMeshComp->SetRelativeRotation(FRotator(pMeshComp->GetRelativeRotation().Pitch, newYaw, pMeshComp->GetRelativeRotation().Roll));

				}
			}
		}
	}

}

//
//担当：海北
//
void AMyCharacter::UpdateJump(float _deltaTime)
{
	//ジャンプ中フラグを確認してから
	if (m_bJumping)
	{
		//アニメーションのために遅らせる
		//
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
		//if (GetCharacterMovement()->IsFalling() == false)
		//{
		//	m_bJumping = false;
		//	m_jumpTime = 0.0f;

		//}
		else
		{
			//現在の座標にジャンプ量を足す
			SetActorLocation(FVector(nowPos.X, nowPos.Y, m_posBeforeJump.Z + m_nowJumpHeight), true);

			//ジャンプ量を保持
			m_prevJumpHeight = m_nowJumpHeight;

		}

		////現在の座標にジャンプ量を足す
		//SetActorLocation(FVector(nowPos.X, nowPos.Y, m_posBeforeJump.Z + m_nowJumpHeight), true);

		////ジャンプ量を保持
		//m_prevJumpHeight = m_nowJumpHeight;


	}
}

//
//担当：海北
//
void AMyCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
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
void AMyCharacter::Chara_MoveForward(float _axisValue)
{
	//コントール可能な場合のみ
	if (m_bCanControl == false) { return; }

	if ((Controller != NULL) && (_axisValue != 0.0f))
	{
		//コントローラーの入力方向を取得する
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//前方向の向きを取得する
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		//繰り出す
		AddMovementInput(Direction, _axisValue);
	}
	//m_charaMoveInput.Y = FMath::Clamp(_axisValue, -1.0f, 10000.0f) * 10000.0f;

}


//
//担当：海北
//
//入力バインド キャラ移動：左右
void AMyCharacter::Chara_MoveRight(float _axisValue)
{
	//コントール可能な場合のみ
	if (m_bCanControl == false) { return; }

	if ((Controller != NULL) && (_axisValue != 0.0f))
	{
		//コントローラーの入力方向を取得する
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//左右方向の向きを取得する
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//繰り出す
		AddMovementInput(Direction, _axisValue);

	}
	//m_charaMoveInput.X = FMath::Clamp(_axisValue, -1.0f, 10000.0f) * 10000.0f;


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

//入力バインド　シュート
void AMyCharacter::Shoot()
{

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