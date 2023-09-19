// Fill out your copyright notice in the Description page of Project Settings.


#include "GrabComponent.h"
#include "EnhancedInputComponent.h"
#include "VRPawn.h"
#include "PickUpActor.h"
#include "Components/BoxComponent.h"
#include <HeadMountedDisplay/Public/MotionControllerComponent.h>
#include "PickUpNo.h"
#include "PickUpMyGun.h"
#include "PickUpAmmo.h"
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
	spawnTestActor = GetWorld()->SpawnActor<AActor>(testActor, FVector::ZeroVector, FRotator::ZeroRotator);
}


// Called every frame
void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	if (grabbedObject!=nullptr)
	{
		if (bgrabNo)
		{//FVector shootDir = player->leftMotionController->GetComponentTransform().TransformVector(FVector::ForwardVector);
			//왼손컨트롤러랑 오른손컨트롤러 중간에 위치하도록 드랩오브젝트가
			FVector center = (player->rightMotionController->GetComponentLocation() + player->leftMotionController->GetComponentLocation())*0.5f;
			grabbedObject->SetActorLocation(center);
			//grabbedObject->SetActorRelativeLocation((player->rightMotionController->GetRelativeLocation()+ player->leftMotionController->GetRelativeLocation()/2));//엥..
			//왼손컨트롤러랑 오른손컨트롤러에 맞게 로테이션이 돌아가도록
			FVector minusvec= player->rightMotionController->GetComponentLocation() - player->leftMotionController->GetComponentLocation();
			grabbedObject->SetActorRotation(minusvec.Rotation());
		}
		if (bgrabAmmo)
		{	
			FVector startLoc = player->leftHand->GetComponentLocation();
			TArray<FOverlapResult> hitInfos;
			if (GetWorld()->OverlapMultiByProfile(hitInfos, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(20)))
			{
				for (const FOverlapResult& hitInfo : hitInfos)
				{
					if (APickUpActor* pickObj = Cast<APickUpActor>(hitInfo.GetActor()))
					{
						if (APickUpMyGun* pickGun = Cast<APickUpMyGun>(pickObj))
						{
							APickUpAmmo* GrabAmmo = Cast<APickUpAmmo>(leftgrabbedObject);
							if (GrabAmmo!=nullptr)
							{
								pickGun->SetAmmoCount(GrabAmmo->RemainAmmo);
								UE_LOG(LogTemp, Warning, TEXT("%d"),pickGun->GetAmmoCount());
								LeftReleaseObject();
								player->pc->PlayHapticEffect(grab_Haptic, EControllerHand::Left, 1.0f, false);
							}
						}
					}
				}
			}
			//DrawDebugSphere(GetWorld(), startLoc, 20, 30, FColor::Green, false, 1.0f);
		}
	}
}

void UGrabComponent::SetupPlayerInputComponent(class UEnhancedInputComponent* enhancedInputComponent, TArray<class UInputAction*> inputActions)
{
	enhancedInputComponent->BindAction(inputActions[3], ETriggerEvent::Started, this, &UGrabComponent::GrabObject);
	enhancedInputComponent->BindAction(inputActions[4], ETriggerEvent::Triggered, this, &UGrabComponent::RightHandMove);
	enhancedInputComponent->BindAction(inputActions[7], ETriggerEvent::Started, this, &UGrabComponent::LeftGrabObject);
	enhancedInputComponent->BindAction(inputActions[7], ETriggerEvent::Completed, this, &UGrabComponent::LeftReleaseObject);
	enhancedInputComponent->BindAction(inputActions[9], ETriggerEvent::Triggered, this, &UGrabComponent::LeftHandMove);
}

