// Fill out your copyright notice in the Description page of Project Settings.


#include "D_Gun.h"

// Sets default values
AD_Gun::AD_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AD_Gun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AD_Gun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

