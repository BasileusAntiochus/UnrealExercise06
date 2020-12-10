// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerStatComponent.h"
#include "LineTrace.h"
#include "Pickups.h"
#include "SurvivalCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "SurvivalGameMode.h"
//////////////////////////////////////////////////////////////////////////
// ASurvivalCharacter

ASurvivalCharacter::ASurvivalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
    PlayerStatComp = CreateDefaultSubobject<UPlayerStatComponent>("PlayerStatComponent");
    LineTraceComp = CreateDefaultSubobject<ULineTrace>("LineTraceComponent");
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASurvivalCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalCharacter::MoveRight);

    
    PlayerInputComponent->BindAction("Interact",IE_Pressed,this, &ASurvivalCharacter::Interact);
    PlayerInputComponent->BindAction("Fire",IE_Pressed,this, &ASurvivalCharacter::Attack);
    //PlayerInputComponent->BindAction("Fire",IE_Pressed,this, &ASurvivalCharacter::Fire);
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASurvivalCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASurvivalCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ASurvivalCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ASurvivalCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASurvivalCharacter::OnResetVR);
}


void ASurvivalCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASurvivalCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
    PlayerStatComp->LowerHunger(1);
}

void ASurvivalCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ASurvivalCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASurvivalCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
FString ASurvivalCharacter::ReturnPlayerStats()
{
    FString RetString = "Health:" + FString::SanitizeFloat(PlayerStatComp->GetHealth());
                      
    return RetString;
}

void ASurvivalCharacter::Attack()
{
    FVector Start = GetMesh()->GetBoneLocation(FName("head"));
    FVector End = Start + FollowCamera->GetForwardVector() * 1500.0f;
    //AActor* Actor = LineTraceComp->LineTraceSingle(Start,End,true);
    FHitResult HitResult = LineTraceComp->LineTraceSingle(Start,End,true);
    if(AActor* Actor = HitResult.GetActor())
    {
        UE_LOG(LogTemp,Warning,TEXT("HIT ACTOR:%s"),*Actor->GetName());
        if(ASurvivalCharacter* Player = Cast<ASurvivalCharacter>(Actor))
        {
            //UE_LOG(LogTemp,Warning,TEXT("ACTOR IS A PICKUP"));
            ServerAttack();
        }
    }
}
bool ASurvivalCharacter::ServerAttack_Validate()
{
    return true;
}
void ASurvivalCharacter::ServerAttack_Implementation()
{
    if(HasAuthority()){
        FVector Start = GetMesh()->GetBoneLocation(FName("head"));
        FVector End = Start + FollowCamera->GetForwardVector() * 1500.0f;
        //AActor* Actor = LineTraceComp->LineTraceSingle(Start,End,true);
        FHitResult HitResult = LineTraceComp->LineTraceSingle(Start,End,true);
        if(AActor* Actor = HitResult.GetActor())
        {
            UE_LOG(LogTemp,Warning,TEXT("HIT ACTOR:%s"),*Actor->GetName());
            if(ASurvivalCharacter* Player = Cast<ASurvivalCharacter>(Actor))
            {
                //UE_LOG(LogTemp,Warning,TEXT("ACTOR IS A PICKUP"));
                float TestDamage  = 20.0f;
                
                Player->TakeDamage(TestDamage,FDamageEvent(),GetController(),this);
                //take damage
                
            }
        }
    }
}
float ASurvivalCharacter::TakeDamage(float Damage,FDamageEvent const& DamageEvent, AController* EventInstigator,AActor* DamageCauser)
{
    if(!HasAuthority()||PlayerStatComp->GetHealth()<=0.0f)
        return 0.0f;
    const float ActualDamage = Super::TakeDamage(Damage,DamageEvent,EventInstigator,DamageCauser);
    if(ActualDamage>0.0f)
    {
        PlayerStatComp->LowerHealth(ActualDamage);
        if(PlayerStatComp->GetHealth()<=0.0f)
        {
            //die
            Die();
        }
    }
    return ActualDamage;
}
void ASurvivalCharacter::Interact()
{
    FVector Start = GetMesh()->GetBoneLocation(FName("head"));
    FVector End = Start + FollowCamera->GetForwardVector() * 170.0f;
    //AActor* Actor = LineTraceComp->LineTraceSingle(Start,End,true);
    FHitResult HitResult = LineTraceComp->LineTraceSingle(Start,End,true);
    if(AActor* Actor = HitResult.GetActor())
    {
        UE_LOG(LogTemp,Warning,TEXT("HIT ACTOR:%s"),*Actor->GetName());
        if(APickups* Pickup = Cast<APickups>(Actor))
        {
            UE_LOG(LogTemp,Warning,TEXT("ACTOR IS A PICKUP"));
            ServerInteract();
        }
    }
}

