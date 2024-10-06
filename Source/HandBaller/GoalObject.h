//------------------------------------------------------------------------------
// ファイル名	：GoalObject.h
// 概要			：ゴールオブジェクトクラスヘッダー
// 作成者		：21cu0216 坂本郁樹
// 更新内容		：2022/11/28 作成
// 
//------------------------------------------------------------------------------

//インクルードガード
#pragma once
//インクルード
#include "CoreMinimal.h"
#include "GoalBase.h"
#include "GoalObject.generated.h"

/**
 *
 */
UCLASS()
class HANDBALLER_API AGoalObject : public AGoalBase
{
	GENERATED_BODY()

public:
	//コンストラクタ
	AGoalObject();

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// 毎フレームの更新処理
	virtual void Tick(float DeltaTime) override;

protected:
	//オーバーラップ接触し始めた時に呼ばれるイベント関数を登録
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

private:
	int m_iGoalScore;	//獲得した点数

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool m_bOverlappedGoal;	//ゴールに接触したかのフラグ

	UFUNCTION(BlueprintCallable, Category = "MY Functions")
		int GetGoalScore()const { return m_iGoalScore; }
};
