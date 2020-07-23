// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"


void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();


    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);
    GetValidWords(Words);

    SetupGame();
    // PrintLine(TEXT("The number is %i"), Words.Num());
    // PrintLine(FString::Printf(TEXT("The HiddenWord is: %s"), *HiddenWord));
    PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord);



    // for (int32 i = 0; i < QualifiedWords.Num(); ++i) {

    //     PrintLine(TEXT("The number of words is: %i"), QualifiedWords.Num());
    // }
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (bGameOver) {
        ClearScreen();
        SetupGame();
    } else {
        ProcessGuess(Input);

}       
    
}

void UBullCowCartridge::SetupGame(){

    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = QualifiedWords[FMath::RandRange(0, QualifiedWords.Num()-1)];
        PrintLine(FString::Printf(TEXT("The HiddenWord is: %s"), *HiddenWord));
    Lives = HiddenWord.Len();
    bGameOver = false;
    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());

    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Type in your guess \nPress Enter to continue..."));



    
}

void UBullCowCartridge::EndGame(){
    bGameOver = true;

    PrintLine(TEXT("\nPress enter to play again!"));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess){
    if (Guess == HiddenWord) {
        PrintLine(TEXT("You have won!"));
        EndGame();
        return;
    } 

    Lives--;
    PrintLine(TEXT("Lost a life"));
    if (Lives <= 0) {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The HiddenWord was: %s"), *HiddenWord);
        EndGame();
        return;
    }
    

    
    if (Guess.Len() != HiddenWord.Len()){
        PrintLine(TEXT("Sorry, try again, you have %i lives remaining"), Lives);
        PrintLine(TEXT("The hidden word is %i letter long"), HiddenWord.Len());
    }

    if (!IsIsogram(Guess)) {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("Sorry, try again, you have %i lives remaining"), Lives);

}

bool UBullCowCartridge::IsIsogram(const FString &s) const{

    for (int32 i = 0; i < s.Len(); i++) {
        for (int32 compar = i + 1; compar < s.Len(); compar++) {
            if (s[i] == s[compar]) {
                return false;
            }
        }
      
    }
    return true;
}

void UBullCowCartridge::GetValidWords(const TArray<FString> &raw) {
    // for (int32 i = 0; i < raw.Num(); ++i) {

    //     if (raw[i].Len() >=4 && raw[i].Len()<=8 && IsIsogram(raw[i])){
    //             QualifiedWords.Emplace(raw[i]);

    //     }
    // }
    for (FString word: raw) {
            if (word.Len() >=4 && word.Len()<=8 && IsIsogram(word)){
            QualifiedWords.Emplace(word);

        }
    }

}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const {

    FBullCowCount Count;

    for (int32 i = 0; i<Guess.Len(); ++i) {
        if (Guess[i] == HiddenWord[i]){

            Count.Bulls++;
            continue;
        }

        for (int32 j = 0; j < HiddenWord.Len(); j++)
        {
            if(Guess[i] == HiddenWord[j]) {

                Count.Cows++;
                break;
            }
        }
        
    }
    return Count;
};