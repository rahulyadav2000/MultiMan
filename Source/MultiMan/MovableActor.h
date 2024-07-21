// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Porter.h"
#include "GameFramework/Actor.h"
#include "MovableActor.generated.h"

UCLASS()
class MULTIMAN_API AMovableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMovableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UArrowComponent* Point1;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UArrowComponent* Point2;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UStaticMeshComponent* Mesh;
 
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UPorter* Porter;
	
};
