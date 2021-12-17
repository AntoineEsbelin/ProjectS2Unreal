// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Splatoon.generated.h"

UCLASS()
class PROJECTS2_API ASplatoon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplatoon();

	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere)
		class UMaterialInterface* DecalMat = nullptr;

	UPROPERTY()
		class UStaticMesh* Asset;

	UPROPERTY()
		class UProjectileMovementComponent* ProjectileMov;

	UPROPERTY()
		FVector velocity = FVector(3000, 0, 0);

	UPROPERTY()
		float initialSpeed;

	UPROPERTY()
		float maxSpeed;

	UPROPERTY(EditAnywhere, Category = "Materials")
		bool bChooseOne = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
