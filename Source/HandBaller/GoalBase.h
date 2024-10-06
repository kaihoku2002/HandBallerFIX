//---------------------------------------
// ファイル名	：GoalBase.h
// 概要			：ゴールのベースクラス
// 作成者		：21cu0216 坂本郁樹
// 更新内容		：2022/11/28 作成
// 
//---------------------------------------

//インクルードガード
#pragma once

//インクルード
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalBase.generated.h"

//前方宣言
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class HANDBALLER_API AGoalBase : public AActor
{
	//UE4のオブジェクトクラスで必ず先頭に書くマクロ
	GENERATED_BODY()

protected:
	//ゴールのステート
	enum GOAL_STATE {
		GOAL_STATE_NONE = 0,	//何も起こっていない
		GOAL_STATE_COLLITED,	//接触時
		GOAL_STATE_DESTROY,	//消滅時
		GOAL_STATE_NUM	//ステートの数
	};

public:
	// コンストラクタ
	AGoalBase();

protected:
	// ゲームスタート時、または生成時に呼ばれる処理
	virtual void BeginPlay() override;

public:
	// 毎フレームの更新処理
	virtual void Tick(float DeltaTime) override;

protected:
	//オーバーラップ接触し始めた時に呼ばれるイベント関数を登録
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//エディタ側で編集できるメンバ変数
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBoxComponent* m_pBoxComp;	//アイテム接触用コリジョン

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* m_pGoalMesh;	//ゴールメッシュ

protected:
	UPROPERTY()
		USceneComponent* m_pBase;	//ベースコンポーネント

protected:
	GOAL_STATE m_state;	//ステート管理
};
