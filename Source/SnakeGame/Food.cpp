// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "Engine/GameEngine.h"
#include "PlayerPawnBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("AFood::BeginPlay - Failed to get player pawn!"));
		return;
	}
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFood::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead)
	{
		if (PlayerPawn)
		{
			auto a = PlayerPawn->Hunger;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("a: %a"), a));
			PlayerPawn->Hunger = 1;
			PlayerPawn->Score += 1;
		}
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake))
		{
			Snake->AddSnakeElement();
		}
		AFood::Destroy();
	}
}
