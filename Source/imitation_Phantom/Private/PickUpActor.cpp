// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"


APickUpActor::APickUpActor()
{
	PrimaryActorTick.bCanEverTick = false;
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	SetRootComponent(meshComp);
	meshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	meshComp->SetCollisionProfileName(FName("PickUp"));
	meshComp->SetSimulatePhysics(false);
	meshComp->SetEnableGravity(false);
}

void APickUpActor::BeginPlay()
{
	Super::BeginPlay();

}

void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickUpActor::Grabbed(USkeletalMeshComponent* handMesh, int32 What)
{
	if (What==1)
	{
		meshComp->SetSimulatePhysics(false);
		//AttachToComponent(handMesh, FAttachmentTransformRules::KeepWorldTransform);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//SetActorRelativeLocation(FVector(0, 0, 100))
	}
	else
	{
		meshComp->SetSimulatePhysics(false);
		//AttachToComponent(handMesh, FAttachmentTransformRules::KeepWorldTransform);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		AttachToComponent(handMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
	}
	//SetActorRelativeLocation(FVector(0, 0, 100))
}


