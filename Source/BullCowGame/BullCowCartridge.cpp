// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/FileHelper.h"
//#include "BCSaveGame.h"
#include "Misc/Paths.h"



void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    LoadHighScores();
    UpdateScreen();
}

void UBullCowCartridge::SetupGame()
{
    if (bGameOver)
    {
        Score = 0;
    }
    HiddenWord = PickAWord();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, *HiddenWord);
    Lives = HiddenWord.Len();
    bPlayerWon = false;
    bGameOver = false; 
    ClearScreen();
    History.Empty();   
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    switch (GameState) 
    {
    case UGameState::Wellcome:
        GameState = UGameState::MainMenu;
        UpdateScreen();
        break;
    case UGameState::MainMenu:
        if (Input == "1")
        {
            GameState = UGameState::Play;
            SetupGame();
            UpdateScreen();
        }
        else if (Input == "2")
        {
            GameState = UGameState::SelectDifficulty;
            UpdateScreen();
        }
        else if (Input == "3")
        {
            GameState = UGameState::HighScore;
            UpdateScreen();
        }
        else if (Input == "4")
        {
            exit(0);
        }
        else
        {
            UpdateScreen();
            PrintLine(TEXT("Incorrect input!"));
            PrintLine(TEXT("Please select right menu option!"));
        }
        break;
    case UGameState::SelectDifficulty:
        if (Input == "1")
        {
            GameState = UGameState::MainMenu;
            Difficulty = 1;
            UpdateScreen();
        }
        else if (Input == "2")
        {
            GameState = UGameState::MainMenu;
            Difficulty = 2;
            UpdateScreen();
        }
        else if (Input == "3")
        {
            GameState = UGameState::MainMenu;
            Difficulty = 3;
            UpdateScreen();
        }
        else
        {
            UpdateScreen();
            PrintLine(TEXT("Incorrect input!"));
            PrintLine(TEXT("Please select right menu option!"));
        }
        break;
    case UGameState::Play:
        ProcessGuess(Input);
        break;
    case UGameState::InputName:
        UpdateHighScores(Input, Score);
        SaveHighScores();
        GameState = UGameState::HighScore;
        UpdateScreen();
        break;
    case UGameState::HighScore:
        
        GameState = UGameState::MainMenu;
        UpdateScreen();
    }    
}


void UBullCowCartridge::ProcessGuess(const FString& Input)
{
    if (bPlayerWon)
    {
        SetupGame();
        UpdateScreen();
    }
    if (bGameOver)
    {
        GameState = UGameState::InputName;
        UpdateScreen();
        return;
    }
    if (Input == HiddenWord)
    {
        bPlayerWon = true;
        Score += Lives * HiddenWord.Len();
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
    ClearScreen();
    if (bPlayerWon)
    {
        PrintLine(TEXT("Congratulations! You guessed the word!"));
        PrintLine(TEXT("The Answer was : % s"), *HiddenWord);
    }
    else
    {
        PrintLine(TEXT("You have Lost! The Answer was: %s"), *HiddenWord);
        bGameOver = true;
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

void UBullCowCartridge::UpdateScreen() const
{
    switch (GameState)
    {
    case UGameState::Wellcome:
        ShowWellcomeMessage();
        break;
    case UGameState::MainMenu:
        ShowMainMenu();
        break;
    case UGameState::SelectDifficulty:
        ShowSelectDifficultyMenu();
        break;
    case UGameState::Play:
        ShowPlayScreen();
        break;
    case UGameState::InputName:
        ShowNameInputScreen();
        break;
    case UGameState::HighScore:
        ShowHighScore();
    }
}

void UBullCowCartridge::ShowWellcomeMessage() const
{
    PrintLine(TEXT("Welcome to Bulls & Cows!"));
    PrintLine(TEXT("Try to guess the secret word!"));
    PrintLine(TEXT("Press 'Enter' to Continue..."));
}

void UBullCowCartridge::ShowMainMenu() const
{
    ClearScreen();
    PrintLine(TEXT("----------Main Menu----------"));
    PrintLine(TEXT(" 1. Play"));
    PrintLine(TEXT(" 2. Select difficulty"));
    PrintLine(TEXT(" 3. High Scores"));
    PrintLine(TEXT(" 4. Exit"));
    PrintLine(TEXT("-----------------------------"));
    PrintLine(TEXT("Please enter menu option number..."));
}

void UBullCowCartridge::ShowSelectDifficultyMenu() const
{
    ClearScreen();
    PrintLine(TEXT("------Select Difficulty------"));
    PrintLine(TEXT(" 1. Easy"));
    PrintLine(TEXT(" 2. Normal"));
    PrintLine(TEXT(" 3. Hard"));
    PrintLine(TEXT("-----------------------------"));
    PrintLine(TEXT("Please enter the menu item number..."));
}

void UBullCowCartridge::ShowPlayScreen() const
{
    ClearScreen();
    ShowHistory();
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
}

void UBullCowCartridge::ShowNameInputScreen() const
{
    ClearScreen();
    PrintLine(TEXT("Please enter your name:"));

}

void UBullCowCartridge::ShowHighScore() const
{
    ClearScreen();
    PrintLine(TEXT("----------High Scores--------"));
    for (int i = 0; i < HighScores.Num(); i++)
    {
        PrintLine(TEXT(" %i. %s   %i"), i + 1, *HighScores[i].Name, HighScores[i].Score);
    }
    PrintLine(TEXT("-----------------------------"));
    PrintLine(TEXT("Press 'Enter' to continue..."));
}

void UBullCowCartridge::UpdateHighScores(const FString& Name, const int32& Score)
{
    //if (HighScores.Num() == 0)
    //{
    //    AddPlayerScoreToHighScores(Name, Score, 0);
    //    return;
    //}
    for (int i = 0; i < HighScores.Num(); i++)
    {
        if (Score > HighScores[i].Score)
        {
            AddPlayerScoreToHighScores(Name, Score, i);
            while (HighScores.Num() > HighScoreLength)
            {
                HighScores.RemoveAt(HighScoreLength);
            }
            return;
        }
    }
    if (HighScores.Num() < HighScoreLength)
    {
        AddPlayerScoreToHighScores(Name, Score, HighScores.Num());
    }
}

void UBullCowCartridge::AddPlayerScoreToHighScores(const FString& Name, const int32& Score, int32 index)
{
    FPlayerScore PlayerScore = { Name, Score };
    if (index == 0)
    {
        HighScores.Add(PlayerScore);
        return;
    }
    HighScores.Insert(PlayerScore, index);
}

void UBullCowCartridge::LoadHighScores()
{
    if (UBCSaveGame* SaveGameInstance = Cast<UBCSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("HighScores"), 0)))
    {
        HighScores = SaveGameInstance->HighScore;
    }    
}

void UBullCowCartridge::SaveHighScores()
{
    if (UBCSaveGame* SaveGameInstance = Cast<UBCSaveGame>(UGameplayStatics::CreateSaveGameObject(UBCSaveGame::StaticClass())))
    {
        SaveGameInstance->HighScore = HighScores;
        UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("HighScores"), 0);
    }
}