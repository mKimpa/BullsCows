// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BCSaveGame.h"
#include "BullCowCartridge.generated.h"


struct FBullsCows
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

enum UGameState 
{
	Wellcome = 0,
	MainMenu = 1,
	SelectDifficulty = 2,
	Play = 3,
	InputName = 4,
	HighScore = 5
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

	UPROPERTY(EditAnywhere)
	int32 HighScoreLength = 5;

	
	
	private:
	TArray<FString> Words;
	FString HiddenWord;
	bool bPlayerWon;
	bool bGameOver;
	TArray<FString> History;
	UGameState GameState = UGameState::Wellcome;
	TArray<FPlayerScore> HighScores;
	int32 Score;


	int32 Lives;
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
	//void PlayGame(const FString& Input);
	void UpdateHighScores(const FString& Name, const int32& Score);
	void LoadHighScores();
	void SaveHighScores();
	void AddPlayerScoreToHighScores(const FString& Name, const int32& Score, int32 index);
	
	// Interface
	void UpdateScreen() const;
	void ShowWellcomeMessage() const;
	void ShowMainMenu() const;
	void ShowSelectDifficultyMenu() const;
	void ShowPlayScreen() const;
	void ShowNameInputScreen() const;
	void ShowHighScore() const;
 

	TArray<FString> GetWordList(int32 minWordLength, int32 maxWordLength);
	int32 GenerateRandomNumber(int32 Size) const;
	FString PickAWord();
};
