// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableKey.h"
#include "MultiManCharacter.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ACollectableKey::ACollectableKey()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	RotationSpeed = 1000.0f;
	
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
	Mesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	CapsuleComp->SetupAttachment(RootComp);
	CapsuleComp->SetIsReplicated(true);
	CapsuleComp->SetCollisionProfileName(FName("OverlapAllDynamic"));
	CapsuleComp->SetCapsuleHalfHeight(150.0f);
	CapsuleComp->SetCapsuleRadius(100.0f);

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComp->SetupAttachment(RootComp);
	AudioComp->SetAutoActivate(false);

}

void ACollectableKey::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACollectableKey, bIsCollected);
}

// Called when the game starts or when spawned
void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
	InitialLoc = Mesh->GetRelativeLocation();
}

// Called every frame
void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority())
	{
		Mesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
		//float y= FMath::Sin(2.0f + DeltaTime);
		//Mesh->SetRelativeLocation(FVector(0.0f, 0.0f, y + DeltaTime));
		TArray<AActor*> OverlappingActors;
		CapsuleComp->GetOverlappingActors(OverlappingActors, AMultiManCharacter::StaticClass());

		if(OverlappingActors.Num() > 0)
		{
			if(!bIsCollected)
			{
				bIsCollected = true;
				OnRep_IsCollected(); 
			}
		}
	}
}

void ACollectableKey::OnRep_IsCollected()
{
	if(HasAuthority())
	{
		UE_LOG(LogTemp, Display, TEXT("OnRep_IsCollected has been called on Server!!"));
		if(bIsCollected)
		{
			OnCollected.Broadcast();  
		}
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("OnRep_IsCollected has been called on Client!!"));
	}

	Mesh->SetVisibility(!bIsCollected);
	AudioComp->Play();

	if(bIsCollected)
	{
		if(KeyHolder)
		{
			KeyHolder->Activator(); 
		}
	}
}
