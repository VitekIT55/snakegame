// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "SnakeElementBase.h"
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
	//SpawnRandomFoodActor();
}

// Called every frame
void APlayerPawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (SnakeActor)
	{
		if (Hunger <= 0)
		{
			SnakeActor->DestroySnake();
		}
		else if (Hunger > -0.1)
			Hunger -= 0.001;
		if (SpawnTimer > 0) 
			SpawnTimer -= 1;
		SpawnBrickTimer -= 1;
		if (SpawnTimer == 0 && SnakeActor->BlockSpawn == 0 && ((FoodQuantity + SnakeActor->SnakeLenght) < 225))
		{
			SpawnRandomFoodActor();
			SpawnTimer = 100;
		}
		SnakeActor->SnakeHeadLocation();
		FVector MyCharacterPosition = SnakeActor->SnakeHL;
		if (SpawnBrickTimer == 0)
		{
			SpawnBrickActor(MyCharacterPosition);
			SpawnBrickTimer = 500;
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

void APlayerPawnBase::SpawnRandomFoodActor()
{
	SpawnDelayX = FMath::RandRange(-7, 7) * int(SnakeActor->ElementSize);
	SpawnDelayY = FMath::RandRange(-7, 7) * int(SnakeActor->ElementSize);
	FVector Location(SpawnDelayX, SpawnDelayY, 10.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	float SpawnCheckRadius = 30.0f;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SpawnCheckRadius);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bOverlap = GetWorld()->OverlapAnyTestByChannel(Location, FQuat::Identity, ECC_GameTraceChannel1, CollisionShape, CollisionParams);
	if (bOverlap)
	{
		SpawnRandomFoodActor();
		return;
	}
	AActor* SpawnedFood = GetWorld()->SpawnActor<AActor>(FoodClass, Location, Rotation);
	if (SpawnedFood)
	{
		FoodQuantity += 1;
		auto FQ = FoodQuantity;
		FString FFQ = FString::SanitizeFloat(FQ);
		GEngine->AddOnScreenDebugMessage(-1, 1.0, FColor::Green, *FFQ);
	}
}

void APlayerPawnBase::SpawnBrickActor(FVector F)
{
	int NewX = F.X;
	int NewY = F.Y;
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	for (int i = -1; i <= 1; i++)
	{
		switch (SnakeActor->LastMoveDirection)
		{
		case EMovementDirection::UP:
			NewX = F.X + 2 * int(SnakeActor->ElementSize);
			NewY = F.Y + i * int(SnakeActor->ElementSize);
			break;
		case EMovementDirection::DOWN:
			NewX = F.X - 2 * int(SnakeActor->ElementSize);
			NewY = F.Y + i * int(SnakeActor->ElementSize);
			break;
		case EMovementDirection::LEFT:
			NewY = F.Y + 2 * int(SnakeActor->ElementSize);
			NewX = F.X + i * int(SnakeActor->ElementSize);
			Rotation = FRotator(0, 90, 0);
			break;
		case EMovementDirection::RIGHT:
			NewY = F.Y - 2 * int(SnakeActor->ElementSize);
			NewX = F.X + i * int(SnakeActor->ElementSize);
			Rotation = FRotator(0, 90, 0);
			break;
		}
		FVector Location(NewX, NewY, int(F.Z));

		float SpawnCheckRadius = 30.0f;
		FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SpawnCheckRadius);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		bool bOverlap = GetWorld()->OverlapAnyTestByChannel(Location, FQuat::Identity, ECC_GameTraceChannel1, CollisionShape, CollisionParams);
		if (!bOverlap)
		{
			GetWorld()->SpawnActor<AActor>(BrickClass, Location, Rotation);
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("Player Location: %s"), *MyCharacterPosition.ToString()));
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

