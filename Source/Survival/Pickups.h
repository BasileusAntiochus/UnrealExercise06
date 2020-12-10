// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickups.generated.h"

UCLASS()
class SURVIVAL_API APickups : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickups();

protected:
    UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* MeshComp;
    
    UPROPERTY(EditAnywhere)
    float IncreaseAmount;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    UFUNCTION(NetMulticast,Reliable)
    void MulticastDestroyActor();
    bool MulticastDestroyActor_Validate();
    void MulticastDestroyActor_Implementation();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
    void UseItem(class ASurvivalCharacter *Player);
	
	
};
