// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"
#include "EnhancedInputComponent.h"
#include "VRPawn.h"
#include "PickUpActor.h"
#include "Components/BoxComponent.h"
#include <HeadMountedDisplay/Public/MotionControllerComponent.h>
// Sets default values for this component's properties
UGrabComponent::UGrabComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	player = GetOwner<AVRPawn>();
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	if (grabbedObject!=nullptr)
	{
		deltaLoc=player->rightMotionController->GetComponentLocation()-prevLoc;
		prevLoc=player->rightMotionController->GetComponentLocation();

		deltaRot= player->rightMotionController->GetComponentQuat()-prevRot.Inverse();
		prevRot = player->rightMotionController->GetComponentQuat();
	}
}

void UGrabComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(inputActions[3], ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
}

void UGrabComponent::GrabObject()
{
#pragma region LineTrace Types
	// 1. Line Trace를 이용했을 때
	/*FHitResult hitInfo;
	FVector startLoc = player->rightMotionController->GetComponentLocation();
	FVector endLoc = startLoc + player->rightMotionController->GetUpVector() * -50.0f;

	if (GetWorld()->LineTraceSingleByProfile(hitInfo, startLoc, endLoc, FName("PickUp")))
	{
		APickUpActor* pickObject = Cast<APickUpActor>(hitInfo.GetActor());
		if (pickObject != nullptr)
		{
			pickObject->Grabbed(player->rightHand);
			grabbedObject=pickObject;
		}

		player->rightLog->SetText(FText::FromString(FString::Printf(TEXT("Grab object: %s"), *hitInfo.GetActor()->GetName())));
		UE_LOG(LogTemp, Warning, TEXT("Grab object: %s"), *hitInfo.GetActor()->GetName());
	}
	else
	{
		player->rightLog->SetText(FText::FromString(FString::Printf(TEXT("No Hit"))));
		UE_LOG(LogTemp, Warning, TEXT("No Hit"));
	}*/

#pragma endregion

	// 2. Sweep 방식을 사용할 때
	FHitResult hitInfo;
	FVector startLoc = player->rightHand->GetComponentLocation();

	if (GetWorld()->SweepSingleByProfile(hitInfo, startLoc, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(20)))
	{
		if (APickUpActor* pickObject = Cast<APickUpActor>(hitInfo.GetActor()))
		{
			pickObject->Grabbed(player->rightHand);
			grabbedObject=pickObject;
			prevLoc=player->rightMotionController->GetComponentLocation();
			prevRot=player->rightMotionController->GetComponentQuat();
		}
	}
	
}

void UGrabComponent::ReleaseObject()
{
	if (grabbedObject!=nullptr)
	{
		grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		grabbedObject->boxComp->SetSimulatePhysics(true);

		if (!deltaLoc.IsNearlyZero())
		{
			grabbedObject->boxComp->AddImpulse(deltaLoc.GetSafeNormal()*throwPower);
		}

		grabbedObject=nullptr;
	}
}

