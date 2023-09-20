// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/Bullet.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "../imitation_Phantom.h"
#include "DKW/Enemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("PlayerBullet"));
	collisionComp->SetSphereRadius(13);

	RootComponent = collisionComp; 

	// Assign Mesh 
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMeshComp->SetRelativeScale3D(FVector(0.1f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempBullet (TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (tempBullet.Succeeded()) {
		bodyMeshComp->SetStaticMesh(tempBullet.Object);
	}

	// Projectile Comp
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);

	//init Projectile
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 10000;
	movementComp->bShouldBounce = true;
	movementComp->ProjectileGravityScale=0;
	// life time
	InitialLifeSpan = 2.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnComponentBeginOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABullet::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PRINT2SCREEN(TEXT("Bullet Collided"));

	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	if (enemy) {
		PRINT2SCREEN(TEXT("Enemy Collided"));
		enemy->OnDamage();
	}
	Destroy();

	if (bulletEffectFactory) {
		FTransform trans;
		trans.SetLocation(SweepResult.ImpactPoint);
		trans.SetRotation(SweepResult.ImpactNormal.ToOrientationQuat());
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletEffectFactory, trans);
	}
}

