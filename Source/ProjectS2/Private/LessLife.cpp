// Fill out your copyright notice in the Description page of Project Settings.


#include "LessLife.h"
#include <ProjectS2/ProjectS2Character.h>

// Sets default values
ALessLife::ALessLife()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void ALessLife::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->OnComponentHit.AddDynamic(this, &ALessLife::LessLife);
}

// Called every frame
void ALessLife::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALessLife::LessLife(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AProjectS2Character* Character = Cast<AProjectS2Character>(OtherActor);

	if (Character == nullptr)
		return;

	GLog->Log("MoinsDeVie");
	GLog->Log("Life : " + FString::FromInt(Character->Life));
	Character->Life = Character->Life--;
}
