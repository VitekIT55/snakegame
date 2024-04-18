// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawnBase.h"
#include "SnakeElementBase.h"
#include "Engine/GameEngine.h"
#include "Food.h"
#include <Kismet/GameplayStatics.h>
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
	if (SnakeActor)
	{
		if (Hunger <= 0)
		{
			SnakeActor->DestroySnake();
		}
		else if (Hunger > -0.1)
			Hunger -= 0.0005;
		SpawnBrickTimer -= 1;
		if (SpawnFoodAllow == 1 && SnakeActor->BlockSpawn == 0 && ((FoodQuantity + SnakeActor->SnakeLenght) < 225))
		{
			SpawnRandomFoodActor();
			SpawnFoodAllow = 0;
		}
		SnakeActor->SnakeHeadLocation();
		FVector MyCharacterPosition = SnakeActor->SnakeHL;
		if (SpawnBrickTimer == 0)
		{
			SpawnBrickActor(MyCharacterPosition);
			SpawnBrickTimer = 1000;
		}
		if (BonusTime > 0)
			BonusTime -= 1;
		else if (BonusTime == 0 && BonusActive == 1)
		{
			switch (BonusType)
			{
			case 1:
				SnakeActor->MovementSpeed *= 2.0f;
				break;
			case 2:
				SnakeActor->MovementSpeed /= 2.0f;
				break;
			case 3:
				SetAllBricksCollision(0);
				BricksCollision = 0;
				break;
			}
			BonusType = 0;
			BonusActive = 0;
		}
		SnakeDestroy = SnakeActor->SnakeBaseDead;
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

void APlayerPawnBase::SpawnRandomFoodActor(bool Type)
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
	if (Type == 0)
	{
		AActor* SpawnedFood = GetWorld()->SpawnActor<AActor>(FoodClass, Location, Rotation);
		if (SpawnedFood) FoodQuantity += 1;
	}
	else
	{
		AActor* SpawnedFood = GetWorld()->SpawnActor<AActor>(GoldFoodClass, Location, Rotation);
		if (SpawnedFood) FoodQuantity += 1;
	}
}

void APlayerPawnBase::SpawnBrickActor(FVector F)
{
	int NewX = F.X;
	int NewY = F.Y;
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	int ImaginaryObstacle = FMath::RandRange(0, 1);
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
			if (i == ImaginaryObstacle)
			{
				AActor* NewActor = GetWorld()->SpawnActor<AActor>(FakeBrickClass, Location, Rotation);
				NewActor->SetActorEnableCollision(false);
			}
			else
			{
				AActor* NewActor = GetWorld()->SpawnActor<AActor>(BrickClass, Location, Rotation);
				if (BricksCollision == 1) SetAllBricksCollision(1);
			}
		}
	}
}

void APlayerPawnBase::SetAllBricksCollision(bool Type)
{
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BrickClass, ActorsToFind);
	for (AActor* Actor : ActorsToFind)
	{
		if (Type == 1)
			Actor->SetActorEnableCollision(false);
		else
			Actor->SetActorEnableCollision(true);
	}
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

