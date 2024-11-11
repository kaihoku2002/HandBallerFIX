

#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "TimerManager.h"
#include "HAL/PlatformProcess.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"								// �v�Z�����̃��C�u����



#define JUMP_HEIGHT (m_jumpTime * m_jumpTime * (-m_gravity) / 2 + (m_jumpTime * m_jumpPower))
#define SET_JUMP_TIME 0.5

//
//�S���F�C�k
//
AMyCharacter::AMyCharacter()
	:m_pSpringArm(NULL)
	, m_pCamera(NULL)
	//, Camera_Shoot(NULL)
	, m_moveVec(FVector::ZeroVector)
	, m_charaMoveInput(FVector2D::ZeroVector)
	, m_cameraRotateInput(FVector2D::ZeroVector)
	, m_cameraPitchLimit(FVector2D(-40.0f, 55.0f))
	, m_bCanControl(true)
	, m_bOverlappedCPU(false)
	, m_Overlaped_CPU(NULL)
	//�萔
	, GRAVITY(-100.f)
	, FALLSPEED_MAX(-3000.f)
	, MOVESPEED_MAX(1500.f)
	, THROW_POWER(2000.f)
	, MOVE_INVALIDRANGE(0.001f)
	, MOVE_SPEED_MAX(200.f)
	, MOVE_SPEED_MIDDLE(150.f)
	, MOVE_SPEED_MIN(100.f)
	, MOVE_SPEED_FALLING(120.f)
	, MOVE_ROTATE_MAX(10.f)
	, MOVE_ROTATE_MIDDLE(5.f)
	, MOVE_ROTATE_MIN(1.f)

{
	//�f�t�H���g�v���C���[�Ƃ��Đݒ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//// �J�v�Z���R���|�[�l���g
	//if (GetCapsuleComponent() != nullptr)
	//{
	//	GetCapsuleComponent()->SetCapsuleHalfHeight(150.f);
	//	GetCapsuleComponent()->SetCapsuleRadius(42.f);
	//}

	//�X�v�����O�A�[���̃I�u�W�F�N�g�𐶐�
	if (m_pSpringArm == NULL)
	{
		m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));

		m_pSpringArm->SetupAttachment(GetCapsuleComponent());

		//�A�[���̒�����ݒ�
		m_pSpringArm->TargetArmLength = 400.f;
		// �ʒu�Ɖ�]
		m_pSpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 100.f), FQuat(FRotator(0.f, 0.f, 0.f)));	// �ʒu�Ɖ�]

		////�J�����̃R���W�����e�X�g���s�����ݒ�
		//m_pSpringArm->bDoCollisionTest = false;
		////�J�����Ǐ]���O���g������ݒ�
		//m_pSpringArm->bEnableCameraLag = false;
		////�J�����Ǐ]���O�̑��x��ݒ�
		//m_pSpringArm->CameraLagSpeed = 10.f;
		////�J������]���O���g������ݒ�
		//m_pSpringArm->bEnableCameraRotationLag = false;
		////�J������]���O�̑��x��ݒ�
		//m_pSpringArm->CameraRotationLagSpeed = 10.f;
	}

	//�J�����̃I�u�W�F�N�Ɛ���
	m_pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("m_pCamera"));
	if ((m_pCamera != NULL) && (m_pSpringArm != NULL))
	{
		//�J�������X�v�����O�A�[���ɃA�^�b�`������
		m_pCamera->SetupAttachment(m_pSpringArm, USpringArmComponent::SocketName);
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;

	//m_pSphereComp�ɃI�[�o�[���b�v�������̏�����o�^
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

	////�ړ�����
	UpdateMove(DeltaTime);
	//�J�����X�V����
	UpdateCamera(DeltaTime);
	//�W�����v����
	UpdateJump(DeltaTime);

	//�^�b�N������
	if (m_bCanTackle != false) {
		UpdateTackle();
	}

	//�t���O��������
	if (m_Overlaped_CPU) {
		m_Overlaped_CPU = false;
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	//�J�����̉�]
	InputComponent->BindAxis("CameraPitch", this, &AMyCharacter::Camera_RotatePitch);
	InputComponent->BindAxis("CameraYaw", this, &AMyCharacter::Camera_RotateYaw);

	//�ړ�
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::Chara_MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::Chara_MoveRight);

	//�W�����v
	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::JumpStart);

	//������
	InputComponent->BindAction("Throw", IE_Pressed, this, &AMyCharacter::Throw);
	//�^�b�N��
	InputComponent->BindAction("Tackle", IE_Pressed, this, &AMyCharacter::TackleStart);


}

