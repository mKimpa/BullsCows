// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    GreetingsMessage();
    GenerateHiddenWord();
    AskForGuess();

}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();
    if (Input == HiddenWord)
    {
        PrintLine(TEXT("You have Won!"));
    }
    else
    {
        WrongInputHandler(Input);
    }
    PrintLine(Input);
}

bool UBullCowCartridge::PlayerInputIsCorrect(const FString& Input)
{
    if (Input.Len() != WordLength)
    {
        return false;
    }
    return true;
}

void UBullCowCartridge::WrongInputHandler(const FString& Input)
{
    Lives--;
    if (Lives > 0)
    {
        if (!PlayerInputIsCorrect(Input))
        {
            PrintLine(TEXT("You have Entered incorrect Word! Lives remain: %i"), Lives);
            return;
        }
        else
        {
            PrintLine(TEXT("Wrong guess! Lives remain: %i"), Lives);
        }
    }
    else
    {
        PrintLine(TEXT("You have Lost!"));
    }
}

void UBullCowCartridge::AskForGuess() const
{
    PrintLine(TEXT("Guess the %i letter word!"), WordLength);
}

void UBullCowCartridge::GreetingsMessage() const
{
    PrintLine(TEXT("Welcome to Bulls & Cows!"));
}

void UBullCowCartridge::GenerateHiddenWord()
{
    HiddenWord = TEXT("cake"); // TODO: Select from list
    WordLength = HiddenWord.Len();
}
