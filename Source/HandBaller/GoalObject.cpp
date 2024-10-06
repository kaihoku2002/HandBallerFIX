//---------------------------------------------------------------------------------------------------------------------
// �t�@�C����	�FGoalObject.cpp
// �T�v			�F�S�[���I�u�W�F�N�g�N���X
// �쐬��		�F21cu0216 ��{���
// �X�V���e		�F2022/11/28 �쐬
//							 �S�[���I�u�W�F�N�g���쐬
//				�@2022/11/29 �{�[�����S�[���I�u�W�F�N�g�ɐG���ƁA1�_�ǉ�����悤�ɏC��
//---------------------------------------------------------------------------------------------------------------------

//�C���N���[�h
#include "GoalObject.h"

//�R���X�g���N�^
AGoalObject::AGoalObject()
	:m_bOverlappedGoal(false)
	, m_iGoalScore(0)
{
}

// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
void AGoalObject::BeginPlay()
{
	Super::BeginPlay();

}

// ���t���[���̍X�V����
void AGoalObject::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//�ڐG�t���O����������
	if (m_bOverlappedGoal) {
		//�P�_�ǉ�
		++m_iGoalScore;

		//�ڐG�t���O�͉����Ă���
		m_bOverlappedGoal = false;
	}
}

//�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐���o�^
void AGoalObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//�I�[�o�[���b�v�����ۂɎ��s�������C�x���g
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//�I�[�o�[���b�v�����̂��{�[���̂Ƃ��̂ݔ�����������
		if (OtherActor->ActorHasTag("Ball")) {
			//�ڐG�t���O�𗧂Ă�
			m_bOverlappedGoal = true;
		}
	}
}