//
//�S���F�C�k
//
void AMyCharacter::UpdateCamera(float DeltaTime)
{
	//���[�g�I�u�W�F�N�g�𒆐S�ɁA�X�v�����O�A�[���ɂ��Ă���J��������]������
	USpringArmComponent* pSpringArm = m_pSpringArm;
	if (pSpringArm != NULL)
	{
		//���݂�FPS�𑪒�
		float fps = 1.0f / DeltaTime;

		//�X���[���[�h�ɂ��鎞�͒x������
		//�����������Ă��A��葬�x�ŃJ���������悤�ɐݒ�
		float rotateCorrection = 60.f / fps;

		//�J�����̐V�����p�x�����߂�
		//���݂̊p�x���擾
		FRotator NewRotation = pSpringArm->GetRelativeRotation();

		//Yaw�͓��͂�������
		NewRotation.Yaw += m_cameraRotateInput.X * rotateCorrection;

		//Pitch�Ɋւ��ẮA�㉺�̐����p�x�͈͓̔��Ő؂�
		NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch +
			(m_cameraRotateInput.Y * rotateCorrection), m_cameraPitchLimit.X, m_cameraPitchLimit.Y);

		//�V�����p�x�𔽉f
		pSpringArm->SetRelativeRotation(NewRotation);
	}
}

//
//�S���F�C�k
//
void AMyCharacter::UpdateMove(float DeltaTime)
{
	// ���݂�FPS����␳�l�̐ݒ�
	float fps = 1.0f / DeltaTime;
	float CorrectionValue = 60.f / fps;

	// �ړ���
	FVector NewVelocity = GetCharacterMovement()->Velocity;
	float RotateVelocity = MOVE_ROTATE_MIDDLE;

	// �ړ������̊m��
	FVector RightVec = m_pCamera->GetRightVector();
	FVector ForwardVec = m_pCamera->GetForwardVector();

	ForwardVec.Z = 0.f;
	ForwardVec.Normalize();

	// �㉺���E�ړ��̔��f
		float movement = m_moveVec.Size();

	// �󒆎��̓X�e�B�b�N�̈ړ��ʂ�}����
	if (GetCharacterMovement()->IsFalling())
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
	// �ڒn��
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


	// �l�̔��f
	GetCharacterMovement()->Velocity = NewVelocity;


	// ���b�V���̉�]
	USkeletalMeshComponent* pMeshComp = GetMesh();
	FRotator rot = pMeshComp->GetRelativeRotation();


	// �ړ����Ă���ꍇ
	if (m_moveVec.Size2D() > 0.5f)
	{

		//�A�[�N�^���W�F���g���g���ăR���g���[���[�̓��͕������Ȃ��p�x�����߂�
		float moveAngleDeg = atan2(m_moveVec.X, m_moveVec.Y);
		//Radian�l��Degree�ɕϊ�
		float angleDeg = FMath::RadiansToDegrees(moveAngleDeg);

		//���͂����p�x + ���b�V���̉�]�p�x + Actor�ɑ΂��ĉ�]���Ă���SpringArm�̑��Ίp�x
		float newYaw = angleDeg + GetBaseRotationOffsetRotator().Yaw + m_pSpringArm->GetRelativeRotation().Yaw + 180.0;

		float difference = newYaw - rot.Yaw;

		// �␳
		if (difference < 0.f)
		{
			difference += 360.f;
		}

		// �ő��]���x������X�s�[�h�ŉ�]���悤�Ƃ���ꍇ�A��������
		if (RotateVelocity < difference && difference <= 180.f)
		{
			newYaw = rot.Yaw + RotateVelocity * CorrectionValue;
		}
		else if (180.f < difference && difference < 360.f - RotateVelocity)
		{
			newYaw = rot.Yaw - RotateVelocity * CorrectionValue;
		}

		// ���f
		pMeshComp->SetRelativeRotation(FRotator(rot.Pitch, newYaw, rot.Roll));
	}
}

