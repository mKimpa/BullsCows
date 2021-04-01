// Fill out your copyright notice in the Description page of Project Settings.
#include <random>
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    GetWordList();
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
    HiddenWord = Words[];
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

        return;
    }
    if (--Lives == 0)
    {
        EndGame();
        return;
    }
    PrintLine(TEXT("You have Entered incorrect Word! Lives remain: %i"), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString& Input) const
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

TArray<FString> UBullCowCartridge::GetWordList()
{
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWords.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    FFileHelper::
}

int UBullCowCartridge::GenerateRaandomNumber(size_t Size)
{
    std::uniform_int_distribution<int> dist(0, Size - 1);
    return dist(gen);
}

FString UBullCowCartridge::PickAWord()
{
    Words[]
}

