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

	FHitResult hitInfo;

	// temp draw line func
	gun = Cast<APickUpMyGun>(UGameplayStatics::GetActorOfClass(GetWorld(), APickUpMyGun::StaticClass()));
	FVector pos = gun->GunComp->GetSocketLocation("FirePosition");

	DrawDebugLine(GetWorld(), pos, pos + gun->GunComp->GetRightVector() * 1000, FColor::Red);
	
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
	// 나중에 고치기 > graped 에서 가져오기
	gun = Cast<APickUpMyGun>(UGameplayStatics::GetActorOfClass(GetWorld(), APickUpMyGun::StaticClass()));
	
	if (!gun) {
		PRINT2SCREEN(TEXT("No Gun Here !!"));
	}
	PRINT2SCREEN(TEXT("Spawn Spawn !!"));

	// gun skeletal transform
	// 소켓확인하기
	FVector pos  = gun->GunComp->GetSocketLocation("FirePosition");
	FRotator rot = gun->GunComp->GetSocketRotation("FirePosition");

	// spawn
	if (gun->GetAmmoCount() - 1 >= 0) {
		gun->SetAmmoCount(gun->GetAmmoCount() - 1);
		PRINT2SCREEN(TEXT("Remain Ammo=%d"),gun->GetAmmoCount());
		GetWorld()->SpawnActor<ABullet>(bulletFactory, pos, rot);
	}
}

