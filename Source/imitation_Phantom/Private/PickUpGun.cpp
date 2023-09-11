// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpGun.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
APickUpGun::APickUpGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxComp->SetCollisionProfileName(FName("PickUp"));
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetWorldScale3D(FVector(0.1f, 0.1f, 0.3f));
	boxComp->SetSimulatePhysics(true);
	boxComp->SetEnableGravity(true);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APickUpGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickUpGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpGun::Grabbed(class USkeletalMeshComponent* handMesh)
{
	meshComp->SetSimulatePhysics(false);
	//AttachToComponent(handMesh, FAttachmentTransformRules::KeepWorldTransform);
	AttachToComponent(handMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
	//SetActorRelativeLocation(FVector(0, 0, 100))
}

