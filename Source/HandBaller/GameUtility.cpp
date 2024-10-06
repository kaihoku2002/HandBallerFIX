//----------------------------------------------------------
// �t�@�C����		�FGameUtility.cpp
// �T�v				�F�ǂ��ł��Ăяo���鋤�ʋ@�\���܂Ƃ߂����[�e�B���e�B�N���X
// �쐬��			�F21CU0207-�C�k����
// �X�V���e			
//----------------------------------------------------------

//�C���N���[�h
#include "GameUtility.h"
#include "TestWidget.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "Containers/UnrealString.h"
#include "Engine/World.h"
#include "Engine.h"
#include "Kismet/GamePlayStatics.h"

//TestWidget���p������Widget�̌Ăяo��
void CGameUtility::CallTestWidget(AActor* _pOwnerActor, const FString& _path)
{
	//�p�X��񂩂�Widget�̃N���X������
	FStringClassReference MyWidgetClassRef(_path);
	TAssetSubclassOf<class UUserWidget> TestWidgetUIClass =
		MyWidgetClassRef.TryLoadClass<UTestWidget>();

	//Widget�𐶐�
	UUserWidget* pWidget = CreateWidget<UTestWidget>(_pOwnerActor->GetWorld(),
		TestWidgetUIClass.LoadSynchronous());

	//�������s�����ꍇ�A���O�o��
	if (!pWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cant Created Widget [%s]"), *_path);
		return;

	}
	//Widget��ViewPort�ɒǉ�
	pWidget->AddToViewport();
}

//�^�O����Actor���擾
AActor* CGameUtility::GetActorFromTag(AActor* _pOwnerActor, const FName& _tag)
{
	if (_pOwnerActor == NULL) { return NULL; }

	//�Q�[���S�̂ɑ΂���Actor�̌����R�X�g���������߁ABeginPlay�ň��ۑ����Ă��������ɂ���
	//�����Ώۂ͑S�Ă�Actor
	TSubclassOf<AActor> findClass;
	findClass = AActor::StaticClass();
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(_pOwnerActor->GetWorld(), findClass, actors);

	//�������ʁAActor�������
	if (actors.Num() > 0)
	{
		//����Actor�̒������ԂɌ���
		for (int idx = 0; idx < actors.Num(); idx++)
		{
			AActor* pActor = Cast<AActor>(actors[idx]);

			//�^�O���Ŕ��f����
			//���O�̓G�f�B�^��ŏd�������ꍇ�͓����̖��O���ς�邽�߁A���ĂɂȂ�Ȃ�
			if (pActor->ActorHasTag(_tag))
			{
				//�f�o�b�O�m�F
				/*FString message = FString("Founded Actor	:") + pActor->GetName();
				UE_LOG(LogTemp, Warning, TEXT("%s"), *message);*/

				return pActor;

			}
		}
	}
	return NULL;
}