//----------------------------------------------------------
// ファイル名		：GameUtility.cpp
// 概要				：どこでも呼び出せる共通機構をまとめたユーティリティクラス
// 作成者			：21CU0207-海北憲吾
// 更新内容			
//----------------------------------------------------------

//インクルード
#include "GameUtility.h"
#include "TestWidget.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Kismet/GamePlayStatics.h"

//TestWidgetを継承するWidgetの呼び出し
void CGameUtility::CallTestWidget(AActor* _pOwnerActor, const FString& _path)
{
	//パス情報からWidgetのクラスを試す
	FStringClassReference MyWidgetClassRef(_path);
	TAssetSubclassOf<class UUserWidget> TestWidgetUIClass =
		MyWidgetClassRef.TryLoadClass<UTestWidget>();

	//Widgetを生成
	UUserWidget* pWidget = CreateWidget<UTestWidget>(_pOwnerActor->GetWorld(),
		TestWidgetUIClass.LoadSynchronous());

	//生成失敗した場合、ログ出力
	if (!pWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant Created Widget [%s]"), *_path);
		return;

	}
	//WidgetをViewPortに追加
	pWidget->AddToViewport();
}

//タグからActorを取得
AActor* CGameUtility::GetActorFromTag(AActor* _pOwnerActor, const FName& _tag)
{
	if (_pOwnerActor == NULL) { return NULL; }

	//ゲーム全体に対するActorの検索コストが高いため、BeginPlayで一回保存しておくだけにする
	//検索対象は全てのActor
	TSubclassOf<AActor> findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(_pOwnerActor->GetWorld(), findClass, actors);

	//検索結果、Actorがあれば
	if (actors.Num() > 0)
	{
		//そのActorの中を順番に検索
		for (int idx = 0; idx < actors.Num(); idx++)
		{
			AActor* pActor = Cast<AActor>(actors[idx]);

			//タグ名で判断する
			//名前はエディタ上で重複した場合は内部の名前が変わるため、あてにならない
			if (pActor->ActorHasTag(_tag))
			{
				//デバッグ確認
				/*FString message = FString("Founded Actor	:") + pActor->GetName();
				UE_LOG(LogTemp, Warning, TEXT("%s"), *message);*/

				return pActor;

			}
		}
	}
	return NULL;
}