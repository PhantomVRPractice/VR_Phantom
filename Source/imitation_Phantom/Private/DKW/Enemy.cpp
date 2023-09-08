// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/Enemy.h"
#include "DKW/EnemyAnim.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "DKW/EnemyFSM.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh 할당
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT(""));

	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
		GetMesh()->SetRelativeScale3D(FVector(.84f));
	}

	// ABP 할당
	ConstructorHelpers::FClassFinder<UEnemyAnim>TempAnim(TEXT(""));
	if (TempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}

	// 이동방향으로 회전하기
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//FSM
	FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

