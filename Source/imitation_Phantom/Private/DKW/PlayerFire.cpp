// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/PlayerFire.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "VRPawn.h"
#include "../imitation_Phantom.h"

#define LeftClick inputActions[2]

// Sets default values for this component's properties
UPlayerFire::UPlayerFire()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = GetOwner<AVRPawn>();
}


// Called every frame
void UPlayerFire::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PRINT2SCREEN(TEXT("FireComp"));
}

void UPlayerFire::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(LeftClick, ETriggerEvent::Started, this, &UPlayerFire::Fire);
}

void UPlayerFire::Fire()
{
	// 총을 가지고 있지 않으면 
	if(!player->bIsGunMode) return;
	PRINT2SCREEN(TEXT("Player Fire !!"));
}

