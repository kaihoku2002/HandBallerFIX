//---------------------------------------------------------------------------------------------------------------------
// �t�@�C����	�FGoalBase.cpp
// �T�v			�F�S�[���̃x�[�X�N���X
// �쐬��		�F21cu0216 ��{���
// �X�V���e		�F2022/11/28 �쐬
//							 �S�[���̃x�[�X�N���X���쐬
//---------------------------------------------------------------------------------------------------------------------

//�C���N���[�h
#include "GoalBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// �R���X�g���N�^
AGoalBase::AGoalBase()
	:m_state(GOAL_STATE_NONE)
{
	// ���t���[���A���̃N���X��Tick()���ĂԂ��ǂ��������߂�t���O
	PrimaryActorTick.bCanEverTick = true;

	//���[�g�ɒu���x�[�X�I�u�W�F�N�g����
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = m_pBase;

	//�R���W��������p�{�b�N�X�R���|�[�l���g����
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	m_pBoxComp->SetupAttachment(RootComponent);

	//���b�V���R���|�[�l���g����(StaticMesh)
	m_pGoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoalMesh"));
	m_pGoalMesh->SetupAttachment(RootComponent);
}

// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void AGoalBase::BeginPlay()
{
	Super::BeginPlay();

	//�I�[�o�[���b�v���n�߂��Ƃ��ɌĂ΂��C�x���g�֐���o�^
	if (m_pBoxComp != NULL) {
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AGoalBase::OnOverlapBegin);
	}
}

// ���t���[���̍X�V����
void AGoalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//�I�[�o�[���b�v�ڐG���n�߂����̏���
void AGoalBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Overlap�����ۂɎ��s�������C�x���g

	//�h���N���X�ŉ���������Ă��Ȃ���΁A��{�N���X�ɏ�����������̊֐����Ă΂��
}