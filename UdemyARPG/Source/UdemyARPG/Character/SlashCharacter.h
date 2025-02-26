#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SlashCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;

UCLASS()
class UDEMYARPG_API ASlashCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:	
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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


private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;
	
	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;
};
