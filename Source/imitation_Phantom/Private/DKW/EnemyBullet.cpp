// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/EnemyBullet.h"
#include "VRPawn.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	collisionComp->SetCollisionProfileName(TEXT("EnemyBullet"));
	collisionComp->SetSphereRadius(13);

	RootComponent = collisionComp;

	// Assign Mesh 
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bodyMeshComp->SetRelativeScale3D(FVector(0.15f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempBullet(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (tempBullet.Succeeded()) {
		bodyMeshComp->SetStaticMesh(tempBullet.Object);
	}


	// Projectile Comp
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);

	//init Projectile
	movementComp->InitialSpeed = 10000;
	movementComp->MaxSpeed = 10000;
	movementComp->bShouldBounce = false;
	movementComp->ProjectileGravityScale=0;
	// life time
	InitialLifeSpan = 10.0f;

}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnComponentBeginOverlap);
}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyBullet::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PRINT2SCREEN(TEXT("Bullet Collided"));

	AVRPawn* player = Cast<AVRPawn>(OtherActor);

	if (player) {
		PRINT2SCREEN(TEXT("Player Collided"));
		//enemy->OnDamage();
		player->Damaged(1);
	}

	Destroy();
}

void AEnemyBullet::setdirection(FVector vec)
{
	this->SetActorRotation(vec.Rotation());
	movementComp->InitialSpeed=5000;
}

