// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/LobbyWidgetActor.h"
#include "Components/WidgetComponent.h"

// Sets default values
ALobbyWidgetActor::ALobbyWidgetActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(rootComp);

	screenUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("Screen UI"));
	screenUI->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALobbyWidgetActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALobbyWidgetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

