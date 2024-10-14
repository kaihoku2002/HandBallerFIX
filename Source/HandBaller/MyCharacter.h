// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Human.h"
#include "MyCharacter.generated.h"

/**
 * 
 */
 //�O���錾
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class HANDBALLER_API AMyCharacter : public AHuman
{
	GENERATED_BODY()
public:
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
private:
	//���̓o�C���h
		//���̓o�C���h �J������]�FPitch[Y��]
	virtual void Camera_RotatePitch(const float _axisValue);
	//���̓o�C���h �J������]�FYaw[X��]
	virtual void Camera_RotateYaw(const float _axisValue);

	//���̓o�C���h �L�����ړ��F�O��
	virtual void Chara_MoveForward(const float _axisValue);
	//���̓o�C���h �L�����ړ��F���E
	virtual void Chara_MoveRight(const float _axisValue);

	//���̓o�C���h �W�����v
	virtual void JumpStart();

	virtual void Shoot();
	//�W�����v����
	virtual void UpdateJump(float DeltaTime);

	//���̓o�C���h �^�b�N��
	virtual void TackleStart();


	// �X�V����
private:

	//�ړ�����
	virtual void UpdateMove(float DeltaTime);
	//�J�����̍X�V����
	virtual void UpdateCamera(float DeltaTime);

	////�^�b�N������
	//virtual void UpdateTackle();


	// �ړ������p�x�N�g��
	FVector m_moveVec;		// �ړ��p�x�N�g��


public:
	//------------------�{�[������-------------------------
	//

private:
	//UPRPPERTY�ɂ��邱�ƂŁA�u���[�v�����g��ŕϐ��̊m�F�A�ҏW�Ȃǂ��ł���
//[BuleprintReadOnly]�Ɏw�肵�Ă��邽�߁A�u���[�v�����g�Ō��邱�Ƃ������\�ŕҏW�͂ł��Ȃ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera, meta =
		(AllowPrivateAccess = "true"))
		USpringArmComponent* m_pSpringArm;		//�X�v�����O�A�[��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera, meta =
		(AllowPrivateAccess = "true"))
		UCameraComponent* m_pCamera;		//�J����


	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector2D m_cameraPitchLimit;		//�J�����̃s�b�`�͈�

	FVector2D m_charaMoveInput;		//Pawn�ړ����͗�
	FVector2D m_cameraRotateInput;		//Pawn�ړ����͗�

	bool m_bCanControl;		//����\�ȏ�Ԃ��H

protected:
	//�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐���o�^
	void ActorOnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bOverlappedCPU;	//CPU��Overlap���Ă��邩�ǂ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* m_Overlaped_CPU;

protected:
	// �v���C���[�̒萔--------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Player")
	float GRAVITY;				// �d��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Player")
	float FALLSPEED_MAX;		// Z�����̍ő呬�x
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Player")
	float MOVESPEED_MAX;		// X-Y�����̍ő�ړ����x


	// �ړ�						
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
	float MOVE_INVALIDRANGE;		// ���̖͂����͈�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
	float MOVE_SPEED_MAX;		// �ړ����x(�ő�)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
	float MOVE_SPEED_MIDDLE;		// �ړ����x(���i�K)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
	float MOVE_SPEED_MIN;		// �ړ����x(���i�K)	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
	float MOVE_SPEED_FALLING;	// �ړ����x(�󒆎�)	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
	float MOVE_ROTATE_MAX;		// ��]���x(�󒆎�)	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
	float MOVE_ROTATE_MIDDLE;	// ��]���x(�󒆎�)	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
	float MOVE_ROTATE_MIN;		// ��]���x(�󒆎�)	


};
