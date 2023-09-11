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
	// 1. Line Trace�� �̿����� ��
		/*FHitResult hitInfo;
		FVector startLoc = player->rightMotionController->GetComponentLocation();
		FVector endLoc = startLoc + player->rightMotionController->GetUpVector() * -50.0f;

		if (GetWorld()->LineTraceSingleByProfile(hitInfo, startLoc, endLoc, FName("PickUp")))
		{
			APickUpActor* pickObject = Cast<APickUpActor>(hitInfo.GetActor());
			if (pickObject != nullptr)
			{
				pickObject->Grabbed(player->rightHand);
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

		// 2. Sweep ����� ����� ��
		//FHitResult hitInfo;
		//FVector startLoc = player->rightHand->GetComponentLocation();

		//if (GetWorld()->SweepSingleByProfile(hitInfo, startLoc, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(20)))
		//{
		//	if (APickUpActor* pickObject = Cast<APickUpActor>(hitInfo.GetActor()))
		//	{
		//		pickObject->Grabbed(player->rightHand);
		//	}
		//}

		// 3. Overlap ����� ����� ��
	TArray<FOverlapResult> hitInfos;
	FVector startLoc = player->rightHand->GetComponentLocation();

	if (GetWorld()->OverlapMultiByProfile(hitInfos, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(20)))
	{
		for (const FOverlapResult& hitInfo : hitInfos)
		{
			if (APickUpActor* pickObj = Cast<APickUpActor>(hitInfo.GetActor()))
			{
				pickObj->Grabbed(player->rightHand);
				grabbedObject = pickObj;
				player->pc->PlayHapticEffect(grab_Haptic, EControllerHand::Right, 1.0f, false);

				break;
			}
		}
	}

	DrawDebugSphere(GetWorld(), startLoc, 20, 30, FColor::Green, false, 1.0f);
}

void UGrabComponent::ReleaseObject()
{
	if (grabbedObject != nullptr)
	{
		// ��ü�� �տ��� �и��ϰ�, ���� �ɷ��� Ȱ��ȭ�Ѵ�.
		grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		grabbedObject->meshComp->SetSimulatePhysics(true);

		// ��ü�� ������ ���⿡ ���� ��(����, ȸ����)�� ���Ѵ�.
		if (!deltaLoc.IsNearlyZero())
		{
			grabbedObject->meshComp->AddImpulse(deltaLoc.GetSafeNormal() * throwPower);
			grabbedObject->meshComp->AddTorqueInRadians(deltaRot.GetRotationAxis() * rotSpeed);
		}

		grabbedObject = nullptr;
	}
}

