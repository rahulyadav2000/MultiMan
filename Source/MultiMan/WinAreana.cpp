// Fill out your copyright notice in the Description page of Project Settings.


#include "WinAreana.h"

#include "MultiManCharacter.h"
#include "Components/BoxComponent.h"


// Sets default values
AWinAreana::AWinAreana()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(BoxComp);

	bWinCondition = false;
	
}

// Called when the game starts or when spawned
void AWinAreana::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWinAreana::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		if(!bWinCondition)
		{
			TArray<AActor*> ActorPlayers;
			BoxComp->GetOverlappingActors(ActorPlayers, AMultiManCharacter::StaticClass());

			bWinCondition = ActorPlayers.Num() == 2;
			if(bWinCondition)
			{
				UE_LOG(LogTemp, Display, TEXT("You Won!!"));
				WinConditionHandler();
			}
		}
	}
	
}

void AWinAreana::WinConditionHandler_Implementation()
{
	OnWinCondition.Broadcast();
}

