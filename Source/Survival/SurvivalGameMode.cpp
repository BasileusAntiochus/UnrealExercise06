// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalGameMode.h"
#include "SurvivalCharacter.h"

#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/Actor.h"
//#include "Actions/PawnAction.h"
#include "EngineUtils.h"
ASurvivalGameMode::ASurvivalGameMode()
{
	// set default pawn class to our Blueprinted character
    //TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter")
    
    PrimaryActorTick.bStartWithTickEnabled = true;
    PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    //
    //MyGameState = GetWorld() != NULL ? GetWorld()->GetGameState<AMyGameStateBase>() : NULL;
//    MyGameState = new AMyGameStateBase();
//    MyGameState->Time = 3600;
    GameStateClass = AMyGameStateBase::StaticClass();
}
void ASurvivalGameMode::Respawn(AController* Controller)
{
    if(Controller){
    if(HasAuthority())
    {
        FVector Location = FVector(-400.0f,50.0f,200.0f);
        ASurvivalCharacter* ASC = Cast<ASurvivalCharacter>(Controller->GetCharacter());
        float Kills = ASC->PlayerStatComp->GetKills();
        float Death = ASC->PlayerStatComp->GetDeath();
        if(APawn* Pawn =  GetWorld()->SpawnActor<APawn>(DefaultPawnClass,Location,FRotator::ZeroRotator)){
            Controller->Possess(Pawn);
            ASurvivalCharacter* aSC = Cast<ASurvivalCharacter>(Controller->GetCharacter());
            aSC->PlayerStatComp->AddKills(-1.0*Kills);
            aSC->PlayerStatComp->AddDeath(-1.0*Death);
            aSC->PlayerStatComp->AddDeath(-1.0);
        }
    }
    }
    
}
void ASurvivalGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if(HasAuthority())
    {
        //MyGameState->Time = MyGameState->Time - DeltaTime;
        GetGameState<AMyGameStateBase>()->Time-=DeltaTime;
//        if(GetGameState<AMyGameStateBase>()->Time<=1.0)
//        {
//            for (TActorIterator<ASurvivalCharacter > mycharactorItr(GetWorld());mycharactorItr;++mycharactorItr)
//            {
//                UE_LOG(LogTemp, Warning, TEXT("%s"), *mycharactorItr->ReturnPlayerStats());
//            }
//        }
    }
}
