// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawnBase.h"
#include "GameFramework/Actor.h"
#include "SnakeBase.generated.h"

class ASnakeElementBase;
class APlayerPawnBase;

UENUM()
enum class EMovementDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

UCLASS()
class SNAKEGAME_API ASnakeBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeBase();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASnakeElementBase> SnakeElementClass;

	UPROPERTY(EditAnywhere)
	APlayerPawnBase* PlayerPawnActor;

	UPROPERTY()
	TArray<ASnakeElementBase*> SnakeElements;

	UPROPERTY()
	EMovementDirection LastMoveDirection;

	UPROPERTY(EditAnywhere)
	float ElementSize = 60; 

	UPROPERTY(EditAnywhere)
	float MovementSpeed = 0.5;

	UPROPERTY(EditDefaultsOnly)
	FVector SnakeHL;

	UPROPERTY(EditAnywhere)
	bool RotateBlock = 0;

	UPROPERTY(EditAnywhere)
	int SnakeLenght = 0;

	UPROPERTY(EditAnywhere)
	bool BlockSpawn = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void AddSnakeElement(int ElementsNum = 1);
	UFUNCTION(BlueprintCallable)
	void Move();
	UFUNCTION()
	void SnakeElemenetOverlap(ASnakeElementBase* OverlappedElement, AActor* Other);

	void SnakeHeadLocation();
	void DestroySnake();
};
