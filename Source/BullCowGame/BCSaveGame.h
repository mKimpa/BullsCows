// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BCSaveGame.generated.h"


USTRUCT() struct FPlayerScore
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FString Name;
	UPROPERTY(VisibleAnywhere)
	int32 Score = 0;
};
/**
 * 
 */
UCLASS()
class BULLCOWGAME_API UBCSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UBCSaveGame();

	UPROPERTY(VisibleAnywhere)
	TArray<FPlayerScore> HighScore;
};
