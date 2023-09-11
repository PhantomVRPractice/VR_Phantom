// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpGun.generated.h"

UCLASS()
class IMITATION_PHANTOM_API APickUpGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UStaticMeshComponent* meshComp;

	UFUNCTION()
	void Grabbed(class USkeletalMeshComponent* handMesh);
};
