//---------------------------------------------------------------------------------------------------------------------
// ファイル名	：GoalBase.cpp
// 概要			：ゴールのベースクラス
// 作成者		：21cu0216 坂本郁樹
// 更新内容		：2022/11/28 作成
//							 ゴールのベースクラスを作成
//---------------------------------------------------------------------------------------------------------------------

//インクルード
#include "GoalBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// コンストラクタ
AGoalBase::AGoalBase()
	:m_state(GOAL_STATE_NONE)
{
	// 毎フレーム、このクラスのTick()を呼ぶかどうかを決めるフラグ
	PrimaryActorTick.bCanEverTick = true;

	//ルートに置くベースオブジェクト生成
	m_pBase = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = m_pBase;

	//コリジョン判定用ボックスコンポーネント生成
	m_pBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	m_pBoxComp->SetupAttachment(RootComponent);

	//メッシュコンポーネント生成(StaticMesh)
	m_pGoalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GoalMesh"));
	m_pGoalMesh->SetupAttachment(RootComponent);
}

// ゲームスタート時、または生成時に呼ばれる処理
void AGoalBase::BeginPlay()
{
	Super::BeginPlay();

	//オーバーラップし始めたときに呼ばれるイベント関数を登録
	if (m_pBoxComp != NULL) {
		m_pBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AGoalBase::OnOverlapBegin);
	}
}

// 毎フレームの更新処理
void AGoalBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//オーバーラップ接触し始めた時の処理
void AGoalBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Overlapした際に実行したいイベント

	//派生クラスで何も書かれていなければ、基本クラスに書いたこちらの関数が呼ばれる
}