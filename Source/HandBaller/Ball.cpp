// Fill out your copyright notice in the Description page of Project Settings.


// �C���N���[�h
#include "Ball.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameUtility.h"
#include "GameFramework/Character.h"

// �R���X�g���N�^
ABall::ABall()
	: m_pSphereComp(NULL)
	, m_pStaticMeshComp(NULL)
	, m_pBase(NULL)
	, m_liftHeightRate(0.25f)
	, m_pStaticMesh(NULL)
	, m_pPlayerChara(NULL)
	, m_meshSize(FVector::ZeroVector)
	, m_isLift(false)
{
	// ���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O�B�K�v�ɉ����āA�p�t�H�[�}���X����̂��߂ɐ؂邱�Ƃ̂ł���B
	PrimaryActorTick.bCanEverTick = true;

	// �x�[�X�I�u�W�F�N�g�𐶐�
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("BaseComponent"));
	RootComponent = m_pBase;


	// �R���W��������p�{�b�N�X�R���|�[�l���g�𐶐�
	m_pSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SpherenComp"));
	m_pSphereComp->SetupAttachment(RootComponent);

	// �X�^�e�B�b�N���b�V���R���|�[�l���g�𐶐�
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	m_pStaticMeshComp->SetupAttachment(RootComponent);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(m_pStaticMeshComp);


}

// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void ABall::BeginPlay()
{
	Super::BeginPlay();

	if (m_pStaticMeshComp != NULL)
	{
		// ���b�V���̒��g���擾
		m_pStaticMesh = m_pStaticMeshComp->GetStaticMesh();
	}

	// ���b�V���̒��g�������ł���ΐi�߂鎖�͂ł��Ȃ�
	if (m_pStaticMesh != NULL)
	{
		// ���b�V���̃o�E���e�B�{�b�N�X�T�C�Y�����߂�
		m_meshSize = m_pStaticMesh->GetBounds().GetBox().GetSize();

		// �I�u�W�F�N�gActor�̃T�C�Y����������
		m_meshSize *= GetActorScale();
	}

	// Tick()�����̎��s��Ԃ𖳌��ɂ��Ă���
	PrimaryActorTick.SetTickFunctionEnable(false);
	ProjectileMovementComponent->SetActive(false, false);
}

// ���t���[���̍X�V����
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//// �����グ����I�u�W�F�N�g���ʂ̍X�V
	//UpdateLiftObject(DeltaTime);
}

// �����グ����I�u�W�F�N�g���ʂ̍X�V
void ABall::UpdateLiftObject(float _deltaTime)
{
	// �v���C���[�̃|�C���^���A�ŏ��̈�񂾂��擾�i���������̃R�X�g���������߁j
	if (m_pPlayerChara == NULL)
	{
		// Utility�N���X���g��
		m_pPlayerChara = CGameUtility::GetActorFromTag(this, "Player");

		// �Q�[���S�̂ɑ΂���Actor�̌��������̓R�X�g���������߁ABeginPlay�ň��ۑ��ɂ��Ă��������ɂ���
		// �����Ώۂ͑S�Ă�Actor
		/*TSubclassOf<AActor> findClass;
		findClass = AActor::StaticClass();
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);*/

		// �������ʁAActor�������
		//if (actors.Num() > 0)
		//{
		//	// ����Actor�̒������ԂɌ���
		//	for (int idx = 0; idx < actors.Num(); idx++)
		//	{
		//		AActor* pActor = Cast<AActor>(actors[idx]);

		//		// �^�O���Ŕ��ʂ���
		//		// �i���O�̓G�f�B�^��ŏd�������ꍇ�͓����̖��O���ς�邽�߁A���ĂɂȂ�Ȃ��j
		//		if (pActor->ActorHasTag("Player"))
		//		{
		//			////�f�o�b�O�m�F�p
		//			// FString message = FString("Founded PlayerChara :") + pActor->GetName();
		//			// UE_LOG(LogTemp, Warning, TEXT("%s"), *message);

		//			m_pPlayerChara = pActor;
		//			break;
		//		}
		//	}
		//}
	}

	// �v���C���[�̍��W��Ǐ]���ē�����
	if (m_pPlayerChara != NULL)
	{
		// �v���C���[�̃��b�V��(SkeletalMesh)���擾
		// APlayerChara�N���X�̊�{�N���X�iACharacter�j�����b�V���������Ă��邽�߁A
		// ACharacter�^�ɃL���X�g����GetMesh()���ĂԂ��Ƃ��ł���
		USkeletalMeshComponent* pPlayerMesh = Cast<ACharacter>(m_pPlayerChara)->GetMesh();

		// �v���C���[�̍��W�E��]�擾
		FVector playerCharaLocation = pPlayerMesh->GetComponentLocation();
		FRotator playerCharaRotation = pPlayerMesh->GetComponentRotation();

		// �v���C���[�̃��b�V���T�C�Y
		FVector playerMeshSize = pPlayerMesh->Bounds.GetBox().GetSize() * m_pPlayerChara->GetActorScale();

		// �I�u�W�F�N�g�������グ�����̍��W���v�Z
		// �v���C���[�̑O�ɁA�I�u�W�F�N�g�̃T�C�Y���l�������ʒu�ɔz�u������
		FVector newLocation = playerCharaLocation + pPlayerMesh->GetForwardVector() * (m_meshSize + playerMeshSize) / 2;

		// �����グ���������A�v���C���[���b�V���̍������Q�Ƃ��āA������ɂ�������
		if (m_isLift == true)
		{
			newLocation += FVector(0, 0, playerMeshSize.Z * m_liftHeightRate);
		}

		// �I�u�W�F�N�g�̍��W�E��]��ݒ�
		SetActorLocation(FVector(newLocation));
		SetActorRotation(FRotator(playerCharaRotation));

		// ���̃I�u�W�F�N�g�̃R���W���������؂�ւ���
		// �����グ�����͖����ɁA�~�낷�Ƃ��͗L����
		this->SetActorEnableCollision(m_isLift ? false : true);
	}

	// �~�낷�Ƃ��A�I�u�W�F�N�g��Tick()�����̎��s��Ԃ𖳌��ɂ���
	if (m_isLift == false)
	{
		PrimaryActorTick.SetTickFunctionEnable(false);
	}

}

// �����グ��Ԃ̐ݒ�
void ABall::SetIsLift(bool _isLift)
{
	m_isLift = _isLift;

	// �����グ�鎞�A�I�u�W�F�N�g��Tcik()�����̎��s��Ԃ�L���ɂ���
	if (m_isLift == true)
	{
		PrimaryActorTick.SetTickFunctionEnable(true);
	}
}