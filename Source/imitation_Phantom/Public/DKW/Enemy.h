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
	class AVRPawn* target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEnemyFSM* FSM;

	UPROPERTY(EditAnywhere, Category = "MySettings")
	class USkeletalMeshComponent* gunMeshComp;

	// ÃÑ¾Ë
	TSubclassOf<class AEnemyBullet> bulletFactory;

	UPROPERTY(EditAnywhere, Category = "MySetting")
	class USoundBase* bulletSound;

	UPROPERTY(EditAnywhere, Category = "MySetting")
	class UNiagaraSystem* bulletEffect;


	UPROPERTY(EditAnywhere, Category = "MySetting")
	int32 initialHP = 5;
	UPROPERTY(EditAnywhere, Category = "MySetting")
	int32 hp = 5;
	bool firsttick=false;

	bool secondtick = false;
	void OnDamage();

	void Fire();
	void SearchPlayer();
	

	//PKM Write with Anim
	UPROPERTY(BlueprintReadWrite)
	bool turnfirsttick=true;
	UPROPERTY(BlueprintReadWrite)
	FRotator PreRotator;
	UPROPERTY(BlueprintReadWrite)
	FRotator CurRotator;
};
