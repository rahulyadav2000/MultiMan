// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionsSubsystem.h"

USessionsSubsystem::USessionsSubsystem()
{
	CreateServerAfterDestroy = false;
	DestroyServerName = "";
	ServerNameToFind = "";
	MySessionName = FName("MultiMan Adventure Session Name");
	RandomServerInt = FMath::RandRange(1000, 5000);
}

void USessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();

	if(OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
	}

	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if(SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USessionsSubsystem::OnCreatedSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &USessionsSubsystem::OnDestroySessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USessionsSubsystem::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USessionsSubsystem::OnJoinSessionComplete);
	}
}

void USessionsSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void USessionsSubsystem::CreateServer(FString ServerName)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString("Create Server!!")); 

	if(ServerName.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString("Server Cannot Be Found!!"));
		ServerCreationDelegate.Broadcast(false);
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	
	if(ExistingSession)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString::Printf(TEXT("OnCreatedSessionCompleted: %s"), *MySessionName.ToString()));
		CreateServerAfterDestroy = true;
		
		ServerName = FString::FromInt(RandomServerInt);
		DestroyServerName = ServerName;
		SessionInterface->DestroySession(MySessionName);
		return;
	}
	
	FOnlineSessionSettings SessionSettings;

	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 3;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	bool IsLAN = false;
	if(IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLAN = true;
	}
	SessionSettings.bIsLANMatch = IsLAN;

	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void USessionsSubsystem::FindServer(FString ServerName)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString("Find Server!!"));

	if(ServerName.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString("Server Name Cannot Be Empty!!"));
		ServerJoiningDelegate.Broadcast(false);
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	bool IsLAN = false;
	if(IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		IsLAN = true;
	}
	SessionSearch->bIsLanQuery = IsLAN;
	SessionSearch->MaxSearchResults = 9999;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void USessionsSubsystem::OnCreatedSessionComplete(FName SessionName, bool WasSuccess)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString::Printf(TEXT("OnCreatedSessionCompleted: %s"), WasSuccess ? TEXT("true") : TEXT("false")));

	ServerCreationDelegate.Broadcast(WasSuccess);
	
	if(WasSuccess)
	{
		FString Path = "/Game/ThirdPerson/Maps/ThirdPersonMap?listen";
		if(!MapToConnectPath.IsEmpty())
		{
			Path = FString::Printf(TEXT("%s?listen"), *MapToConnectPath);
		}
		
		GetWorld()->ServerTravel(Path);
	}
}

void USessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool WasSuccess)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString::Printf(TEXT("OnDestroySessionCompleted, SessionName: %s, Success: %s,"),*SessionName.ToString() , WasSuccess ? TEXT("true") : TEXT("false")));

	if(CreateServerAfterDestroy)
	{
		CreateServerAfterDestroy = false;
		CreateServer(DestroyServerName);
	}
}

void USessionsSubsystem::OnFindSessionsComplete(bool WasSuccess)
{
	if(!WasSuccess)
	{
		return;
	}
	if(ServerNameToFind.IsEmpty())
	{
		return;
	}
	

	TArray<FOnlineSessionSearchResult> Results = SessionSearch->SearchResults;
	FOnlineSessionSearchResult* DesiredResult = 0;
	if(Results.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString::Printf(TEXT("%d Sessions Found!"), Results.Num()));

		for(FOnlineSessionSearchResult Result : Results)
		{
			if(Result.IsValid())
			{
				FString ServerName = "No-Name";
				Result.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);


				if(ServerName.Equals(ServerNameToFind))
				{
					DesiredResult = &Result;
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString::Printf(TEXT("Server Name Found: %s"), *ServerName));
					break;
				}
				
			}
			
		}

		if(DesiredResult)
		{
			SessionInterface->JoinSession(0, MySessionName, *DesiredResult);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString::Printf(TEXT("Server Name Can't be Found: %s"), *ServerNameToFind));
			ServerNameToFind = "";
			ServerJoiningDelegate.Broadcast(false);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString("No Session was Found!!"));
		ServerJoiningDelegate.Broadcast(false);
	}
}

void USessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	ServerJoiningDelegate.Broadcast(Result == EOnJoinSessionCompleteResult::Success);
	
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString(FString::Printf(TEXT("Successfully Joined Session: %s"), *SessionName.ToString())));

		FString Address = "";
		bool Success = SessionInterface->GetResolvedConnectString(MySessionName, Address);
		if(Success)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString(FString::Printf(TEXT("Address: %s"), *Address)));
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();

			if(PlayerController)
			{
				PlayerController->ClientTravel(Address, TRAVEL_Absolute);
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString("No Travelling Occured"));
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Turquoise, FString("OnJoinSessionComplete Failed!!"));
	}
}
