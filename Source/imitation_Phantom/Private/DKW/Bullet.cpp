// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetSphereRadius(13);

	RootComponent = collisionComp; 

	// Assign Mesh 
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMeshComp->SetRelativeScale3D(FVector(0.25f));

	// Projectile Comp
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);

	//init Projectile
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;

	// life time
	InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

