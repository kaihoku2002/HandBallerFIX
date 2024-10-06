// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Human.h"
#include "CPUCharacter.generated.h"

/**
 * 
 */
UCLASS()
class HANDBALLER_API ACPUCharacter : public AHuman
{
	GENERATED_BODY()
	
};

UENUM(BlueprintType)
enum class ECPUState :uint8 {
	CPUS_None UMETA(DisplayName = "None"),
	CPUS_Chase UMETA(DisplayName = "doChase"),
	CPUS_Have UMETA(DisplayName = "doHave"),
	CPUS_Go UMETA(DisplayName = "doGo"),
	CPUS_Throw UMETA(DisplayName = "doThrouw"),
	CPUS_Tackle UMETA(DisplayName = "doTackle"),
	CPUS_Pass UMETA(DisplayName = "doPass"),
};