bool ASurvivalCharacter::ServerInteract_Validate()
{
    return true;
}
void ASurvivalCharacter::ServerInteract_Implementation()
{
    FVector Start = GetMesh()->GetBoneLocation(FName("head"));
    FVector End = Start + FollowCamera->GetForwardVector() * 170.0f;
    FHitResult HitResult = LineTraceComp->LineTraceSingle(Start,End,true);
    if(AActor* Actor = HitResult.GetActor())
    {
        UE_LOG(LogTemp,Warning,TEXT("HIT ACTOR:%s"),*Actor->GetName());
        if(APickups* Pickup = Cast<APickups>(Actor))
        {
            UE_LOG(LogTemp,Warning,TEXT("ACTOR IS A PICKUP"));
            ServerInteract();
            Pickup->UseItem(this);
        }
    }
}

void ASurvivalCharacter::AddHunger(float Value)
{
    PlayerStatComp->AddHunger(Value);
}
void ASurvivalCharacter::CallDestroy()
{
    if(HasAuthority())
        Destroy();
}
void ASurvivalCharacter::Die()
{
    if(HasAuthority())
    {
    
        MultiDie();
        AGameModeBase* GM = GetWorld()->GetAuthGameMode();
        if(ASurvivalGameMode* GameMode = Cast<ASurvivalGameMode>(GM))
        {
            GameMode->Respawn(GetController());
        }
        //Start out destroy timer to remove actor from world
        GetWorld()->GetTimerManager().SetTimer(DestroyHandle,this,&ASurvivalCharacter::CallDestroy,1.0f,false);
    }
}

bool ASurvivalCharacter::MultiDie_Validate()
{
    return true;
}

void ASurvivalCharacter::MultiDie_Implementation()
{
    //Ragdoll
//    this->GetCharacterMovement()->DisableMovement();
//    this->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
//    this->GetMesh()->SetAllBodiesSimulatePhysics(true);
    
    this->GetMesh()->SetAllBodiesSimulatePhysics(true);
    this->GetMesh()->SetSimulatePhysics(true);
    this->GetMesh()->WakeAllRigidBodies();
    this->GetMesh()->bBlendPhysics = true;

    UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(this->GetMovementComponent());
    if (CharacterComp)
    {
        CharacterComp->StopMovementImmediately();
        CharacterComp->DisableMovement();
        CharacterComp->SetComponentTickEnabled(false);
    }

    SetLifeSpan(10.0f);
}
//void ASurvivalCharacter::Fire()
//{
//    UE_LOG(LogTemp,Warning,TEXT("Fire"));
//    //ServerFire();
//    Montage_Play(UAnimMontage("Games/ThirdPersonCPP/Blueprints/Throwing/StarThrowingMontage"));
//}


//bool ASurvivalCharacter::ServerFire_Validate()
//{
//    return true;
//}
//void ASurvivalCharacter::ServerFire_Implementation()
//{
//    UE_LOG(LogTemp,Warning,TEXT("Fire"));
//    ServerFire();
//}