//
//�S���F�C�k
//
void AMyCharacter::UpdateJump(float _deltaTime)
{
	//�W�����v���t���O���m�F���Ă���
	if (m_bJumping)
	{
		//�A�j���[�V�����̂��߂ɒx�点��
		//
		//�W�����v�ʂ��v�Z
		m_nowJumpHeight = JUMP_HEIGHT;

		//�W�����v���Ԃ𑝉�
		m_jumpTime += _deltaTime;

		//Actor�̌��݂̍��W���擾
		FVector nowPos = GetActorLocation();

		////���n���i=�W�����v�ʂ��}�C�i�X�ɓ]�������j�W�����v�O��Ԃɖ߂�
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
			//���݂̍��W�ɃW�����v�ʂ𑫂�
			SetActorLocation(FVector(nowPos.X, nowPos.Y, m_posBeforeJump.Z + m_nowJumpHeight), true);

			//�W�����v�ʂ�ێ�
			m_prevJumpHeight = m_nowJumpHeight;

		}

		////���݂̍��W�ɃW�����v�ʂ𑫂�
		//SetActorLocation(FVector(nowPos.X, nowPos.Y, m_posBeforeJump.Z + m_nowJumpHeight), true);

		////�W�����v�ʂ�ێ�
		//m_prevJumpHeight = m_nowJumpHeight;


	}
}

//
//�S���F�C�k
//
//�{�[���𓊂���Input�C�x���g���Ă΂ꂽ���̏���
void AMyCharacter::Throw()
{
	//�{�[���������Ă��Ȃ��Ȃ�ȍ~�̏������s��Ȃ�
	if (!m_bIsHolding){
		return;
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		//������A�j���[�V�������Đ�
		AnimInstance->Montage_Play(ThrowBallMontage);
	}

}


//
//�S���F�C�k
//
//�{�[���𓊂��鏈��
void AMyCharacter::ThrowBall()
{
	//FVector fVector = UKismetMathLibrary::GetForwardVector(m_pSpringArm->GetForwardVector().Rotation());
	//FVector fVector = UKismetMathLibrary::GetForwardVector(m_pSpringArm->GetComponentRotation());

	//FVector fVector = UKismetMathLibrary::GetForwardVector(m_pSpringArm->GetComponentRotation());
	FVector fThrowVector = UKismetMathLibrary::GetForwardVector(m_pSpringArm->GetComponentRotation());

	FRotator fRotator(-30, 0, 0);
	FVector fVector = fRotator.RotateVector(FVector(fThrowVector.X, fThrowVector.Y, 0.0f));
	//FVector fVector = UKismetMathLibrary::MakeVector(30.f, m_pSpringArm->GetComponentRotation().Yaw, 0.f);

	//fVector = fVector.RotateAngleAxis(50, (FVector(0, 0, 1)));
	fVector *= THROW_POWER;

	m_bThrowAnim = true;

	m_pBall->m_pStaticMeshComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	m_pBall->ProjectileMovementComponent->SetActive(true);

	m_pBall->m_pStaticMeshComp->SetSimulatePhysics(true);
	m_pBall->m_pSphereComp->SetSimulatePhysics(false);
	//m_pBall->m_pSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	m_pBall->ProjectileMovementComponent->SetUpdatedComponent(m_pBall->GetRootComponent());
	m_pBall->m_pStaticMeshComp->SetPhysicsLinearVelocity(FVector::ZeroVector);

	//m_pBall->ProjectileMovementComponent->SetVelocityInLocalSpace(fVector);




	//m_pBall->m_pStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//m_pBall->SetActorEnableCollision(true);

	//m_pBall->m_pStaticMeshComp->SetSimulatePhysics(true);
	//m_pBall->m_pSphereComp->SetSimulatePhysics(false);

	//m_pBall->m_pSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//m_pBall->m_pStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);



	//m_pBall->m_pStaticMeshComp->SetSimulatePhysics(true);
	//m_pBall->m_pSphereComp->SetSimulatePhysics(true);
	//m_pBall->m_pSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//m_pBall->SetActorEnableCollision(true);

	
	//SetActorRotation(FRotator(0.0f, m_pSpringArm->GetTargetRotation().Yaw, 0.0f), ETeleportType::ResetPhysics);
	m_pBall->m_pStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	m_pBall->m_pStaticMeshComp->AddImpulse(fVector);
	//m_pBall->m_pStaticMeshComp->AddImpulse
}


//�S���F�C�k
//
//�{�[���𓊂�����̃t���O�ύX����
void AMyCharacter::ChangeBallFlag()
{
	//m_bIsHolding = false;
	m_bCanTackle = true;
	m_bThrowAnim = false;
	m_bCanHold = true;

	//m_pBall->SetActorEnableCollision(true);

	m_pBall->m_pSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}


