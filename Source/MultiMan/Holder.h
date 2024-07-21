// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Holder.generated.h"

UCLASS()
class MULTIMAN_API AHolder : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHolder();

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
	class UStaticMeshComponent* KeyMesh;

	UFUNCTION()
	void Activator();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationSpeed;
};
