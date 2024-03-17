// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "SnakeElementBase.h"
#include "Interactable.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ElementSize = 100.f;
	MovementSpeed = 1.f;
	LastMoveDirection = EMovementDirection::DOWN;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();
}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; ++i)
	{
		//if (SnakeElements.Num() < 4)
		if (ElementsNum != 0)
		{
			//while (i < SnakeElements.Num())
			//{
				//++i;
				FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
				FTransform NewTransform(NewLocation);
				ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
				NewSnakeElem->SnakeOwner = this;
				int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
				if (ElemIndex == 0)
				{
					NewSnakeElem->SetFirstElementType();
				}
				//i++;
			//}
		}
		//else
		//{
		//	FVector NewLocation = SnakeElements[i]->GetActorLocation();
		//	//FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		//	FTransform NewTransform(NewLocation);
		//	ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		//	NewSnakeElem->SnakeOwner = this;
		//	int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		//}
		//auto PrevElement = SnakeElements[i - 1];
		//FVector PrevLocation = PrevElement->GetActorLocation();
		//CurrentElement->SetActorLocation(PrevLocation);
		//auto ActorElement = SnakeElements[i];
		//FVector NewLocation = ActorElement->GetActorLocation();
		//FVector NewLocation(ActorLocation * ElementSize, 0, 0);
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
	SnakeElements[0]->ToggleCollision();
	for (int i = SnakeElements.Num() - 1; i > 0; i--)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();
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

