// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableActor.h"

#include "Components/ArrowComponent.h"


// Sets default values
AMovableActor::AMovableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	Point1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point 1"));
	Point1->SetupAttachment(RootComp);
	Point1->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	Point2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point 2"));
	Point2->SetupAttachment(RootComp);
	Point2->SetRelativeLocation(FVector(0.0f, 0.0f, 400.0f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);

	Porter = CreateDefaultSubobject<UPorter>(TEXT("Porter"));
}

// Called when the game starts or when spawned
void AMovableActor::BeginPlay()
{
	Super::BeginPlay();

	FVector StartPt = GetActorLocation() + Point1->GetRelativeLocation();
	FVector EndPt = GetActorLocation() + Point2->GetRelativeLocation();

	Porter->SetPoints(StartPt, EndPt);
}

// Called every frame
void AMovableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

