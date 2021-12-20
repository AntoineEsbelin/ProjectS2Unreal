// Fill out your copyright notice in the Description page of Project Settings.


#include "Unload.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include <ProjectS2/ProjectS2Character.h>

// Sets default values
AUnload::AUnload()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OverlapsVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapVolume"));
	RootComponent = OverlapsVolume;

	OverlapsVolume->OnComponentBeginOverlap.AddUniqueDynamic(this, &AUnload::OverlapBegins);
}

// Called when the game starts or when spawned
void AUnload::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUnload::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUnload::OverlapBegins(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

	if (OtherActor == MyCharacter && LevelToLoad != "")
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
	}

}
