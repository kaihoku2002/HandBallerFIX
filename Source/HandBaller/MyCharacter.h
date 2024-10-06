// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Human.h"
#include "MyCharacter.generated.h"

/**
 * 
 */
 //前方宣言
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

	virtual void Shoot();
	//ジャンプ処理
	virtual void UpdateJump(float DeltaTime);

	//入力バインド タックル
	virtual void TackleStart();

	virtual void TurnAtRate(float Rate);

private:
	//移動処理
	virtual void UpdateMove(float DeltaTime);
	//カメラの更新処理
	virtual void UpdateCamera(float DeltaTime);

	////タックル処理
	//virtual void UpdateTackle();

public:
	//------------------ボール処理-------------------------
	//

private:
	//UPRPPERTYにすることで、ブループリント上で変数の確認、編集などができる
//[BuleprintReadOnly]に指定しているため、ブループリントで見ることだけが可能で編集はできない
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera, meta =
		(AllowPrivateAccess = "true"))
		USpringArmComponent* m_pSpringArm;		//スプリングアーム

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera, meta =
		(AllowPrivateAccess = "true"))
		UCameraComponent* m_pCamera;		//カメラ

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = camera, meta =
	//	(AllowPrivateAccess = "true"))
	//	UCameraComponent* Camera_Shoot;		//カメラ(シュート時の一人称)

	UPROPERTY(EditAnywhere, Category = "Camera")
		FVector2D m_cameraPitchLimit;		//カメラのピッチ範囲

	FVector2D m_charaMoveInput;		//Pawn移動入力量
	FVector2D m_cameraRotateInput;		//Pawn移動入力量

	bool m_bCanControl;		//操作可能な状態か？

protected:
	//オーバーラップ接触し始めた時に呼ばれるイベント関数を登録
	void ActorOnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool m_bOverlappedCPU;	//CPUとOverlapしているかどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AActor* m_Overlaped_CPU;

};
