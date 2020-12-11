// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVAL_API UPlayerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerStatComponent();
public:
    UPROPERTY(EditAnywhere,Category="S|PlayerStats")
    FString PlayerName;
protected:
    UPROPERTY(Replicated)
    float Hunger;
    UPROPERTY(EditAnywhere,Category="S|PlayerStats")
    float HungerDecrementValue;
    UPROPERTY(Replicated)
    float Thirst;
    UPROPERTY(EditAnywhere,Category="S|PlayerStats")
    float ThirstDecrementValue;
    UPROPERTY(Replicated)
    float Health;
    
    UPROPERTY(Replicated)
    float Kills;
    UPROPERTY(Replicated)
    float Death;
    FTimerHandle TimerHandle;
protected:
    virtual void GetLifetimeReplicatedProps(TArray <FLifetimeProperty > & OutLifetimeProps) const override;
	// Called when the game starts
	virtual void BeginPlay() override;

    void HandleHungerAndThirst();
    
    UFUNCTION(Server,Reliable,WithValidation)
    void ServerLowerHunger(float Value);
    bool ServerLowerHunger_Validate(float Value);
    void ServerLowerHunger_Implementation(float Value);
    
    UFUNCTION(Server,Reliable,WithValidation)
    void ServerLowerThirst(float Value);
    bool ServerLowerThirst_Validate(float Value);
    void ServerLowerThirst_Implementation(float Value);
    
    UFUNCTION(Server,Reliable,WithValidation)
    void ServerLowerHealth(float Value);
    bool ServerLowerHealth_Validate(float Value);
    void ServerLowerHealth_Implementation(float Value);
    
    UFUNCTION(Server,Reliable,WithValidation)
    void ServerLowerKills(float Value);
    bool ServerLowerKills_Validate(float Value);
    void ServerLowerKills_Implementation(float Value);
    
    UFUNCTION(Server,Reliable,WithValidation)
    void ServerLowerDeath(float Value);
    bool ServerLowerDeath_Validate(float Value);
    void ServerLowerDeath_Implementation(float Value);
public:
    void LowerHunger(float Value);
    void LowerThirst(float Value);
    void LowerHealth(float Value);
    void LowerKills(float Value);
    void LowerDeath(float Value);
    void AddHunger(float Value);
    void AddThirst(float Value);
    void AddHealth(float Value);
    void AddKills(float Value);
    void AddDeath(float Value);
    float GetHunger();
    float GetThirst();
    float GetHealth();
    float GetKills();
    float GetDeath();
    FString GetName();
//    class UPlayerStatComponent* GetStatComponent()
//    {
//        return PlayerStatComp;
//    }
};
