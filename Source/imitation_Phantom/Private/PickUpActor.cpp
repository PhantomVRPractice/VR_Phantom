// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"


APickUpActor::APickUpActor()
{
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

void APickUpActor::BeginPlay()
{
	Super::BeginPlay();

}

void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpActor::Grabbed(USkeletalMeshComponent* handMesh)
{
	meshComp->SetSimulatePhysics(false);
	//AttachToComponent(handMesh, FAttachmentTransformRules::KeepWorldTransform);
	AttachToComponent(handMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
	//SetActorRelativeLocation(FVector(0, 0, 100))
}


