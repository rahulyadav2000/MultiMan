// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WinAreana.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWinAreanaCondition);

UCLASS()
class MULTIMAN_API AWinAreana : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWinAreana();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bWinCondition;

	UFUNCTION(NetMulticast, Reliable)
	void WinConditionHandler();

	UPROPERTY(BlueprintAssignable)
	FWinAreanaCondition OnWinCondition;
};
