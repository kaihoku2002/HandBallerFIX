// Fill out your copyright notice in the Description page of Project Settings.


#include "Human.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameUtility.h"

#define JUMP_HEIGHT (m_jumpTime * m_jumpTime * (-m_gravity) / 2 + (m_jumpTime * m_jumpPower))


// Sets default values
AHuman::AHuman()
	: m_jumpPower(750.f)
	, m_gravity(980.f)
	, m_bCanControl(true)
	, m_jumpTime(0.f)
	, m_nowJumpHeight(0.f)
	, m_prevJumpHeight(0.f)
	, m_bJumping(false)
	, m_charaMoveInput(FVector2D::ZeroVector)
	, m_cameraRotateInput(FVector2D::ZeroVector)
	, m_moveSpeed(10000.0f)
	, m_bIsHoleding(false)
	, m_TackleSpeed(10000000.f)
	, m_bTackle(false)
	, m_bCanTackle(true)
	, m_iTime(0)
	, m_iTimeCnt(0)
	, m_iTackleTime(5)

{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//コリジョン判定用コンポーネント生成
	m_pSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	m_pSphereComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AHuman::BeginPlay()
{
	Super::BeginPlay();
	
	//オーバーラップし始めたときに呼ばれるイベント関数を登録
	if (m_pSphereComp != NULL) {
		m_pSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AHuman::ActorOnOverlapBeginComponent);
	}

}

// Called every frame
void AHuman::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//移動処理
	UpdateMove(DeltaTime);

	//ジャンプ処理
	UpdateJump(DeltaTime);

}

// Called to bind functionality to input
void AHuman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AHuman::UpdateCamera(float DeltaTime)
{

}

void AHuman::UpdateMove(float DeltaTime)
{

}

void AHuman::UpdateJump(float _deltaTime)
{

}

//入力バインド カメラ回転：Pitch(Y軸)
void AHuman::Camera_RotatePitch(float _axisValue)
{

}

//入力バインド カメラ回転：Yaw(Z軸)
void AHuman::Camera_RotateYaw(float _axisValue)
{
}

//入力バインド キャラ移動：前後
void AHuman::Chara_MoveForward(float _axisValue)
{
}

//入力バインド キャラ移動：左右
void AHuman::Chara_MoveRight(float _axisValue)
{
}

//入力バインド　ジャンプ
void AHuman::JumpStart()
{

}

//入力バインド　シュート
void AHuman::Shoot()
{

}

//入力バインド　タックル
void AHuman::TackleStart()
{

}

//入力バインド　シュート
void AHuman::TurnAtRate(float Rate)
{

}

//タックル処理
void AHuman::UpdateTackle()
{
	if (m_bTackle != false)
	{
		//一定の時間が経過するまでタックル不可
		if (m_iTime < m_iTackleTime) {
			//毎フレーム加算
			m_iTimeCnt++;
			if (m_iTimeCnt % 60 == 0) {
				m_iTimeCnt = 0;
				m_iTime++;

				//AddForce(FVecter Force, FName BoneName, Bool bAccelChange)
				//AddForce();
			}
		}
		else {
			//値を初期化
			m_iTimeCnt = 0;
			m_iTime = 0;
			m_bCanTackle = true;
			m_bTackle = false;
		}
	}
}

//オーバーラップ接触し始めた時の処理
void AHuman::ActorOnOverlapBeginComponent(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Overlapした際に実行したいイベント

	//派生クラスで何も書かれていなければ、基本クラスに書いたこちらの関数が呼ばれる
}