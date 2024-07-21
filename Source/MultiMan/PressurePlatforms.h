// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Porter.h"
#include "GameFramework/Actor.h"
#include "PressurePlatforms.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressurePlatformActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPressurePlatformDeactivated);


UCLASS()
class MULTIMAN_API APressurePlatforms : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APressurePlatforms();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* TriggerMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsActivated;

	UPROPERTY(BlueprintAssignable)
	FOnPressurePlatformActivated OnActivated;

	UPROPERTY(BlueprintAssignable)
	FOnPressurePlatformDeactivated OnDeactivated;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UPorter* Porter;
};
