// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "PickUpNo.generated.h"

/**
 * 
 */
UCLASS()
class IMITATION_PHANTOM_API APickUpNo : public APickUpActor
{
	GENERATED_BODY()
public:
	APickUpNo();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USceneComponent* rightScene;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USceneComponent* leftScene;
	UPROPERTY(EditAnywhere, Category="MySettings|player")
	class AVRPawn* player;
	bool binwater=false;
	bool bfirstTick = true;
	bool bright = true;
	bool bsoundplay=false;
	UPROPERTY(EditAnywhere, Category="MyAction")
	FVector PreVec;

	UPROPERTY(EditAnywhere, Category="MyAction")
	FVector CurVec;

	UPROPERTY(EditAnywhere, Category="MySound")
	USoundBase* NoSound;


	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
