// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Human.generated.h"

//�O���錾
class USphereComponent;

UCLASS()
class HANDBALLER_API AHuman : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHuman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



protected:
	//�J�����̍X�V����
	virtual void UpdateCamera(float DeltaTime);
	//�ړ�����
	virtual void UpdateMove(float DeltaTime);
	//�W�����v����
	virtual void UpdateJump(float DeltaTime);
	//�^�b�N������
	virtual void UpdateTackle();

private:
	//���̓o�C���h
		//���̓o�C���h �J������]�FPitch[Y��]
	virtual void Camera_RotatePitch(float _axisValue);
	//���̓o�C���h �J������]�FYaw[X��]
	virtual void Camera_RotateYaw(float _axisValue);

	//���̓o�C���h �L�����ړ��F�O��
	virtual void Chara_MoveForward(float _axisValue);
	//���̓o�C���h �L�����ړ��F���E
	virtual void Chara_MoveRight(float _axisValue);

	//���̓o�C���h �W�����v
	virtual void JumpStart();

	//���̓o�C���h �W�����v
	virtual void Shoot();

	//���̓o�C���h �^�b�N��
	virtual void TackleStart();

	virtual void TurnAtRate(float Rate);

public:
	//UPROPERTY(EditAnywhere, Category = "Move")
	//	float m_moveSpeed;		//�ړ���

	//-------------------------�W�����v---------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		float m_jumpPower;		//�W�����v��

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
		float m_jumpTime;
		//�W�����v����
	float m_nowJumpHeight;		//���݃t���[���̃W�����v��
	float m_prevJumpHeight;		//�O�t���[���̃W�����v��

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool m_bJumping;			//�W�����v���̃��O

	FVector m_posBeforeJump;	//�W�����v�J�n�O�̃L�����N�^�[���W

	//-------------------------�W�����v---------------------------------


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		float m_gravity;		//�d��
	//�����Ă���Œ���
	UPROPERTY(EditAnywhere, Category = "Throw")
		bool IsThrowing;

protected:
	bool m_bCanControl;		//����\�ȏ�Ԃ��H


protected:
	FVector2D m_charaMoveInput;		//Pawn�ړ����͗�
	FVector2D m_cameraRotateInput;		//Pawn�ړ����͗�

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_moveSpeed;	//�^�b�N���̑��x
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bIsHoleding;	//�{�[���������Ă��邩�ǂ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_TackleSpeed;	//�^�b�N���̑��x
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bTackle;		//�^�b�N�����̃t���O
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bCanTackle;		//�^�b�N���ł��邩�ǂ����̃t���O
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iTimeCnt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iTackleTime;	//�^�b�N���N�[���_�E��

public:
	//�N�[���^�C���̃J�E���^�[�i�b�j
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CoolTime")
		int m_CoolTimeCnt;
	//�N�[���^�C���̃J�E���^�[�i�t���[���j
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CoolTime")
		int m_CoolTimeFlameCnt;
	//�N�[���^�C�����N�����邩�ǂ����̃t���O
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CoolTime")
		bool m_bCoolTimeStarter;
	/*�N�[���^�C���̏���
	* CoolTimeCounter(/�����F�N�[���^�C��/)
	*/
	UFUNCTION(BlueprintCallable, Category = "CoolTime")
		void CoolTimeCounter(const int _CoolTime)
	{
		if (_CoolTime > m_CoolTimeCnt == true) {
			++m_CoolTimeFlameCnt;
			if (m_CoolTimeFlameCnt % 60 == 0) {
				++m_CoolTimeCnt;
				m_CoolTimeFlameCnt = 0;
			}
		}
		//������
		else {
			m_CoolTimeCnt = 0;
			m_bCoolTimeStarter = false;
		}
	}
protected:
	UFUNCTION()
		//�I�[�o�[���b�v�ڐG���n�߂����ɌĂ΂��C�x���g�֐���o�^
		virtual void ActorOnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* m_pSphereComp;	//�A�C�e���ڐG�p�R���W����


};
