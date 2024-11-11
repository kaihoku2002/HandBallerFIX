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


	// �G�f�B�^���ŕҏW�ł��郁���o�ϐ�
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* m_pSphereComp;				// �ڐG�p�R���W����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pStaticMeshComp;	// �X�^�e�B�b�N���b�V���R���|�\�l���g

	// Projectile movement component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_firstPos;				// �ڐG�p�R���W����



	UPROPERTY()
		USceneComponent* m_pBase;				// �x�[�X�R���|�[�l���g

private:
	UStaticMesh* m_pStaticMesh;					// �X�^�e�B�b�N���b�V��
	FVector m_meshSize;							// �X�^�e�B�b�N���b�V���̃T�C�Y
};
