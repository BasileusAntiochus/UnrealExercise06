// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameStateBase.h"
#include "SurvivalGameMode.generated.h"

UCLASS(minimalapi)
class ASurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ASurvivalGameMode();
    AMyGameStateBase* MyGameState;

public:
    void Respawn(AController* Controller);

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
};



