// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"


struct FBullsCows {
	
	int32 Bulls = 0;
	int32 Cows = 0;
};

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
	TArray<FString> History;

	UPROPERTY(EditAnywhere)
		int32 Lives = 4;

	UPROPERTY(EditAnywhere)
		int32 Difficulty = 1;

	bool PlayerInputIsCorrect(const FString& Input) const;
	static bool IsIsogram(const FString& Input);
	void ProcessGuess(const FString& Input);
	void GetAndShowBullsCows(const FString& Input);
	void EndGame();
	void ShowBullsCows(const FBullsCows& BullsCows) const;
	void GetBullCows(const FString& Input, FBullsCows& BullsCowsOut) const;
	void ShowHistory() const;
	FString GenerateHystoryLine(const FString& Input, const FBullsCows& BullsCows) const;
	void AddToHistory(const FString& HistoryLine);


	TArray<FString> GetWordList(int32 minWordLength, int32 maxWordLength);
	int32 GenerateRandomNumber(int32 Size) const;
	FString PickAWord();
};
