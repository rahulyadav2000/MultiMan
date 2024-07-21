// Fill out your copyright notice in the Description page of Project Settings.


#include "PressurePlatforms.h"


// Sets default values
APressurePlatforms::APressurePlatforms()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	bIsActivated = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Trigger Mesh"));
	TriggerMesh->SetupAttachment(RootComp);
	TriggerMesh->SetIsReplicated(true);

	auto TriggerMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Assets/Cylinder.Cylinder"));
	if(TriggerMeshAsset.Succeeded())
	{
		TriggerMesh->SetStaticMesh(TriggerMeshAsset.Object);
		TriggerMesh->SetRelativeScale3D(FVector(3.3f,3.3f, 0.2f));
		TriggerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Game/Asian_Village/meshes/props/SM_statue_platform_01.SM_statue_platform_01"));
	if(MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeScale3D(FVector(3.3f,3.3f, 0.2f));
		Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, 10.0f));
	}

	Porter = CreateDefaultSubobject<UPorter>(TEXT("Porter"));
	Porter->MoveTime = 0.2f;
	
}

// Called when the game starts or when spawned
void APressurePlatforms::BeginPlay()
{
	Super::BeginPlay();

	TriggerMesh->SetVisibility(false);
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));

	FVector InitialLocation = GetActorLocation();
	FVector TargetLocation = InitialLocation + FVector(0.0f, 0.0f, -10.0f);

	Porter->SetPoints(InitialLocation, TargetLocation);
}

// Called every frame
void APressurePlatforms::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		TArray<AActor*> OverlappingActors;
		AActor* TriggeredActor = 0;
		TriggerMesh->GetOverlappingActors(OverlappingActors);

		for(int i=0; i< OverlappingActors.Num(); i++)
		{
			AActor* Actor = OverlappingActors[i];


			if(Actor->ActorHasTag("TriggerActor"))
			{
				TriggeredActor = Actor; 
				break;
			}
			
		}

		if(TriggeredActor)
		{
			if(!bIsActivated)
			{
				bIsActivated = true;
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("Activated"));
				OnActivated.Broadcast();
			}
		}
		else
		{
			if(bIsActivated)
			{
				bIsActivated = false;
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, TEXT("Deactivated"));
				OnDeactivated.Broadcast();
			}
		}
	}
}

