//---------------------------------------
// �t�@�C����	�FGoalBase.h
// �T�v			�F�S�[���̃x�[�X�N���X
// �쐬��		�F21cu0216 ��{���
// �X�V���e		�F2022/11/28 �쐬
// 
//---------------------------------------

//�C���N���[�h�K�[�h
#pragma once

//�C���N���[�h
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalBase.generated.h"

//�O���錾
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class HANDBALLER_API AGoalBase : public AActor
{
	//UE4�̃I�u�W�F�N�g�N���X�ŕK���擪�ɏ����}�N��
	GENERATED_BODY()

protected:
	//�S�[���̃X�e�[�g
	enum GOAL_STATE {
		GOAL_STATE_NONE = 0,	//�����N�����Ă��Ȃ�
		GOAL_STATE_COLLITED,	//�ڐG��
		GOAL_STATE_DESTROY,	//���Ŏ�
		GOAL_STATE_NUM	//�X�e�[�g�̐�
	};

public:
	// �R���X�g���N�^
	AGoalBase();

protected:
	// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:
	// ���t���[���̍X�V����
	virtual void Tick(float DeltaTime) override;

protected:
	//�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐���o�^
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//�G�f�B�^���ŕҏW�ł��郁���o�ϐ�
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* m_pBoxComp;	//�A�C�e���ڐG�p�R���W����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pGoalMesh;	//�S�[�����b�V��

protected:
	UPROPERTY()
		USceneComponent* m_pBase;	//�x�[�X�R���|�[�l���g

protected:
	GOAL_STATE m_state;	//�X�e�[�g�Ǘ�
};
