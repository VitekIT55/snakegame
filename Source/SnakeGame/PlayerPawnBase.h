// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SnakeBase.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawnBase.generated.h"

class UCameraComponent;
class ASnakeBase;

UCLASS()
class SNAKEGAME_API APlayerPawnBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawnBase();

	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* PawnCamera;

	UPROPERTY(BlueprintReadWrite)
	ASnakeBase* SnakeActor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeBase> SnakeActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Score = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Hunger = 1;

	UPROPERTY(EditAnywhere)
	float FoodQuantity = 0;

	UPROPERTY()
	float SpawnDelayX = 0.0f;

	UPROPERTY()
	float SpawnDelayY = 0.0f;

	UPROPERTY()
	int SpawnTimer = 100;

	UPROPERTY()
	int SpawnBrickTimer = 500;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> FoodClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> BrickClass;

	void CreateSnakeActor();
	void SpawnRandomFoodActor();
	void SpawnBrickActor(FVector F);

	UFUNCTION()
	void HandlePlayerVerticalInput(float value);
	UFUNCTION()
	void HandlePlayerHorizontalInput(float value);
};
