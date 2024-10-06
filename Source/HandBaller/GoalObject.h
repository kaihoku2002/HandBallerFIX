//------------------------------------------------------------------------------
// �t�@�C����	�FGoalObject.h
// �T�v			�F�S�[���I�u�W�F�N�g�N���X�w�b�_�[
// �쐬��		�F21cu0216 ��{���
// �X�V���e		�F2022/11/28 �쐬
// 
//------------------------------------------------------------------------------

//�C���N���[�h�K�[�h
#pragma once
//�C���N���[�h
#include "CoreMinimal.h"
#include "GoalBase.h"
#include "GoalObject.generated.h"

/**
 *
 */
UCLASS()
class HANDBALLER_API AGoalObject : public AGoalBase
{
	GENERATED_BODY()

public:
	//�R���X�g���N�^
	AGoalObject();

protected:
	// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:
	// ���t���[���̍X�V����
	virtual void Tick(float DeltaTime) override;

protected:
	//�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐���o�^
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

private:
	int m_iGoalScore;	//�l�������_��

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool m_bOverlappedGoal;	//�S�[���ɐڐG�������̃t���O

	UFUNCTION(BlueprintCallable, Category = "MY Functions")
		int GetGoalScore()const { return m_iGoalScore; }
};
