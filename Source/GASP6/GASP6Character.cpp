// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASP6Character.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Components/Movement/ComponentMovement.h"
#include "Components/Combat/Lockon/ComponentLockon.h"
#include "Components/Combat/Guard/ComponentGuard.h"
#include "Abilities/Combat/HandleGetHit/AbilityHandleGetHit.h"
#include "Attribute/Health/AttributeHealth.h"
#include "Abilities/Combat/HandleGetHit/AbilityDisablePlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGASP6Character

AGASP6Character::AGASP6Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;			 // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 230.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;		// The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;								// Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	// TODO: Maby cache the components
	this->AbilitySystemComponent = this->CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("MyASC"));
	this->CreateDefaultSubobject<UComponentMovement>(TEXT("MyMovementComponent"));
	this->myLockonComponent = this->CreateDefaultSubobject<UComponentLockon>(TEXT("myLockonComponent"));
	this->myGuardComponent = this->CreateDefaultSubobject<UComponentGuard>(TEXT("MyGuardComponent"));
	// This ability is triggered exclusively via event so shouldn't be contained in any component
	// TODO: Maybe cache this ability
	this->Health = this->CreateDefaultSubobject<UAttributeHealth>(TEXT("MyHealthAttribute"));
	this->AbilitySystemComponent->AddAttributeSetSubobject<UAttributeHealth>(this->Health);
	this->Health->OnHealthChangeEvent.AddDynamic(this, &AGASP6Character::CheckNegativeHealth);
}

void AGASP6Character::BeginPlay()
{
	Super::BeginPlay();
	this->AbilitySystemComponent->K2_GiveAbility(UAbilityHandleGetHit::StaticClass());
	FGameplayAbilitySpecHandle abilityHandle = this->AbilitySystemComponent->K2_GiveAbility(UAbilityDisablePlayer::StaticClass());
	FGameplayAbilitySpec *spec = this->AbilitySystemComponent->FindAbilitySpecFromHandle(abilityHandle);
	UAbilityDisablePlayer *abi = Cast<UAbilityDisablePlayer>(spec->GetPrimaryInstance());
	abi->NotifyPlayerDown.BindLambda([this](bool down){this->IsPlayerDown = down; });
	// if(abi)
	// {
	// }
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGASP6Character::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGASP6Character::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AGASP6Character::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGASP6Character::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGASP6Character::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AGASP6Character::Move(const FInputActionValue &Value)
{
	// input is a Vector2D
	if (this->IsPlayerDown)
	{
		return;
	}
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGASP6Character::Look(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (this->myLockonComponent->Target)
	{
		return;
	}
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

UAbilitySystemComponent *AGASP6Character::GetAbilitySystemComponent() const
{
	return &(*AbilitySystemComponent);
}

void AGASP6Character::CheckNegativeHealth(float percentage)
{
	if (percentage <= 0.0f)
	{
		this->Destroy();
	}
}

void AGASP6Character::Jump()
{
	if (!this->IsPlayerDown)
	{
		Super::Jump();
	}
}