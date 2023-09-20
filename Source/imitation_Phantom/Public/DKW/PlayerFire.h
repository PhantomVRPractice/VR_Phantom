// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerFire.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMITATION_PHANTOM_API UPlayerFire : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFire();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// Input Setting
	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions);

	UPROPERTY()
	TSubclassOf<class ABullet> bulletFactory;
	UPROPERTY(EditAnywhere, Category = "bullet")
	class USoundBase* fireSound;

	UPROPERTY()
	class AVRPawn* player;
	UPROPERTY()
	class APickUpMyGun* gun; 
	// Fire
	void Fire();
	
};
