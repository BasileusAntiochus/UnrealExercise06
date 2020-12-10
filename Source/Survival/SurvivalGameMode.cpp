// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalGameMode.h"
#include "SurvivalCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
#include "Actions/PawnAction.h"
ASurvivalGameMode::ASurvivalGameMode()
{
	// set default pawn class to our Blueprinted character
    //TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter")
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
void ASurvivalGameMode::Respawn(AController* Controller)
{
    if(Controller){
    if(HasAuthority())
    {
        FVector Location = FVector(-400.0f,50.0f,200.0f);
        if(APawn* Pawn =  GetWorld()->SpawnActor<APawn>(DefaultPawnClass,Location,FRotator::ZeroRotator)){
            Controller->Possess(Pawn);
        }
    }
    }
    
}
