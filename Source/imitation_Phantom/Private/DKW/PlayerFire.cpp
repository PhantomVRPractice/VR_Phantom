// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/PlayerFire.h"
#include <../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include "VRPawn.h"
#include "../imitation_Phantom.h"
#include "GrabComponent.h"
#include <Kismet/GameplayStatics.h>
#include "PickUpMyGun.h"

#define LeftClick inputActions[2]
#define RightClick inputActions[10]


// Sets default values for this component's properties
UPlayerFire::UPlayerFire()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<ABullet>tempBullet(TEXT("/Script/Engine.Blueprint'/Game/DKW/PlayerFire/BP_Bullet.BP_Bullet_C'"));
	if (tempBullet.Succeeded()) {
		bulletFactory = tempBullet.Class;
	}

	
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
}

void UPlayerFire::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(LeftClick, ETriggerEvent::Started, this, &UPlayerFire::Fire);
	enhancedInputComponent->BindAction(RightClick, ETriggerEvent::Started, this, &UPlayerFire::Fire);
}

void UPlayerFire::Fire()
{
	PRINT2SCREEN(TEXT("Player Fire !!"));
	// 총을 가지고 있지 않으면 
	if(!player->bIsGunMode) return;
	
	//gun = Cast<APickUpMyGun>(UGameplayStatics::GetActorOfClass(GetWorld(), APickUpMyGun::StaticClass()));
	gun = Cast<APickUpMyGun>(UGameplayStatics::GetActorOfClass(GetWorld(), APickUpMyGun::StaticClass()));
	
	if(!gun) return;

	// gun skeletal transform
	FVector pos = gun->GunComp->GetSocketLocation("FirePosition");
	FRotator rot = gun->GunComp->GetSocketRotation("FirePosition");

	// spawn
	GetWorld()->SpawnActor<ABullet>(bulletFactory ,pos, rot);
}

