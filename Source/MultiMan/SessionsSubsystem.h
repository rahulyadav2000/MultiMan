// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "Online//OnlineSessionNames.h"
#include "SessionsSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerCreationDelegate, bool, WasSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerJoiningDelegate, bool, WasSuccess);

/**
 * 
 */
UCLASS()
class MULTIMAN_API USessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	USessionsSubsystem();

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	IOnlineSessionPtr SessionInterface;

	UFUNCTION(BlueprintCallable)
	void CreateServer(FString ServerName);

	UFUNCTION(BlueprintCallable)
	void FindServer(FString ServerName);

	void OnCreatedSessionComplete(FName SessionName, bool WasSuccess);
	void OnDestroySessionComplete(FName SessionName, bool WasSuccess);
	void OnFindSessionsComplete(bool WasSuccess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	bool CreateServerAfterDestroy;
	FString DestroyServerName;
	FString ServerNameToFind;
	FName MySessionName;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	UPROPERTY(BlueprintAssignable)
	FServerCreationDelegate ServerCreationDelegate;

	UPROPERTY(BlueprintAssignable)
	FServerJoiningDelegate ServerJoiningDelegate;

	UPROPERTY(BlueprintReadWrite)
	int RandomServerInt;

	UPROPERTY(BlueprintReadWrite)
	FString MapToConnectPath;
};

