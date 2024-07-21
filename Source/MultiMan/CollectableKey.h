// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Holder.h"
#include "GameFramework/Actor.h"
#include "CollectableKey.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyCollected);

UCLASS()
class MULTIMAN_API ACollectableKey : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACollectableKey();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(ReplicatedUsing= OnRep_IsCollected, BlueprintReadWrite, VisibleAnywhere)
	bool bIsCollected;

	UFUNCTION()
	void OnRep_IsCollected();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationSpeed;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UAudioComponent* AudioComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AHolder* KeyHolder;

	FOnKeyCollected OnCollected;

	FVector InitialLoc;
};

