// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups.h"
#include "Components/StaticMeshComponent.h"
#include "SurvivalCharacter.h"
#include "PlayerStatComponent.h"
#include "Net/UnrealNetwork.h"
// Sets default values
APickups::APickups()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
    RootComponent = MeshComp;
    
    IncreaseAmount = 30.0f;
}

// Called when the game starts or when spawned
void APickups::BeginPlay()
{
	Super::BeginPlay();
    SetReplicates(true);
}

// Called every frame
void APickups::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//void APickups::DestroyActor()
//{
//    if(Role==ROLE_AUTHORITY)
//    {
//        Destroy();
//    }
//}

bool APickups::MulticastDestroyActor_Validate()
{
    return true;
}
void APickups::MulticastDestroyActor_Implementation()
{
    Destroy();
}
void APickups::UseItem(ASurvivalCharacter* Player)
{
    UE_LOG(LogTemp,Warning,TEXT("TRYing to Add Hunger"));
    if(GetLocalRole()==ROLE_Authority)
    {
        UE_LOG(LogTemp,Warning,TEXT("ADDing Hunger"));
        Player->PlayerStatComp->AddHunger(IncreaseAmount);
        MulticastDestroyActor();
    }
}
