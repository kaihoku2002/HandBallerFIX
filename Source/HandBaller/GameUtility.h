//----------------------------------------------------------
// ファイル名		：GameUtility.h
// 概要				：どこでも呼び出せる共通機構をまとめたユーティリティクラス
// 作成者			：21CU0207-海北憲吾
// 更新内容			
//----------------------------------------------------------

#pragma once

//前方宣言
class FString;
class UTestWidget;
class AActor;

class CGameUtility
{
	//ユーティリティクラスのための、メンバ関数はpublicに
public:
	//TestWidgetを継承するWidgetの呼び出し
	static void CallTestWidget(AActor* _pOwnerActor, const FString& _path);

	//タグからActorを取得
	static AActor* GetActorFromTag(AActor* _pOwnerActor, const FName& _tag);
};
