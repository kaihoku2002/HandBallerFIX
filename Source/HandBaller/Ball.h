// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

// �O���錾
class USphereComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class UStaticMesh;
class UProjectileMovementComponent;

UCLASS()
class ABall : public AActor
{
	GENERATED_BODY()

public:
	// �R���X�g���N�^
	ABall();

protected:
	// �Q�[���X�^�[�g���A�܂��͐������ɌĂ΂�鏈��
	virtual void BeginPlay() override;

public:
	// ���t���[���̍X�V����
	virtual void Tick(float DeltaTime) override;

private:
	// �����グ����I�u�W�F�N�g���ʂ̍X�V
	void UpdateLiftObject(float _deltaTime);

public:
	// �����グ��Ԃ̐ݒ�
	void SetIsLift(bool _isLift);

	// �����グ��ԃt���O�̎擾
	bool GetIsLift() const { return m_isLift; }

	// �G�f�B�^���ŕҏW�ł��郁���o�ϐ�
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* m_pSphereComp;				// �ڐG�p�R���W����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pStaticMeshComp;	// �X�^�e�B�b�N���b�V���R���|�\�l���g

	// Projectile movement component.
	UPROPERTY(BlueprintReadWrite, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	// �h���N���X�Ŏg����悤��protected�ɐݒ肷��ϐ�

	UPROPERTY()
		USceneComponent* m_pBase;				// �x�[�X�R���|�[�l���g
protected:
	UPROPERTY()
		float m_liftHeightRate;					// �����グ�����̍����䗦

private:
	UStaticMesh* m_pStaticMesh;					// �X�^�e�B�b�N���b�V��
	AActor* m_pPlayerChara;						// �v���C���[�̏��i��{�N���X�|�C���^�j
	FVector m_meshSize;							// �X�^�e�B�b�N���b�V���̃T�C�Y

	bool m_isLift;								// �����グ��ԃt���O
};
