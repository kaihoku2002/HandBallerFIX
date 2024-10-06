

#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameUtility.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#define JUMP_HEIGHT (m_jumpTime * m_jumpTime * (-m_gravity) / 2 + (m_jumpTime * m_jumpPower))
#define SET_JUMP_TIME 0.5

//
//�S���F�C�k
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
	//�f�t�H���g�v���C���[�Ƃ��Đݒ�
	AutoPossessPlayer = EAutoReceiveInput::Player0;


	//�X�v�����O�A�[���̃I�u�W�F�N�g�𐶐�
	m_pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("m_pSpringArm"));
	if (m_pSpringArm != NULL)
	{
		m_pSpringArm->SetupAttachment(RootComponent);
		//�A�[���̒�����ݒ�
		//�J�����̃R���W�����e�X�g���s�����ݒ�
		m_pSpringArm->bDoCollisionTest = false;
		////�J�����Ǐ]���O���g������ݒ�
		//m_pSpringArm->bEnableCameraLag = true;
		////�J�����Ǐ]���O�̑��x��ݒ�
		//m_pSpringArm->CameraLagSpeed = 10.f;
		////�J������]���O���g������ݒ�
		//m_pSpringArm->bEnableCameraRotationLag = true;
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

	//�V���[�g
	InputComponent->BindAction("shoot", IE_Pressed, this, &AMyCharacter::Shoot);
	//�^�b�N��
	InputComponent->BindAction("Tackle", IE_Pressed, this, &AMyCharacter::TackleStart);

	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);

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
	//�ړ����͂�����ꍇ
	if (!m_charaMoveInput.IsZero() && (DeltaTime != 0.0f))
	{
		//���͂ɍ��킹�āAActor�����E�O��Ɉړ�
		USpringArmComponent* pSpringArm = m_pSpringArm;
		if (pSpringArm != NULL)
		{
			FVector NewLocation = GetActorLocation();

			//�L�����N�^�[�̈ړ�
			{
				//SpringArm�����������ɁA���͂ɂ��ړ��ʂ�PawnMoveComponent�ɓn��
				FVector forwardVec = pSpringArm->GetForwardVector();
				AddMovementInput(forwardVec, m_charaMoveInput.Y * m_moveSpeed);
				FVector rightVec = pSpringArm->GetRightVector();
				AddMovementInput(rightVec, m_charaMoveInput.X * m_moveSpeed);

			}

			//�ړ�����L�����N�^�[�̉�]
			{
				//�p�x��Degree�̊p�x�͈̔͂ŕ\�L(-180 ~ 180)
				USkeletalMeshComponent* pMeshComp = GetMesh();
				if (pMeshComp != NULL)
				{
					//�A�[�N�^���W�F���g���g���ăR���g���[���[�̓��͕������Ȃ��p�x�����߂�
					float angle = atan2(m_charaMoveInput.X, m_charaMoveInput.Y);
					//Radian�l��Degree�ɕϊ�
					float angleDeg = FMath::RadiansToDegrees(angle);

					//���͂����p�x + ���b�V���̉�]�p�x + Actor�ɑ΂��ĉ�]���Ă���SpringArm�̑��Ίp�x
					float newYaw = angleDeg + GetBaseRotationOffsetRotator().Yaw + pSpringArm->GetRelativeRotation().Yaw;

					pMeshComp->SetRelativeRotation(FRotator(pMeshComp->GetRelativeRotation().Pitch, newYaw, pMeshComp->GetRelativeRotation().Roll));

				}
			}
		}
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
void AMyCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
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
void AMyCharacter::Chara_MoveForward(float _axisValue)
{
	//�R���g�[���\�ȏꍇ�̂�
	if (m_bCanControl == false) { return; }

	if ((Controller != NULL) && (_axisValue != 0.0f))
	{
		//�R���g���[���[�̓��͕������擾����
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//�O�����̌������擾����
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		//�J��o��
		AddMovementInput(Direction, _axisValue);
	}
	//m_charaMoveInput.Y = FMath::Clamp(_axisValue, -1.0f, 10000.0f) * 10000.0f;

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
		//�R���g���[���[�̓��͕������擾����
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//���E�����̌������擾����
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		//�J��o��
		AddMovementInput(Direction, _axisValue);

	}
	//m_charaMoveInput.X = FMath::Clamp(_axisValue, -1.0f, 10000.0f) * 10000.0f;


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

//���̓o�C���h�@�V���[�g
void AMyCharacter::Shoot()
{

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