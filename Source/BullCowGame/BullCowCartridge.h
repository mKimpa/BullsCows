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
	FString HiddenWord;

	UPROPERTY(EditAnywhere)
	int Lives = 4;

	UPROPERTY(EditAnywhere)
	int Difficulty = 4;
	
	private:

	bool PlayerInputIsCorrect(const FString& Input);
	void WrongInputHandler(const FString& Input);
	void AskForGuess() const;
	void GreetingsMessage() const;
	void GenerateHiddenWord();
	int WordLength;

		
	
};
