// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/Pistol.h"

// Sets default values
APistol::APistol()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APistol::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistol::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void  APistol::Fire()
{
	// FirePos 소켓의 위치를 가져온다
	// FirePos 소켓에 Bullet 을 스폰한다
	FTransform firePos = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<AEnemyBullet>(bulletFactory, firePos);
}
