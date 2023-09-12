// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpMyGun.h"
#include <Components/SkeletalMeshComponent.h>

APickUpMyGun::APickUpMyGun()
{
	PrimaryActorTick.bCanEverTick = false;
	GunComp = CreateDefaultSubobject<USkeletalMeshComponent>("GunComp");
	GunComp->SetupAttachment(RootComponent);
}