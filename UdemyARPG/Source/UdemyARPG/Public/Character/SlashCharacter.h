#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CharacterTypes.h"
#include "SlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class UDEMYARPG_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:	
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);
	FString CollisionEnabledToString_Simple(ECollisionEnabled::Type CollisionType);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext * SlashContext;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction * MovementAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction * LookAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction * DodgeAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction * AttackAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction * JumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction * EquipAction;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void Equip();
	void Dodge();
	void Attack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();

	void PlayEquipMontage(FName SectionName);

	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta= (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;
	
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon * EquippedWeapon;

	/**
	 *Animation Montages
	 **/
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage * AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage * EquipMontage;
public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
