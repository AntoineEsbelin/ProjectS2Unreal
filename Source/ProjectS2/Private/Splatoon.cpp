// Fill out your copyright notice in the Description page of Project Settings.


#include "Splatoon.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <ProjectS2/ProjectS2Character.h>

// Sets default values
ASplatoon::ASplatoon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Zinedine Zidane"));
    RootComponent = MeshComponent;


    DecalMat = CreateDefaultSubobject<UMaterialInterface>("Paint");



    // Chercher le mesh .
    auto Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
    Asset = Mesh.Object;
    ProjectileMov = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Mov op"));
    // Caractéristiques du projectiles ( velocité tout ça tavu ) 
    ProjectileMov->Velocity = velocity;
    ProjectileMov->InitialSpeed = initialSpeed;
    ProjectileMov->MaxSpeed = maxSpeed;
    // Je set et match
    MeshComponent->SetStaticMesh(Asset);
    MeshComponent->SetWorldScale3D(FVector(0.2f));
    MeshComponent->SetSimulatePhysics(true);

    MeshComponent->SetGenerateOverlapEvents(true);
    MeshComponent->SetCollisionProfileName(FName("PhysicsActor"));
}

// Called when the game starts or when spawned
void ASplatoon::BeginPlay()
{
	Super::BeginPlay();
    // Envie de me suicider que ce truc fasse crash unreal
    // MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ASplatoon::OnBeginOverlap);
	
}

// Called every frame
void ASplatoon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASplatoon::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->ActorHasTag(TEXT("OnTheSurface")))
    {
        GLog->Log("Hit a surface");
        FVector BulletLocation = this->GetActorLocation();
        FRotator BulletRotation = this->GetActorRotation();
        FActorSpawnParameters BulletParameters;
        FRotator DecalRotation = UKismetMathLibrary::NormalizedDeltaRotator(BulletRotation, FRotator(0.f, 0.0f, -90.0f));
        ADecalActor* Decal = GetWorld()->SpawnActor<ADecalActor>(BulletLocation, DecalRotation, BulletParameters);
        if (Decal)
        {
            Decal->SetDecalMaterial(DecalMat);
            Decal->GetDecal()->DecalSize = FVector(192.0f, 100.0f, 100.0f);
            Decal->SetLifeSpan(0.0);

        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Snif :'( "));
        }
    }
    Destroy();
}

