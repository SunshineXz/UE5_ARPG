// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class UDEMYARPG_API AItem : public AActor
{
	GENERATED_BODY()
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameter")
	float Amplitude = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameter")
	float TimeContant = 1.f;

	UFUNCTION(BlueprintPure)
	float TransformedSin() const;

	UFUNCTION(BlueprintPure)
	float TransformedCos() const;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime = 0.f;
};
