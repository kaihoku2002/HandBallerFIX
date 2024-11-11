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


	// エディタ側で編集できるメンバ変数
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USphereComponent* m_pSphereComp;				// 接触用コリジョン

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pStaticMeshComp;	// スタティックメッシュコンポ―ネント

	// Projectile movement component.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector m_firstPos;				// 接触用コリジョン



	UPROPERTY()
		USceneComponent* m_pBase;				// ベースコンポーネント

private:
	UStaticMesh* m_pStaticMesh;					// スタティックメッシュ
	FVector m_meshSize;							// スタティックメッシュのサイズ
};
