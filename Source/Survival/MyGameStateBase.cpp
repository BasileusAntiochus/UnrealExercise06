// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Containers/Array.h"
#include "Engine/Engine.h"
#include "PlayerStatComponent.h"
AMyGameStateBase::AMyGameStateBase():Time(60.0f)
{
    
}
void AMyGameStateBase::GetLifetimeReplicatedProps(TArray <FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // Replicate to everyone
    DOREPLIFETIME(AMyGameStateBase,Time);
}
void AMyGameStateBase::BeginPlay()
{
    Super::BeginPlay();
    // ...
    SetReplicates(true);
}

//AMyGameStateBase::~AMyGameStateBase()
//{
//    
//}
FString AMyGameStateBase::ReturnGameStats()
{
    FString RetString = "Time:" + FString::SanitizeFloat(Time/60.0f);
    return RetString;
}

