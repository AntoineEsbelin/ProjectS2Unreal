// Fill out your copyright notice in the Description page of Project Settings.

#include <ProjectS2/ProjectS2Character.h>
#include "Lava.h"

// Sets default values
ALava::ALava()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

}

// Called when the game starts or when spawned
void ALava::BeginPlay()
{
	Super::BeginPlay();

	MeshComponent->OnComponentHit.AddDynamic(this, &ALava::OnLava);
	
}

// Called every frame
void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALava::OnLava(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AProjectS2Character* Character = Cast<AProjectS2Character>(OtherActor);
   if (Character == NULL)
       return;
   GLog->Log("Hit !");
   GLog->Log("Life : " + FString::FromInt(Character->Life));
   Character->Life = Character->Life - 2000;
}

