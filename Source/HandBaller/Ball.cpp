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
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

// �R���X�g���N�^
ABall::ABall()
	: m_pSphereComp(NULL)
	, m_pStaticMeshComp(NULL)
	, m_pBase(NULL)
	, m_pStaticMesh(NULL)
	, m_meshSize(FVector::ZeroVector)
	, m_firstPos(0.0, 0.0, 0.0)
{
	// ���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O�B�K�v�ɉ����āA�p�t�H�[�}���X����̂��߂ɐ؂邱�Ƃ̂ł���B
	PrimaryActorTick.bCanEverTick = true;

	// �x�[�X�I�u�W�F�N�g�𐶐�
	//m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("BaseComponent"));
	//RootComponent = m_pBase;
	

	// �X�^�e�B�b�N���b�V���R���|�[�l���g�𐶐�
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = m_pStaticMeshComp;


	// �R���W��������p�{�b�N�X�R���|�[�l���g�𐶐�
	m_pSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SpherenComp"));
	m_pSphereComp->SetupAttachment(RootComponent);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	// �^�O�̐ݒ�
	Tags.Add(FName("Ball"));

}

// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void ABall::BeginPlay()
{
	Super::BeginPlay();

}

// ���t���[���̍X�V����
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


