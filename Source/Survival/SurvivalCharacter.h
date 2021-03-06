// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerStatComponent.h"
#include "SurvivalCharacter.generated.h"

UCLASS(config=Game)
class ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ASurvivalCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

    
    //UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    //int GetHitBy;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int Kills;
    
     UAnimSequence *Anim;
//    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
//    int ID;
protected:
    class ULineTrace* LineTraceComp;
    UFUNCTION(BlueprintPure)
    FString ReturnPlayerStats();
protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

    void Interact();
    
    UFUNCTION(Server,Reliable,WithValidation)
    void ServerInteract();
    bool ServerInteract_Validate();
    void ServerInteract_Implementation();
    
    
    void Attack();
    UFUNCTION(Server,Reliable,WithValidation)
    void ServerAttack();
    bool ServerAttack_Validate();
    void ServerAttack_Implementation();
//    void Fire();
//    UFUNCTION(Server,Reliable,WithValidation)
//    void ServerFire();
//    bool ServerFire_Validate();
//    void ServerFire_Implementation();
    
    void Die();
    UFUNCTION(NetMulticast,Reliable,WithValidation)
    void MultiDie();
    bool MultiDie_Validate();
    void MultiDie_Implementation();
    
    FTimerHandle DestroyHandle;
    
    void CallDestroy();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
public:
    void AddHunger(float Value);
//    {
//        PlayerStatComp->AddHunger(Value);
//    }
public:
    class UPlayerStatComponent* PlayerStatComp;

public:
    virtual float TakeDamage(float Damage,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,class AActor* DamageCauser) override;
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