void UGrabComponent::LeftGrabObject()
{
	TArray<FOverlapResult> hitInfos;
	FVector startLoc = player->leftHand->GetComponentLocation();

	if (GetWorld()->OverlapMultiByProfile(hitInfos, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(20)))
	{
		for (const FOverlapResult& hitInfo : hitInfos)
		{
			if (APickUpActor* pickObj = Cast<APickUpActor>(hitInfo.GetActor()))
			{
				if (APickUpAmmo* pickAmmo = Cast<APickUpAmmo>(pickObj))
				{
					bgrabAmmo=true;
					pickAmmo->Grabbed(player->leftHand, 2);
					leftgrabbedObject = pickObj;
					player->pc->PlayHapticEffect(grab_Haptic, EControllerHand::Left, 1.0f, false);
					break;
				}
			}
		}
	}
//	DrawDebugSphere(GetWorld(), startLoc, 20, 30, FColor::Green, false, 1.0f);
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
		//FHitResult hitInfo;
		//FVector startLoc = player->rightHand->GetComponentLocation();

		//if (GetWorld()->SweepSingleByProfile(hitInfo, startLoc, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(20)))
		//{
		//	if (APickUpActor* pickObject = Cast<APickUpActor>(hitInfo.GetActor()))
		//	{
		//		pickObject->Grabbed(player->rightHand);
		//	}
		//}

		// 3. Overlap 방식을 사용할 때
	if (grabbedObject!=nullptr)
	{		//Release부분
	
			if (APickUpNo* GrabNo = Cast<APickUpNo>(grabbedObject))
			{
				player->bIsNoMode=false;
				bgrabNo = false;
				GrabNo->player = nullptr;
				GrabNo->AttachToComponent(player->NoScene, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
			if (APickUpMyGun* GrabGun = Cast<APickUpMyGun>(grabbedObject))
			{	
				player->bIsGunMode=false;
				grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				GrabGun->AttachToComponent(player->GunScene, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				UE_LOG(LogTemp, Warning, TEXT("GunReleasezz"));
			}
			//// 물체의 던지는 방향에 따른 힘(선형, 회전력)을 가한다.
			//if (!deltaLoc.IsNearlyZero())
			//{
			//	grabbedObject->meshComp->AddImpulse(deltaLoc.GetSafeNormal() * throwPower);
			//	grabbedObject->meshComp->AddTorqueInRadians(deltaRot.GetRotationAxis() * rotSpeed);
			//}
			grabbedObject = nullptr;
	}
	else
	{
		TArray<FOverlapResult> hitInfos;
		FVector startLoc = player->rightHand->GetComponentLocation();

		if (GetWorld()->OverlapMultiByProfile(hitInfos, startLoc, FQuat::Identity, FName("PickUp"), FCollisionShape::MakeSphere(20)))
		{
			for (const FOverlapResult& hitInfo : hitInfos)
			{
				if (APickUpActor* pickObj = Cast<APickUpActor>(hitInfo.GetActor()))
				{
					if (APickUpNo* pickNo = Cast<APickUpNo>(pickObj))
					{
						player->bIsNoMode=true;
						pickNo->Grabbed(player->rightHand, 1);
						pickNo->player=player;
						bgrabNo = true;
						grabbedObject = pickObj;
						player->pc->PlayHapticEffect(grab_Haptic, EControllerHand::Right, 1.0f, false);
						break;
					}
					if (APickUpMyGun* pickGun = Cast<APickUpMyGun>(pickObj))
					{
						player->bIsGunMode=true;
						pickGun->Grabbed(player->rightHand, 2);
						grabbedObject = pickObj;
						player->pc->PlayHapticEffect(grab_Haptic, EControllerHand::Right, 1.0f, false);
						break;
					}
				}
			}
		}
		//DrawDebugSphere(GetWorld(), startLoc, 20, 30, FColor::Green, false, 1.0f);
	}
}

void UGrabComponent::LeftReleaseObject()
{
		if (leftgrabbedObject != nullptr)
		{
		// 물체를 손에서 분리하고, 물리 능력을 활성화한다.
			bgrabAmmo=false;
			leftgrabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			if (APickUpAmmo* GrabAmmo = Cast<APickUpAmmo>(leftgrabbedObject))
			{
				GrabAmmo->AttachToComponent(player->MyAmmoScene, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			}
			leftgrabbedObject = nullptr;
	}
}

//void UGrabComponent::ReleaseObject()
//{
//	if (grabbedObject != nullptr)
//	{
//		// 물체를 손에서 분리하고, 물리 능력을 활성화한다.
//		grabbedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//		if (APickUpNo* GrabNo = Cast<APickUpNo>(grabbedObject))
//		{
//			GrabNo->AttachToComponent(player->NoScene, FAttachmentTransformRules::KeepRelativeTransform);
//		}
//		if (APickUpMyGun* GrabGun = Cast<APickUpMyGun>(grabbedObject))
//		{
//			GrabGun->AttachToComponent(player->GunScene, FAttachmentTransformRules::KeepRelativeTransform);
//		}
//		//// 물체의 던지는 방향에 따른 힘(선형, 회전력)을 가한다.
//		//if (!deltaLoc.IsNearlyZero())
//		//{
//		//	grabbedObject->meshComp->AddImpulse(deltaLoc.GetSafeNormal() * throwPower);
//		//	grabbedObject->meshComp->AddTorqueInRadians(deltaRot.GetRotationAxis() * rotSpeed);
//		//}
//
//		grabbedObject = nullptr;
//	}
//}


void UGrabComponent::RightHandMove(const FInputActionValue& value)
{
	FVector direction = value.Get<FVector>();
	player->rightMotionController->SetRelativeLocation(player->rightMotionController->GetRelativeLocation() + direction.GetSafeNormal());
}

void UGrabComponent::LeftHandMove(const struct FInputActionValue& value)
{
	FVector direction = value.Get<FVector>();
	player->leftMotionController->SetRelativeLocation(player->leftMotionController->GetRelativeLocation() + direction.GetSafeNormal());
}

