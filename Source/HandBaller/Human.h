// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Human.generated.h"

//前方宣言
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
	//カメラの更新処理
	virtual void UpdateCamera(float DeltaTime);
	//移動処理
	virtual void UpdateMove(float DeltaTime);
	//ジャンプ処理
	virtual void UpdateJump(float DeltaTime);
	//タックル処理
	virtual void UpdateTackle();

private:
	//入力バインド
		//入力バインド カメラ回転：Pitch[Y軸]
	virtual void Camera_RotatePitch(float _axisValue);
	//入力バインド カメラ回転：Yaw[X軸]
	virtual void Camera_RotateYaw(float _axisValue);

	//入力バインド キャラ移動：前後
	virtual void Chara_MoveForward(float _axisValue);
	//入力バインド キャラ移動：左右
	virtual void Chara_MoveRight(float _axisValue);

	//入力バインド ジャンプ
	virtual void JumpStart();

	//入力バインド ジャンプ
	virtual void Shoot();

	//入力バインド タックル
	virtual void TackleStart();

	virtual void TurnAtRate(float Rate);

public:
	//UPROPERTY(EditAnywhere, Category = "Move")
	//	float m_moveSpeed;		//移動量

	//-------------------------ジャンプ---------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		float m_jumpPower;		//ジャンプ力

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jump")
		float m_jumpTime;
		//ジャンプ時間
	float m_nowJumpHeight;		//現在フレームのジャンプ量
	float m_prevJumpHeight;		//前フレームのジャンプ量

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool m_bJumping;			//ジャンプ中のラグ

	FVector m_posBeforeJump;	//ジャンプ開始前のキャラクター座標

	//-------------------------ジャンプ---------------------------------


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Jump")
		float m_gravity;		//重力
	//投げている最中か
	UPROPERTY(EditAnywhere, Category = "Throw")
		bool IsThrowing;

protected:
	bool m_bCanControl;		//操作可能な状態か？


protected:
	FVector2D m_charaMoveInput;		//Pawn移動入力量
	FVector2D m_cameraRotateInput;		//Pawn移動入力量

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_moveSpeed;	//タックルの速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bIsHoleding;	//ボールを持っているかどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float m_TackleSpeed;	//タックルの速度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bTackle;		//タックル中のフラグ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bCanTackle;		//タックルできるかどうかのフラグ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iTimeCnt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int m_iTackleTime;	//タックルクールダウン

public:
	//クールタイムのカウンター（秒）
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CoolTime")
		int m_CoolTimeCnt;
	//クールタイムのカウンター（フレーム）
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CoolTime")
		int m_CoolTimeFlameCnt;
	//クールタイムを起動するかどうかのフラグ
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CoolTime")
		bool m_bCoolTimeStarter;
	/*クールタイムの処理
	* CoolTimeCounter(/引数：クールタイム/)
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
		//初期化
		else {
			m_CoolTimeCnt = 0;
			m_bCoolTimeStarter = false;
		}
	}
protected:
	UFUNCTION()
		//オーバーラップ接触し始めた時に呼ばれるイベント関数を登録
		virtual void ActorOnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* m_pSphereComp;	//アイテム接触用コリジョン


};
