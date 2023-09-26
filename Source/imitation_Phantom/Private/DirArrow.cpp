// Fill out your copyright notice in the Description page of Project Settings.


#include "DirArrow.h"

// Sets default values
ADirArrow::ADirArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Mesh"));
	SetRootComponent(ArrowMesh);
}

// Called when the game starts or when spawned
void ADirArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADirArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

