//----------------------------------------------------------
// �t�@�C����		�FGameUtility.h
// �T�v				�F�ǂ��ł��Ăяo���鋤�ʋ@�\���܂Ƃ߂����[�e�B���e�B�N���X
// �쐬��			�F21CU0207-�C�k����
// �X�V���e			
//----------------------------------------------------------

#pragma once

//�O���錾
class FString;
class UTestWidget;
class AActor;

class CGameUtility
{
	//���[�e�B���e�B�N���X�̂��߂́A�����o�֐���public��
public:
	//TestWidget���p������Widget�̌Ăяo��
	static void CallTestWidget(AActor* _pOwnerActor, const FString& _path);

	//�^�O����Actor���擾
	static AActor* GetActorFromTag(AActor* _pOwnerActor, const FName& _tag);
};
