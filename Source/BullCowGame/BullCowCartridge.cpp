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

void UBullCowCartridge::SetupGame()
{
    HiddenWord = PickAWord();
    Lives = HiddenWord.Len();
    bGameOver = false;
    bPlayerWon = false;
    ClearScreen();
    History.Empty();
    PrintLine(TEXT("Welcome to Bulls & Cows!"));
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());

   
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
        GetAndShowBullsCows(Input);
    }
    if (--Lives == 0)
    {
        EndGame();
        return;
    }
    PrintLine(TEXT("Wrong! Lives remain: %i"), Lives);
}

bool UBullCowCartridge::PlayerInputIsCorrect(const FString& Input) const
{
    return ((Input.Len() == HiddenWord.Len()) && IsIsogram(Input));
}

void UBullCowCartridge::GetAndShowBullsCows(const FString& Input)
{
    ClearScreen();
    FBullsCows BullsCowsOut;
    GetBullCows(Input, BullsCowsOut);
    AddToHistory(GenerateHystoryLine(Input, BullsCowsOut));
    ShowHistory();
    ShowBullsCows(BullsCowsOut);
}

void UBullCowCartridge::GetBullCows(const FString& Input, FBullsCows& BullsCowsOut) const
{
    for (int i = 0; i < HiddenWord.Len(); i++)
    {
        if (HiddenWord[i] == Input[i])
        {
            BullsCowsOut.Bulls++;
            continue;
        }
        int32 index;
        if (Input.FindChar(HiddenWord[i], index))
        {
            BullsCowsOut.Cows++;
        }
    }
}

void UBullCowCartridge::ShowBullsCows(const FBullsCows& BullsCows) const
{
    PrintLine(TEXT("You have %i Bulls and %i Cows!"), BullsCows.Bulls, BullsCows.Cows);
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

void UBullCowCartridge::ShowHistory() const
{
    PrintLine(TEXT("-----------History-----------"));
    for (FString Line : History)
    {
        PrintLine(TEXT("%s"), *Line);
    }
    PrintLine(TEXT("-----------------------------"));
}

FString UBullCowCartridge::GenerateHystoryLine(const FString& Input, const FBullsCows& BullsCows) const
{
    FString Result = Input + TEXT("   B:") + FString::FromInt(BullsCows.Bulls) + TEXT("  C:") + FString::FromInt(BullsCows.Cows);
    return Result;
}


void UBullCowCartridge::AddToHistory(const FString& HistoryLine)
{
    History.Add(HistoryLine);
}