// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "Engine/GameEngine.h"
#include "Food.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "Components/InputComponent.h"

// Sets default values
APlayerPawnBase::APlayerPawnBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnCamera"));
	RootComponent = PawnCamera;
}

// Called when the game starts or when spawned
void APlayerPawnBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorRotation(FRotator(-90, 0, 0));
	CreateSnakeActor();
}

// Called every frame
void APlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto a = SpawnTimer;
	FString TheFloatStr = FString::SanitizeFloat(a);
	if (SnakeActor)
	{
		if (SpawnTimer % 50 == 0)
			GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Red, *TheFloatStr);
		if (Hunger <= 0)
		{
			SnakeActor->DestroySnake();
			GetWorld()->ForceGarbageCollection(true);
		}
		else if (Hunger > -0.1)
			Hunger -= 0.001;
		SpawnTimer -= 1;
		if (SpawnTimer == 0)
		{
			SpawnDelayX = FMath::RandRange(-7, 7); SpawnDelayX *= int(SnakeActor->ElementSize);
			SpawnDelayY = FMath::RandRange(-7, 7); SpawnDelayY *= int(SnakeActor->ElementSize);
			SpawnRandomActor(SpawnDelayX, SpawnDelayY);
			SpawnTimer = 500;
		}
	}
}

// Called to bind functionality to input
void APlayerPawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Vertical", this, &APlayerPawnBase::HandlePlayerVerticalInput);
	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerPawnBase::HandlePlayerHorizontalInput);
}

void APlayerPawnBase::CreateSnakeActor()
{
	SnakeActor = GetWorld()->SpawnActor<ASnakeBase>(SnakeActorClass, FTransform());
}

void APlayerPawnBase::SpawnRandomActor(float X, float Y)
{
	FVector Location(X, Y, 10.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	GetWorld()->SpawnActor<AActor>(FoodClass, Location, Rotation);
}

void APlayerPawnBase::HandlePlayerVerticalInput(float value)
{
	if (IsValid(SnakeActor))
	{
		if (value > 0 && SnakeActor->LastMoveDirection != EMovementDirection::DOWN && SnakeActor->RotateBlock != 1)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::UP;
			SnakeActor->RotateBlock = 1;
		}
		else if (value < 0 && SnakeActor->LastMoveDirection != EMovementDirection::UP && SnakeActor->RotateBlock != 1)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::DOWN;
			SnakeActor->RotateBlock = 1;
		}
	}
}

void APlayerPawnBase::HandlePlayerHorizontalInput(float value)
{
	if (IsValid(SnakeActor))
	{
		if (value > 0 && SnakeActor->LastMoveDirection != EMovementDirection::LEFT && SnakeActor->RotateBlock != 1)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::RIGHT;
			SnakeActor->RotateBlock = 1;
		}
		else if (value < 0 && SnakeActor->LastMoveDirection != EMovementDirection::RIGHT && SnakeActor->RotateBlock != 1)
		{
			SnakeActor->LastMoveDirection = EMovementDirection::LEFT;
			SnakeActor->RotateBlock = 1;
		}
	}
}

