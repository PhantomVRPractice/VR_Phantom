// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "PickUpAmmo.generated.h"

/**
 * 
 */
UCLASS()
class IMITATION_PHANTOM_API APickUpAmmo : public APickUpActor
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 RemainAmmo=30;
};
