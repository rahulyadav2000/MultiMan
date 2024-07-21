// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Porter.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MULTIMAN_API UPorter : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPorter();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FVector StartPoint;
	FVector EndPoint;

	bool bArePointsSet;

	UPROPERTY(EditAnywhere)
	float MoveTime;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> TriggeredActors;

	UPROPERTY(VisibleAnywhere)
	int Count;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bAllTriggerd;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsTriggerPlatform;

	UFUNCTION(BlueprintCallable)
	void SetPoints(FVector Start, FVector End);

	UFUNCTION()
	void OnTriggeredActorActivated();

	UFUNCTION()
	void OnTriggeredActorDeactivated();
};



