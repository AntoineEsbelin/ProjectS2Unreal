  // Fill out your copyright notice in the Description page of Project Settings.

#include "MyFirstActor.h"
#include <ProjectS2/ProjectS2Character.h>

// Sets default values
AMyFirstActor::AMyFirstActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	

	LetterCount = 10;
}

// Called when the game starts or when spawned
void AMyFirstActor::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->OnComponentHit.AddDynamic(this, &AMyFirstActor::OnHit);

}

// Called every frame
void AMyFirstActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyFirstActor::PrintLetterCount() {
	GLog->Log("LetterCount = " + FString::FromInt(LetterCount));
}

void AMyFirstActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AProjectS2Character* Character = Cast<AProjectS2Character>(OtherActor);
	
	if (Character == nullptr)
		return;

	Character->LaunchCharacter(FVector(0, 0, 1000), false, true);
	GLog->Log("Boink");
}

