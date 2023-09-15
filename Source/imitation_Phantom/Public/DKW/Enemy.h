// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class IMITATION_PHANTOM_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY()
	class AVRCharacter* target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEnemyFSM* FSM;

	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class USkeletalMeshComponent* gunMeshComp;

	// �Ѿ�
	TSubclassOf<class AEnemyBullet> bulletFactory;

	// Hp
	int hp = 5;
	
	void OnDamage();

	void Fire();
	void SearchPlayer();
};
