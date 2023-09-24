// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingDoor.generated.h"

UCLASS()
class IMITATION_PHANTOM_API AMovingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USceneComponent* SwitchScene;

		UPROPERTY(EditDefaultsOnly, Category="MySettings|Components")
	class UStaticMeshComponent* SwitchComp;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	FVector Startloc;
	FVector Endloc;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category="MySettings|dist")
	float Zdist=1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings|dist")
	float movingtime=10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MySettings|dist")
	float switchtime=1;
	float curtime=0;
	bool boverlap=false;
};
