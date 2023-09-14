// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMITATION_PHANTOM_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions);

	UPROPERTY(EditDefaultsOnly,Category=MySettings)
	float throwPower=1.0f;

	UPROPERTY(EditDefaultsOnly,Category=MySettings)
	float rotSpeed=100.0f;
	UPROPERTY(EditDefaultsOnly, Category=MySettings)
	class UHapticFeedbackEffect_Base* grab_Haptic;

	UPROPERTY(EditDefaultsOnly, Category=MySettings)
	TSubclassOf<AActor> testActor;

private:
	class AVRPawn* player;
	class APickUpActor* grabbedObject;
	class APickUpActor* leftgrabbedObject;
	FVector prevLoc;
	FVector deltaLoc;
	FQuat prevRot;
	FQuat deltaRot;
	bool bgrabNo = false;
	void LeftGrabObject();
	void GrabObject();
	void LeftReleaseObject();
	void RightHandMove(const struct FInputActionValue& value);
	void LeftHandMove(const struct FInputActionValue& value);
	class AActor* spawnTestActor;
};
