// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameUtility.h"

#define JUMP_HEIGHT (m_jumpTime * m_jumpTime * (-m_gravity) / 2 + (m_jumpTime * m_jumpPower))


// Sets default values
AHuman::AHuman()
	: m_jumpPower(750.f)
	, m_gravity(980.f)
	, m_bCanControl(true)
	, m_jumpTime(0.f)
	, m_nowJumpHeight(0.f)
	, m_prevJumpHeight(0.f)
	, m_bJumping(false)
	, m_charaMoveInput(FVector2D::ZeroVector)
	, m_cameraRotateInput(FVector2D::ZeroVector)
	, m_moveSpeed(10000.0f)
	, m_bIsHoleding(false)
	, m_TackleSpeed(10000000.f)
	, m_bTackle(false)
	, m_bCanTackle(true)
	, m_iTime(0)
	, m_iTimeCnt(0)
	, m_iTackleTime(5)

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�R���W��������p�R���|�[�l���g����
	m_pSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	m_pSphereComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHuman::BeginPlay()
{
	Super::BeginPlay();
	
	//�I�[�o�[���b�v���n�߂��Ƃ��ɌĂ΂��C�x���g�֐���o�^
	if (m_pSphereComp != NULL) {
		m_pSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHuman::ActorOnOverlapBeginComponent);
	}

}

// Called every frame
void AHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//�ړ�����
	UpdateMove(DeltaTime);

	//�W�����v����
	UpdateJump(DeltaTime);

}

// Called to bind functionality to input
void AHuman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHuman::UpdateCamera(float DeltaTime)
{

}

void AHuman::UpdateMove(float DeltaTime)
{

}

void AHuman::UpdateJump(float _deltaTime)
{

}

//���̓o�C���h �J������]�FPitch(Y��)
void AHuman::Camera_RotatePitch(float _axisValue)
{

}

//���̓o�C���h �J������]�FYaw(Z��)
void AHuman::Camera_RotateYaw(float _axisValue)
{
}

//���̓o�C���h �L�����ړ��F�O��
void AHuman::Chara_MoveForward(float _axisValue)
{
}

//���̓o�C���h �L�����ړ��F���E
void AHuman::Chara_MoveRight(float _axisValue)
{
}

//���̓o�C���h�@�W�����v
void AHuman::JumpStart()
{

}

//���̓o�C���h�@�V���[�g
void AHuman::Shoot()
{

}

//���̓o�C���h�@�^�b�N��
void AHuman::TackleStart()
{

}

//���̓o�C���h�@�V���[�g
void AHuman::TurnAtRate(float Rate)
{

}

//�^�b�N������
void AHuman::UpdateTackle()
{
	if (m_bTackle != false)
	{
		//���̎��Ԃ��o�߂���܂Ń^�b�N���s��
		if (m_iTime < m_iTackleTime) {
			//���t���[�����Z
			m_iTimeCnt++;
			if (m_iTimeCnt % 60 == 0) {
				m_iTimeCnt = 0;
				m_iTime++;

				//AddForce(FVecter Force, FName BoneName, Bool bAccelChange)
				//AddForce();
			}
		}
		else {
			//�l��������
			m_iTimeCnt = 0;
			m_iTime = 0;
			m_bCanTackle = true;
			m_bTackle = false;
		}
	}
}

//�I�[�o�[���b�v�ڐG���n�߂����̏���
void AHuman::ActorOnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Overlap�����ۂɎ��s�������C�x���g

	//�h���N���X�ŉ���������Ă��Ȃ���΁A��{�N���X�ɏ�����������̊֐����Ă΂��
}