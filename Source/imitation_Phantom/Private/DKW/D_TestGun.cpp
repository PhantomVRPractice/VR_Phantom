// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/D_TestGun.h"

// Sets default values
AD_TestGun::AD_TestGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_TestGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AD_TestGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

