// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "PickUpMyGun.generated.h"

/**
 * 
 */
UCLASS()
class IMITATION_PHANTOM_API APickUpMyGun : public APickUpActor
{
	GENERATED_BODY()
public:
	APickUpMyGun();
	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class USkeletalMeshComponent* GunComp;

	UPROPERTY()
	int32 ammo = 0;

	int32 GetAmmoCount(){return ammo;}
	void SetAmmoCount(int32 ammoNum){ammo = ammoNum;}

};
