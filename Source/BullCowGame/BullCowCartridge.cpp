// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include <random>
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

TArray<FString> UBullCowCartridge::GetWordList(int32 minWordLength, int32 maxWordLength)
{
    int32 LowerBound = minWordLength;
    int32 UpperBound = maxWordLength;
    TArray<FString> List;
    TArray<FString> Isograms;
    FString  WordListPath = FPaths::ProjectContentDir() / TEXT("/WordLists/HiddenWords.txt");
    FFileHelper::LoadFileToStringArray(List, *WordListPath); 
    for (const auto& Word : List)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s "), *Word);
        //transform to lowercase and make sure it's an isogram
        if (IsIsogram(Word) && LowerBound <= Word.Len() && Word.Len() <= UpperBound)
        {
            Isograms.Add(Word);
        }       
    }
    return Isograms;
}

int UBullCowCartridge::GenerateRandomNumber(int32 Size) const
{
    std::random_device rd;
    std::default_random_engine gen{ rd() };
    std::uniform_int_distribution<int> dist(0, Size - 1);
    return dist(gen);
}

FString UBullCowCartridge::PickAWord()
{
    TArray<FString> Words = GetWordList(Difficulty + 2, Difficulty + 3);
    int32 WordIndex = GenerateRandomNumber(Words.Num());
    return Words[WordIndex];
}

