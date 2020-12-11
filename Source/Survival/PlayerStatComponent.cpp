// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Containers/Array.h"
#include <random>
// Sets default values for this component's properties
UPlayerStatComponent::UPlayerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
    int n = rand()%10+3;
    for(int i=0;i<n;i++)
    {
        int now = rand()%26;
        PlayerName += char(now+'a');
    }
    
    Hunger = 100.0f;
    HungerDecrementValue = 0.3f;
    Thirst = 100.0f;
    ThirstDecrementValue = 0.5f;
    Health = 100.0f;
    Kills = 0.0f;
    Death = 0.0f;
}


// Called when the game starts
void UPlayerStatComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
    SetIsReplicated(true);
    
    GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&UPlayerStatComponent::HandleHungerAndThirst,3.0f,true);
}

void UPlayerStatComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate to everyone
    DOREPLIFETIME(UPlayerStatComponent,Health);
    DOREPLIFETIME(UPlayerStatComponent,Hunger);
    DOREPLIFETIME(UPlayerStatComponent,Thirst);
    DOREPLIFETIME(UPlayerStatComponent,Kills);
    DOREPLIFETIME(UPlayerStatComponent,Death);
}

void UPlayerStatComponent::HandleHungerAndThirst()
{
    if(GetOwnerRole()==ROLE_Authority)
    {
        LowerHunger(HungerDecrementValue);
        LowerThirst(ThirstDecrementValue);
    }
}
void UPlayerStatComponent::LowerHunger(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerHunger(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Hunger-=Value;
    }
    
}
void UPlayerStatComponent::LowerThirst(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerThirst(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Thirst -= Value;
    }
}
void UPlayerStatComponent::LowerHealth(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerHealth(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Health -= Value;
        if(Health<0.0f)
            Health = 0.0f;
    }
}
void UPlayerStatComponent::LowerKills(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerKills(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Kills -= Value;
        if(Kills<0.0f)
            Kills = 0.0f;
    }
}

void UPlayerStatComponent::LowerDeath(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerDeath(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Death -= Value;
        if(Death<0.0f)
            Death = 0.0f;
    }
}
bool UPlayerStatComponent::ServerLowerHunger_Validate(float Value)
{
    return true;
}
void UPlayerStatComponent::ServerLowerHunger_Implementation(float Value)
{
    if(GetOwnerRole()==ROLE_Authority)
    {
        LowerHunger(Value);
    }
}
bool UPlayerStatComponent::ServerLowerThirst_Validate(float Value)
{
    return true;
}
void UPlayerStatComponent::ServerLowerThirst_Implementation(float Value)
{
    if(GetOwnerRole()==ROLE_Authority)
    {
        LowerThirst(Value);
    }
}
bool UPlayerStatComponent::ServerLowerHealth_Validate(float Value)
{
    return true;
}
void UPlayerStatComponent::ServerLowerHealth_Implementation(float Value)
{
    if(GetOwnerRole()==ROLE_Authority)
    {
        LowerHealth(Value);
    }
}
bool UPlayerStatComponent::ServerLowerKills_Validate(float Value)
{
    return true;
}
void UPlayerStatComponent::ServerLowerKills_Implementation(float Value)
{
    if(GetOwnerRole()==ROLE_Authority)
    {
        LowerKills(Value);
    }
}
bool UPlayerStatComponent::ServerLowerDeath_Validate(float Value)
{
    return true;
}
void UPlayerStatComponent::ServerLowerDeath_Implementation(float Value)
{
    if(GetOwnerRole()==ROLE_Authority)
    {
        LowerDeath(Value);
    }
}
float UPlayerStatComponent::GetHunger()
{
    return Hunger;
}
float UPlayerStatComponent::GetThirst()
{
    return Thirst;
}
float UPlayerStatComponent::GetHealth()
{
    return Health;
}
float UPlayerStatComponent::GetKills()
{
    return Kills;
}
float UPlayerStatComponent::GetDeath()
{
    return Death;
}
void UPlayerStatComponent::AddHunger(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerHunger(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Hunger-=Value;
    }
    
}
void UPlayerStatComponent::AddThirst(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerThirst(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Thirst -= Value;
    }
}
void UPlayerStatComponent::AddHealth(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerHealth(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Thirst -= Value;
    }
}
void UPlayerStatComponent::AddKills(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerKills(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Kills -= Value;
    }
}
void UPlayerStatComponent::AddDeath(float Value)
{
    if(GetOwnerRole()<ROLE_Authority)//is a client
    {
        ServerLowerDeath(Value);
    }
    else if(GetOwnerRole()==ROLE_Authority)
    {
        Death -= Value;
    }
}
FString UPlayerStatComponent::GetName()
{
    return PlayerName;
}


