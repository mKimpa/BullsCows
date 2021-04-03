// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"



void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (!bGameOver)
    {
        ProcessGuess(Input);
    }
    else
    {
        SetupGame();     
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = PickAWord();
    Lives = HiddenWord.Len();
    bGameOver = false;
    bPlayerWon = false;
    ClearScreen();
    PrintLine(TEXT("Welcome to Bulls & Cows!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());

    const TCHAR WE[] = TEXT("cake");
}

bool UBullCowCartridge::PlayerInputIsCorrect(const FString& Input) const
{
    return ((Input.Len() == HiddenWord.Len()) && IsIsogram(Input));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    if (bPlayerWon)
    {
        PrintLine(TEXT("Congratulations! You have Won! The Answer was: %s"), *HiddenWord);
    }
    else
    {
        PrintLine(TEXT("You have Lost! The Answer was: %s"), *HiddenWord);
    }
    PrintLine(TEXT("Press 'Enter' to play again"));
}

void UBullCowCartridge::ShowBullsCows(TPair<int32, int32> BullsCows) const
{
    PrintLine(TEXT("You have %i Bulls and %i Cows!"), BullsCows.Key, BullsCows.Value);
}

TPair<int32, int32> UBullCowCartridge::GetBullCows(const FString& Input) const
{
    TPair<int32, int32> BullsCows{ 0,0 };
    for (int i = 0; i < HiddenWord.Len(); i++)
    {
        if (HiddenWord[i] == Input[i])
        {
            BullsCows.Key++;
            continue;
        }
        if (Input.Contains(&HiddenWord[i]))
        {
            BullsCows.Value++;
        }
    }
    return BullsCows;
}

void UBullCowCartridge::ProcessGuess(const FString& Input)
{
    if (Input == HiddenWord)
    {
        bPlayerWon = true;
        EndGame();
        return;
    }
    if (PlayerInputIsCorrect(Input))
    {
        ShowBullsCows(GetBullCows(Input));        
    }
    if (--Lives == 0)
    {
        EndGame();
        return;
    }
    PrintLine(TEXT("You have Entered incorrect Word! Lives remain: %i"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Input)
{
    for (int32 i = 0; i < Input.Len(); i++)
    {
        for (int32 j = i + 1; j < Input.Len(); j++)
        {
            if (Input[i] == Input[j])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetWordList(int32 minWordLength, int32 maxWordLength)
{
    TArray<FString> List;
    TArray<FString> Isograms;
    FString  WordListPath = FPaths::ProjectContentDir() / TEXT("/WordLists/HiddenWords.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(
        Isograms,
        *WordListPath,
        [&minWordLength, &maxWordLength](const FString& Word) 
        {return (IsIsogram(Word) && minWordLength <= Word.Len() && Word.Len() <= maxWordLength); });
    return Isograms;
}

int32 UBullCowCartridge::GenerateRandomNumber(int32 Size) const
{
    return FMath::RandRange(0, Size - 1);
}

FString UBullCowCartridge::PickAWord()
{
    TArray<FString> Words = GetWordList(Difficulty + 2, Difficulty + 3);
    int32 WordIndex = GenerateRandomNumber(Words.Num());
    return Words[WordIndex];
}

