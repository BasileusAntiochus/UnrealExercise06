// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
    public:
    AMyGameStateBase();
    virtual ~AMyGameStateBase() = default;
        
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Replicated)
    float Time;

protected:
    virtual void GetLifetimeReplicatedProps(TArray <FLifetimeProperty > & OutLifetimeProps) const override;
    virtual void BeginPlay() override;
    UFUNCTION(BlueprintPure)
    FString ReturnGameStats();
};