//
//�S���F�C�k
//
//�X�t�B�A�R���|�[�l���g�ɃI�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐���o�^
void AMyCharacter::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�{�[���ێ����ƃ^�b�N�����́A�ȉ��̏������s��Ȃ�
	if (m_bIsHolding && m_bTackle){
		return;
	}

	//�I�[�o�[���b�v�����ۂɎ��s�������C�x���g
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//�I�[�o�[���b�v�����̂�CPU�̂Ƃ��̂ݔ�����������
		if (OtherActor->ActorHasTag("Ball")) {
			//Ball���擾
			m_pBall = Cast<ABall>(OtherActor);
			m_pBall->AttachToComponent(GetMesh(), { EAttachmentRule::SnapToTarget, true }, "BallCatch");

			//�{�[���̋����A�R���W������ύX
			m_pBall->ProjectileMovementComponent->SetActive(false);
			m_pBall->m_pStaticMeshComp->SetSimulatePhysics(false);
			m_pBall->m_pSphereComp->SetSimulatePhysics(false);
			m_pBall->m_pSphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			m_pBall->m_pStaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

			//m_pBall->SetActorEnableCollision(false);

			//�t���O��ݒ�
			m_bIsHolding = true;
			m_bCanHold = false;
			m_bCanTackle = false;
		}
	}
}

//
//�S���F�C�k
//
//���̓o�C���h �J������]�FPitch(Y��)
void AMyCharacter::Camera_RotatePitch(float _axisValue)
{
	m_cameraRotateInput.Y = -_axisValue * 3.0f;
}

//
//�S���F�C�k
//
//���̓o�C���h �J������]�FYaw(Z��)
void AMyCharacter::Camera_RotateYaw(float _axisValue)
{
	m_cameraRotateInput.X = _axisValue * 3.0f;
}

//
//�S���F�C�k
//
//���̓o�C���h �L�����ړ��F�O��
void AMyCharacter::Chara_MoveForward(const float _axisValue)
{
	//�R���g�[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	if ((Controller != NULL) && (_axisValue != 0.0f))
	{
		// �f�b�h�]�[���͖���
		bool isDeadZone = -MOVE_INVALIDRANGE <= _axisValue && _axisValue <= MOVE_INVALIDRANGE;
		m_moveVec.Y = isDeadZone ? 0.f : _axisValue;

		m_moveVec.Y = _axisValue;

	}
}


//
//�S���F�C�k
//
//���̓o�C���h �L�����ړ��F���E
void AMyCharacter::Chara_MoveRight(float _axisValue)
{
	//�R���g�[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	if ((Controller != NULL) && (_axisValue != 0.0f))
	{
		// �f�b�h�]�[���͖���
		bool isDeadZone = -MOVE_INVALIDRANGE <= _axisValue && _axisValue <= MOVE_INVALIDRANGE;
		m_moveVec.X = isDeadZone ? 0.f : _axisValue;

		m_moveVec.X = _axisValue;
	}
}

//
//�S���F�C�k
//
//���̓o�C���h�@�W�����v
void AMyCharacter::JumpStart()
{
	//�R���g�[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	//�W�����v���ł͂Ȃ��ꍇ
	if (m_bJumping == false)
	{
		//�W�����v����Ԃɂ���
		m_bJumping = true;

		//�W�����v�O��Actor���W��ێ�
		m_posBeforeJump = GetActorLocation();
	}
}


//
//�S���F��{
//
//���̓o�C���h �^�b�N��
void AMyCharacter::TackleStart() {
	////�R���g�[���\�ȏꍇ�̂�
	//if (m_bCanTackle != false) { return; }

	//if (m_bTackle == false) {
	//	m_bTackle = true;
	//	m_bCanTackle = false;
	//}
}



//
//�S���F��{
//
//�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐���o�^
void AMyCharacter::ActorOnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�I�[�o�[���b�v�����ۂɎ��s�������C�x���g
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//�I�[�o�[���b�v�����̂�CPU�̂Ƃ��̂ݔ�����������
		if (OtherActor->ActorHasTag("CPU")) {
			//�ڐG�t���O�𗧂Ă�
			m_bOverlappedCPU = true;
			//CPU���擾
			m_Overlaped_CPU = OtherActor;
		}
	}
}