// Fill out your copyright notice in the Description page of Project Settings.


#include "Bush.h"
#include "VRPawn.h"

// Sets default values
ABush::ABush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bushMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boat Mesh"));
	RootComponent = bushMesh;
}

// Called when the game starts or when spawned
void ABush::BeginPlay()
{
	Super::BeginPlay();
	bushMesh->OnComponentBeginOverlap.AddDynamic(this, &ABush::OnOverlapBegin);
	bushMesh->OnComponentEndOverlap.AddDynamic(this, &ABush::OnOverlapEnd);
}

// Called every frame
void ABush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABush::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVRPawn* player=Cast<AVRPawn>(OtherActor);
	if (player!=nullptr)
	{
		player->bhide=true;
		UE_LOG(LogTemp,Warning,TEXT("player hide"));
	}
}

void ABush::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVRPawn* player = Cast<AVRPawn>(OtherActor);
	if (player != nullptr)
	{
		player->bhide = false;
		UE_LOG(LogTemp, Warning, TEXT("player open"));
	}
}

