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
#include "GameFramework/Character.h"

// コンストラクタ
ABall::ABall()
	: m_pSphereComp(NULL)
	, m_pStaticMeshComp(NULL)
	, m_pBase(NULL)
	, m_liftHeightRate(0.25f)
	, m_pStaticMesh(NULL)
	, m_pPlayerChara(NULL)
	, m_meshSize(FVector::ZeroVector)
	, m_isLift(false)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ。必要に応じて、パフォーマンス向上のために切ることのできる。
	PrimaryActorTick.bCanEverTick = true;

	// ベースオブジェクトを生成
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("BaseComponent"));
	RootComponent = m_pBase;


	// コリジョン判定用ボックスコンポーネントを生成
	m_pSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SpherenComp"));
	m_pSphereComp->SetupAttachment(RootComponent);

	// スタティックメッシュコンポーネントを生成
	m_pStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	m_pStaticMeshComp->SetupAttachment(RootComponent);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(m_pStaticMeshComp);


}

// ゲームスタート時、または生成時に呼ばれる処理
void ABall::BeginPlay()
{
	Super::BeginPlay();

	if (m_pStaticMeshComp != NULL)
	{
		// メッシュの中身を取得
		m_pStaticMesh = m_pStaticMeshComp->GetStaticMesh();
	}

	// メッシュの中身が無効であれば進める事はできない
	if (m_pStaticMesh != NULL)
	{
		// メッシュのバウンティボックスサイズを求める
		m_meshSize = m_pStaticMesh->GetBounds().GetBox().GetSize();

		// オブジェクトActorのサイズを加味する
		m_meshSize *= GetActorScale();
	}

	// Tick()処理の実行状態を無効にしておく
	PrimaryActorTick.SetTickFunctionEnable(false);
	ProjectileMovementComponent->SetActive(false, false);
}

// 毎フレームの更新処理
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//// 持ち上げられるオブジェクト共通の更新
	//UpdateLiftObject(DeltaTime);
}

// 持ち上げられるオブジェクト共通の更新
void ABall::UpdateLiftObject(float _deltaTime)
{
	// プレイヤーのポインタを、最初の一回だけ取得（検索処理のコストが高いため）
	if (m_pPlayerChara == NULL)
	{
		// Utilityクラスを使う
		m_pPlayerChara = CGameUtility::GetActorFromTag(this, "Player");

		// ゲーム全体に対するActorの検索処理はコストが高いため、BeginPlayで一回保存にしておくだけにする
		// 検索対象は全てのActor
		/*TSubclassOf<AActor> findClass;
		findClass = AActor::StaticClass();
		TArray<AActor*> actors;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), findClass, actors);*/

		// 検索結果、Actorがあれば
		//if (actors.Num() > 0)
		//{
		//	// そのActorの中を順番に検索
		//	for (int idx = 0; idx < actors.Num(); idx++)
		//	{
		//		AActor* pActor = Cast<AActor>(actors[idx]);

		//		// タグ名で判別する
		//		// （名前はエディタ上で重複した場合は内部の名前が変わるため、宛てにならない）
		//		if (pActor->ActorHasTag("Player"))
		//		{
		//			////デバッグ確認用
		//			// FString message = FString("Founded PlayerChara :") + pActor->GetName();
		//			// UE_LOG(LogTemp, Warning, TEXT("%s"), *message);

		//			m_pPlayerChara = pActor;
		//			break;
		//		}
		//	}
		//}
	}

	// プレイヤーの座標を追従して動かす
	if (m_pPlayerChara != NULL)
	{
		// プレイヤーのメッシュ(SkeletalMesh)を取得
		// APlayerCharaクラスの基本クラス（ACharacter）もメッシュを持っているため、
		// ACharacter型にキャストしてGetMesh()を呼ぶことができる
		USkeletalMeshComponent* pPlayerMesh = Cast<ACharacter>(m_pPlayerChara)->GetMesh();

		// プレイヤーの座標・回転取得
		FVector playerCharaLocation = pPlayerMesh->GetComponentLocation();
		FRotator playerCharaRotation = pPlayerMesh->GetComponentRotation();

		// プレイヤーのメッシュサイズ
		FVector playerMeshSize = pPlayerMesh->Bounds.GetBox().GetSize() * m_pPlayerChara->GetActorScale();

		// オブジェクトを持ち上げた時の座標を計算
		// プレイヤーの前に、オブジェクトのサイズを考慮した位置に配置したい
		FVector newLocation = playerCharaLocation + pPlayerMesh->GetForwardVector() * (m_meshSize + playerMeshSize) / 2;

		// 持ち上げた時だけ、プレイヤーメッシュの高さを参照して、少し上にあげたい
		if (m_isLift == true)
		{
			newLocation += FVector(0, 0, playerMeshSize.Z * m_liftHeightRate);
		}

		// オブジェクトの座標・回転を設定
		SetActorLocation(FVector(newLocation));
		SetActorRotation(FRotator(playerCharaRotation));

		// このオブジェクトのコリジョン判定を切り替える
		// 持ち上げた時は無効に、降ろすときは有効に
		this->SetActorEnableCollision(m_isLift ? false : true);
	}

	// 降ろすとき、オブジェクトのTick()処理の実行状態を無効にする
	if (m_isLift == false)
	{
		PrimaryActorTick.SetTickFunctionEnable(false);
	}

}

// 持ち上げ状態の設定
void ABall::SetIsLift(bool _isLift)
{
	m_isLift = _isLift;

	// 持ち上げる時、オブジェクトのTcik()処理の実行状態を有効にする
	if (m_isLift == true)
	{
		PrimaryActorTick.SetTickFunctionEnable(true);
	}
}