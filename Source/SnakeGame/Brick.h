// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SnakeBase.h"
#include "PlayerPawnBase.h"
#include "Interactable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Brick.generated.h"

class APlayerPawnBase;

UCLASS()
class SNAKEGAME_API ABrick : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABrick();

	UPROPERTY(BlueprintReadWrite)
	APlayerPawnBase* PlayerPawn;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APlayerPawnBase> PlayerPawnClass;

	UPROPERTY(BlueprintReadWrite)
	int BrickHealth = 1000;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(AActor* Interactor, bool bIsHead) override;
};
