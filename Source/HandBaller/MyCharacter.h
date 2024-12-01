// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Human.h"
#include "Ball.h"
#include "MyCharacter.generated.h"

/**
 * 
 */
 //前方宣言
class USpringArmComponent;
class UCameraComponent;
class UAnimMontage;

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
	//入力バインド カメラ回転：Pitch[Y軸]
	virtual void Camera_RotatePitch(const float _axisValue);
	//入力バインド カメラ回転：Yaw[X軸]
	virtual void Camera_RotateYaw(const float _axisValue);

	//入力バインド キャラ移動：前後
	virtual void Chara_MoveForward(const float _axisValue);
	//入力バインド キャラ移動：左右
	virtual void Chara_MoveRight(const float _axisValue);

	//入力バインド ジャンプ
	virtual void JumpStart();

	//入力バインド 投げる
	virtual void Throw();
	//ジャンプ処理
	virtual void UpdateJump(float DeltaTime);

	//入力バインド タックル
	virtual void TackleStart();


private:

	//移動処理
	virtual void UpdateMove(float DeltaTime);
	//カメラの更新処理
	virtual void UpdateCamera(float DeltaTime);

	////タックル処理
	//virtual void UpdateTackle();

	//前回と現在のZ座標を設定する
	void SetBeforeNowLocationZ();


	//前回と現在のZ座標を比較する
	void CompareLocationZ();
	//坂を下る時間を管理する
	void ManageDownhill();

	// 移動処理用ベクトル
	FVector m_moveVec;		// 移動用ベクトル



public:
	//------------------ボール処理-------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ball")
	class UAnimMontage* ThrowBallMontage;


	UFUNCTION(BlueprintCallable, Category = "Ball")
	void ThrowBall();

	UFUNCTION(BlueprintCallable, Category = "Ball")
	void ChangeBallFlag();



	//------------------ボール処理-------------------------

private:
	//UPRPPERTYにすることで、ブループリント上で変数の確認、編集などができる
//[BuleprintReadOnly]に指定しているため、ブループリントで見ることだけが可能で編集はできない
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera, meta =
		(AllowPrivateAccess = "true"))
		USpringArmComponent* m_pSpringArm;		//スプリングアーム

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera, meta =
		(AllowPrivateAccess = "true"))
		UCameraComponent* m_pCamera;		//カメラ



	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector2D m_cameraPitchLimit;		//カメラのピッチ範囲

	FVector2D m_charaMoveInput;		//Pawn移動入力量
	FVector2D m_cameraRotateInput;		//Pawn移動入力量




	//------------------坂を下る処理-------------------------

	bool m_bGetBeforeLocationZ;		//前回取得したZ座標を取得しているか
	bool m_bGetNowLocationZ;		//現在のZ座標を取得しているか
	bool m_bShouldDownhill;			//坂を下るべきか

	int	m_NowDownhillTime;			//坂を下っている時間
	int m_WaitGetLoacationTime;		//座標を取得するのに待っている時間

	float m_BeforeLocationZ;		//前回取得したZ座標
	float m_NowLocationZ;			//現在の取得したZ座標

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Downhill")
		class UAnimMontage* DownhillMontage;

	//------------------坂を下る処理-------------------------


protected:
	//オーバーラップ接触し始めた時に呼ばれるイベント関数を登録
	void ActorOnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bOverlappedCPU;	//CPUとOverlapしているかどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* m_Overlaped_CPU;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ABall* m_pBall;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "vector")
	//FVector fVector;				


protected:
	// プレイヤーの定数--------------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Player")
		float GRAVITY;				// 重力
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Player")
		float FALLSPEED_MAX;		// Z方向の最大速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Player")
		float MOVESPEED_MAX;		// X-Y方向の最大移動速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Player")
		float THROW_POWER;				// 投げる力


	// 移動						
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_INVALIDRANGE;		// 入力の無効範囲
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_SPEED_MAX;		// 移動速度(最大)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_SPEED_MIDDLE;		// 移動速度(第二段階)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_SPEED_MIN;		// 移動速度(第一段階)	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_SPEED_FALLING;	// 移動速度(空中時)	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_SPEED_DOWNHILL;	// 坂を下っている時の移動速度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_ROTATE_MAX;		// 回転速度(空中時)	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_ROTATE_MIDDLE;	// 回転速度(空中時)	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_ROTATE_MIN;		// 回転速度(空中時)	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Move")
		float MOVE_BALL_HOLDING_RASIO;	// ボールを保持時の移動速度倍率


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Time")
		int GET_LOCATION_COOLTIME;	// 座標を取得するクールタイム	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConstantData|Time")
		int DOWNHILL_POSSIBLE_TIME;	// 坂を下ることができる時間
};
