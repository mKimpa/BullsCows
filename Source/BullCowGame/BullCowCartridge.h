// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	// Your declarations go below!
	void SetupGame();

	
	
	private:
	TArray<FString> Words;
	FString HiddenWord;
	bool bGameOver;
	bool bPlayerWon;

	UPROPERTY(EditAnywhere)
		int32 Lives = 4;

	UPROPERTY(EditAnywhere)
		int32 Difficulty = 1;

	bool PlayerInputIsCorrect(const FString& Input) const;
	bool IsIsogram(const FString& Input) const;
	void ProcessGuess(const FString& Input);
	void EndGame();

	TArray<FString> GetWordList();
	int GenerateRaandomNumber(size_t Size);
	FString PickAWord();

	

		
	
};
