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
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FoodHealth == 0)
	{
		PlayerPawn->FoodQuantity -= 1;
		if (Gold != 1)	PlayerPawn->SpawnFoodAllow = 1;
		AFood::Destroy();
	}
	else
		FoodHealth -= 1;
}

void AFood::Interact(AActor* Interactor, bool bIsHead)
{
	if (bIsHead)
	{
		auto Snake = Cast<ASnakeBase>(Interactor);
		if (IsValid(Snake))
		{
			int Bonus = FMath::RandRange(1, 1);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Blue , *FString::SanitizeFloat(Bonus));
			if (Bonus == 1)
			{
				PlayerPawn->BonusTime = 500;
				if (PlayerPawn->BonusActive == 0)
				{
					PlayerPawn->BonusType = FMath::RandRange(1, 4);
					//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Green, *FString::SanitizeFloat(PlayerPawn->BonusType));
					switch (PlayerPawn->BonusType)
					{
					case 1:
						PlayerPawn->SnakeActor->MovementSpeed /= 2.0f;
						break;
					case 2:
						PlayerPawn->SnakeActor->MovementSpeed *= 2.0f;
						break;
					case 3:
						PlayerPawn->SetAllBricksCollision(1);
						PlayerPawn->BricksCollision = 1;
						break;
					case 4:
						if (Gold != 1)	PlayerPawn->SpawnRandomFoodActor(1);
						break;
					}
					PlayerPawn->BonusActive = 1;
				}
				//GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Black, *FString::SanitizeFloat(PlayerPawn->SnakeActor->MovementSpeed));
			}
			PlayerPawn->Hunger = 1;
			PlayerPawn->Score += 1;
			if (Gold != 1)	PlayerPawn->SpawnFoodAllow = 1;
			Snake->AddSnakeElement();
		}
		AFood::Destroy();
	}
}
