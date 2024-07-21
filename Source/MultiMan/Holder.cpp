// Fill out your copyright notice in the Description page of Project Settings.


#include "Holder.h"


// Sets default values
AHolder::AHolder()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
	Mesh->SetCollisionProfileName(FName("BlockAllDynamic"));

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key Mesh"));
	KeyMesh->SetupAttachment(RootComp);
	KeyMesh->SetIsReplicated(true);
	KeyMesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	RotationSpeed = 100.0f;
}

// Called when the game starts or when spawned
void AHolder::BeginPlay()
{
	Super::BeginPlay();

	KeyMesh->SetVisibility(false);
}

// Called every frame
void AHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	KeyMesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
}

void AHolder::Activator()
{
	KeyMesh->SetVisibility(true);
}

