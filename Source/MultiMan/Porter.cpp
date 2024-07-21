// Fill out your copyright notice in the Description page of Project Settings.


#include "Porter.h"

#include "CollectableKey.h"
#include "PressurePlatforms.h"


// Sets default values for this component's properties
UPorter::UPorter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MoveTime = 3.0f;
	Count = 0;

	bArePointsSet = false;

	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
}


// Called when the game starts
void UPorter::BeginPlay()
{
	Super::BeginPlay();
	if(bIsTriggerPlatform)
	{
		TriggeredActors.Add(GetOwner());
	} 

	for(AActor* Actor : TriggeredActors)
	{
		APressurePlatforms* PressurePlatforms = Cast<APressurePlatforms>(Actor);
		if(PressurePlatforms)
		{
			PressurePlatforms->OnActivated.AddDynamic(this, &UPorter::OnTriggeredActorActivated);
			PressurePlatforms->OnDeactivated.AddDynamic(this, &UPorter::OnTriggeredActorDeactivated);
			continue;
		}

		ACollectableKey* Keys = Cast<ACollectableKey>(Actor);
		if(Keys)
		{
			Keys->OnCollected.AddDynamic(this, &UPorter::OnTriggeredActorActivated);
		}
	}
	
}


// Called every frame
void UPorter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(TriggeredActors.Num() > 0)
	{
		bAllTriggerd = Count >= TriggeredActors.Num();
		if(bAllTriggerd)
		{
			
		}
	}

	AActor* Owner = GetOwner();
	if(Owner && Owner->HasAuthority() && bArePointsSet)
	{
		FVector CurrentLocation = Owner->GetActorLocation();
		float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

		FVector TargetLocation = bAllTriggerd ? EndPoint : StartPoint;

		if(!CurrentLocation.Equals(TargetLocation))
		{
			FVector DesiredLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			Owner->SetActorLocation(DesiredLocation);
		}
	}
}

void UPorter::SetPoints(FVector Start, FVector End)
{
	if(Start.Equals(End)) return;

	StartPoint = Start;
	EndPoint = End;

	bArePointsSet = true;
}

void UPorter::OnTriggeredActorActivated()
{
	Count++;
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString(FString::Printf(TEXT("Activated Count: %d"), Count)));
}

void UPorter::OnTriggeredActorDeactivated()
{
	Count--;
}

