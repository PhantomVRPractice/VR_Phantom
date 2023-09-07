// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "D_Gun.generated.h"

UCLASS()
class IMITATION_PHANTOM_API AD_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AD_Gun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		UPROPERTY(EditAnywhere, Category="MySetting")
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere, Category="MySetting")
	class UStaticMeshComponent* nesg;
};
