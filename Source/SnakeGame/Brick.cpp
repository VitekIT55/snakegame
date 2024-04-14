// Fill out your copyright notice in the Description page of Project Settings.


#include "Brick.h"
#include "Engine/GameEngine.h"
#include "PlayerPawnBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABrick::ABrick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABrick::BeginPlay()
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
void ABrick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABrick::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead)
	{
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake))
		{
			Snake->DestroySnake();
		}
	}
}
