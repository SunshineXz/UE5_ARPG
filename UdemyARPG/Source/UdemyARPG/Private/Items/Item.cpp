#include "Items/Item.h"
#include "UdemyARPG/DebugMacros.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

}

float AItem::TransformedSin() const
{
	return Amplitude * FMath::Sin(RunningTime * TimeContant);
}


float AItem::TransformedCos() const
{
	return Amplitude * FMath::Cos(RunningTime * TimeContant);
}



