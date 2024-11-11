// Fill out your copyright notice in the Description page of Project Settings.


// インクルード
#include "Ball.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameUtility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

// コンストラクタ
ABall::ABall()
	: m_pSphereComp(NULL)
	, m_pStaticMeshComp(NULL)
	, m_pBase(NULL)
	, m_pStaticMesh(NULL)
	, m_meshSize(FVector::ZeroVector)
	, m_firstPos(0.0, 0.0, 0.0)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ。必要に応じて、パフォーマンス向上のために切ることのできる。
	PrimaryActorTick.bCanEverTick = true;

	// ベースオブジェクトを生成
	//m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("BaseComponent"));
	//RootComponent = m_pBase;
	

	// スタティックメッシュコンポーネントを生成
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = m_pStaticMeshComp;


	// コリジョン判定用ボックスコンポーネントを生成
	m_pSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SpherenComp"));
	m_pSphereComp->SetupAttachment(RootComponent);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	// タグの設定
	Tags.Add(FName("Ball"));

}

// ゲームスタート時、または生成時に呼ばれる処理
void ABall::BeginPlay()
{
	Super::BeginPlay();

}

// 毎フレームの更新処理
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


