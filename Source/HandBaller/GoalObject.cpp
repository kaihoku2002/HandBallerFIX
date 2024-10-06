//---------------------------------------------------------------------------------------------------------------------
// ファイル名	：GoalObject.cpp
// 概要			：ゴールオブジェクトクラス
// 作成者		：21cu0216 坂本郁樹
// 更新内容		：2022/11/28 作成
//							 ゴールオブジェクトを作成
//				　2022/11/29 ボールがゴールオブジェクトに触れると、1点追加するように修正
//---------------------------------------------------------------------------------------------------------------------

//インクルード
#include "GoalObject.h"

//コンストラクタ
AGoalObject::AGoalObject()
	:m_bOverlappedGoal(false)
	, m_iGoalScore(0)
{
}

// ゲームスタート時、または生成時に呼ばれる処理
void AGoalObject::BeginPlay()
{
	Super::BeginPlay();

}

// 毎フレームの更新処理
void AGoalObject::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	//接触フラグが立ったら
	if (m_bOverlappedGoal) {
		//１点追加
		++m_iGoalScore;

		//接触フラグは下げておく
		m_bOverlappedGoal = false;
	}
}

//オーバーラップ接触し始めた時に呼ばれるイベント関数を登録
void AGoalObject::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//オーバーラップした際に実行したいイベント
	if (OtherActor && (OtherActor != this) && OtherComp) {
		//オーバーラップしたのがボールのときのみ反応させたい
		if (OtherActor->ActorHasTag("Ball")) {
			//接触フラグを立てる
			m_bOverlappedGoal = true;
		}
	}
}