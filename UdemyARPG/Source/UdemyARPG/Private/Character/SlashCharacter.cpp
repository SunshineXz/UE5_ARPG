#include "Character/SlashCharacter.h"

#include <string>

#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

ASlashCharacter::ASlashCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(SpringArm);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f,0.f);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if(Subsystem)
		{
			Subsystem->AddMappingContext(SlashContext, 0);
		}
	}
}

void ASlashCharacter::Move(const FInputActionValue& Value)
{
	if(ActionState != EActionState::EAS_Unoccupied)
		return;
	
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);
	
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASlashCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void ASlashCharacter::Jump()
{
	Super::Jump();
}

void ASlashCharacter::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if(OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"));
		CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if(CanDisarm())
		{
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if(CanArm())
		{
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedTwoHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}

void ASlashCharacter::Dodge()
{
}

void ASlashCharacter::Attack()
{
	if(ActionState != EActionState::EAS_Unoccupied || CharacterState == ECharacterState::ECS_Unequipped)
		return;

	ActionState = EActionState::EAS_Attacking;
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 RandomNumber = FMath::RandRange(1, 2);
		FString RandomNumberString = FString::FromInt(RandomNumber);
		FString AttackString = TEXT("Attack") + RandomNumberString;
		AnimInstance->Montage_JumpToSection(FName(AttackString), AttackMontage); 
	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ASlashCharacter::Disarm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::Arm()
{
	if(EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Attack);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Equip);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &ASlashCharacter::Dodge);

	}
}

void ASlashCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if(EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

FString ASlashCharacter::CollisionEnabledToString_Simple(ECollisionEnabled::Type CollisionType)
{
	switch (CollisionType)
	{
	case ECollisionEnabled::NoCollision: return TEXT("NoCollision");
	case ECollisionEnabled::QueryOnly: return TEXT("QueryOnly");
	case ECollisionEnabled::PhysicsOnly: return TEXT("PhysicsOnly");
	case ECollisionEnabled::QueryAndPhysics: return TEXT("QueryAndPhysics");
	default: return TEXT("Unknown");
	}
}

