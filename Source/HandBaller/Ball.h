// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ball.generated.h"

// 前方宣言
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
	// コンストラクタ
	ABall();

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// 毎フレームの更新処理
	virtual void Tick(float DeltaTime) override;

private:
	// 持ち上げられるオブジェクト共通の更新
	void UpdateLiftObject(float _deltaTime);

public:
	// 持ち上げ状態の設定
	void SetIsLift(bool _isLift);

	// 持ち上げ状態フラグの取得
	bool GetIsLift() const { return m_isLift; }

	// エディタ側で編集できるメンバ変数
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* m_pSphereComp;				// 接触用コリジョン

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pStaticMeshComp;	// スタティックメッシュコンポ―ネント

	// Projectile movement component.
	UPROPERTY(BlueprintReadWrite, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	// 派生クラスで使えるようにprotectedに設定する変数

	UPROPERTY()
		USceneComponent* m_pBase;				// ベースコンポーネント
protected:
	UPROPERTY()
		float m_liftHeightRate;					// 持ち上げた時の高さ比率

private:
	UStaticMesh* m_pStaticMesh;					// スタティックメッシュ
	AActor* m_pPlayerChara;						// プレイヤーの情報（基本クラスポインタ）
	FVector m_meshSize;							// スタティックメッシュのサイズ

	bool m_isLift;								// 持ち上げ状態フラグ
};
