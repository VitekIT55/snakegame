// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Interactable.h"
#include <Kismet/GameplayStatics.h>
#include "Engine/GameEngine.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LastMoveDirection = EMovementDirection::DOWN;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawnActor = Cast<APlayerPawnBase>(UGameplayStatics::GetPlayerPawn(this, 0));
	//SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorTickInterval(MovementSpeed);
	Move();
}

void ASnakeBase::DestroySnake()
{
	ASnakeBase::Destroy();
	PlayerPawnActor->SnakeDestroy = 1;
	GetWorld()->ForceGarbageCollection(true);
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; ++i)
	{
		SnakeLenght += 1;
		auto NewLocation = FVector(0, 0, 0);
		if(SnakeElements.Num()>=4)
			NewLocation = SnakeElements[SnakeElements.Num() - 1]->GetActorLocation();
		else
			NewLocation = FVector(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		NewSnakeElem->SnakeOwner = this;
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		if (ElemIndex == 0)
		{
			NewSnakeElem->SetFirstElementType();
		}

		//auto a = SnakeElements[i]->GetActorLocation();
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, a.ToString());
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("i: %i"), i));
	}
}

void ASnakeBase::Move()
{
	FVector MovementVector(FVector::ZeroVector);
	switch (LastMoveDirection)
	{
	case EMovementDirection::UP:
		MovementVector.X += ElementSize;
		break;
	case EMovementDirection::DOWN:
		MovementVector.X -= ElementSize;
		break;
	case EMovementDirection::LEFT:
		MovementVector.Y += ElementSize;
		break;
	case EMovementDirection::RIGHT:
		MovementVector.Y -= ElementSize;
		break;
	}
	RotateBlock = 0;
	BlockSpawn = 1;
	//SnakeElements[0]->ToggleCollision();
	SnakeElements[0]->SetActorEnableCollision(false);
	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	//SnakeElements[0]->ToggleCollision();
	SnakeElements[0]->SetActorEnableCollision(true);
	BlockSpawn = 0;
}

void ASnakeBase::SnakeElemenetOverlap(ASnakeElementBase* OverlappedElement, AActor* Other)
{
	if (IsValid(OverlappedElement))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverlappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(Other);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this, bIsFirst);
		}
	}
}

void ASnakeBase::SnakeHeadLocation()
{
	SnakeHL = SnakeElements[0]->GetActorLocation();
}

