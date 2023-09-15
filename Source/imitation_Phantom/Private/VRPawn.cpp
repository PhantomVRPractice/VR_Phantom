// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MotionControllerComponent.h"
#include "Components/TextRenderComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MoveComponent.h"
#include "GrabComponent.h"
#include "NoComponent.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "PickUpNo.h"
#include "PickUpMyGun.h"
#include "DKW/PlayerFire.h"
#include "PickUpAmmo.h"
// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boat Mesh"));
	RootComponent=boatMesh;
	hmdCam = CreateDefaultSubobject<UCameraComponent>(TEXT("HMD Camera"));
	hmdCam->SetupAttachment(RootComponent);

	hmdMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HMD Mesh"));
	hmdMesh->SetupAttachment(hmdCam);

	leftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Motion Controller"));
	leftMotionController->SetupAttachment(RootComponent);
	leftMotionController->SetTrackingMotionSource(FName("Left"));

	leftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand Mesh"));
	leftHand->SetupAttachment(leftMotionController);

	leftLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Left Log"));
	leftLog->SetupAttachment(leftMotionController);
	leftLog->SetRelativeLocation(FVector(0, 0, 15));
	leftLog->SetRelativeRotation(FRotator(0, 180, 0));
	leftLog->SetWorldSize(10);
	leftLog->SetTextRenderColor(FColor(255, 255, 0, 255));
	leftLog->SetHorizontalAlignment(EHTA_Center);
	leftLog->SetVerticalAlignment(EVRTA_TextCenter);

	rightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Motion Controller"));
	rightMotionController->SetupAttachment(RootComponent);
	rightMotionController->SetTrackingMotionSource(FName("Right"));

	rightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand Mesh"));
	rightHand->SetupAttachment(rightMotionController);

	rightLog = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Right Log"));
	rightLog->SetupAttachment(rightMotionController);
	rightLog->SetWorldSize(10);
	rightLog->SetTextRenderColor(FColor(255, 255, 0, 255));
	rightLog->SetHorizontalAlignment(EHTA_Center);
	rightLog->SetVerticalAlignment(EVRTA_TextCenter);

	lineFx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Line Effect"));
	lineFx->SetupAttachment(RootComponent);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;
	// 컴포넌트 패턴
	moveComp = CreateDefaultSubobject<UMoveComponent>(TEXT("Move Component"));
	grabComp = CreateDefaultSubobject<UGrabComponent>(TEXT("Grab Component"));
	NoComp = CreateDefaultSubobject<UNoComponent>(TEXT("No Component"));
	FireComp = CreateDefaultSubobject<UPlayerFire>(TEXT("Fire Component"));

	NoScene=CreateDefaultSubobject<USceneComponent>(TEXT("No Scene"));
	NoScene->SetupAttachment(boatMesh);
	GunScene = CreateDefaultSubobject<USceneComponent>(TEXT("Gun Scene"));
	GunScene->SetupAttachment(boatMesh);
	MyAmmoScene = CreateDefaultSubobject<USceneComponent>(TEXT("MyAmmo Scene"));
	MyAmmoScene->SetupAttachment(boatMesh);

}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	leftLog->SetText(FText::FromString("Left Log..."));
	rightLog->SetText(FText::FromString("Right Log..."));

	// 머리 장비 기준점 설정
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

	pc = GetController<APlayerController>();

	if (pc != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		if (subSys != nullptr)
		{
			subSys->AddMappingContext(imc_VRmap, 0);
		}
	}
	APickUpNo* PickUpNo=GetWorld()->SpawnActor<APickUpNo>(NoActor);
	if (PickUpNo!=nullptr)
	{
		PickUpNo->AttachToComponent(NoScene,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	APickUpMyGun* PickUpGun = GetWorld()->SpawnActor<APickUpMyGun>(GunActor);
	if (PickUpGun != nullptr)
	{
		PickUpGun->AttachToComponent(GunScene, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
	APickUpAmmo* PickUpAmmo = GetWorld()->SpawnActor<APickUpAmmo>(AmmoActor);
	if (PickUpAmmo != nullptr)
	{
		if (PickUpAmmo->AttachToComponent(MyAmmoScene, FAttachmentTransformRules::SnapToTargetNotIncludingScale))
		{
			UE_LOG(LogTemp, Warning, TEXT("Sorry"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Sorry2"));

		}
		
	}
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (enhancedInputComponent != nullptr)
	{
		moveComp->SetupPlayerInputComponent(enhancedInputComponent, inputActions);
		grabComp->SetupPlayerInputComponent(enhancedInputComponent, inputActions);
		FireComp->SetupPlayerInputComponent(enhancedInputComponent, inputActions);
	}
}

void AVRPawn::Damaged(int32 value)
{
	if (hp-value > 0) {
		hp-=value;
		PRINT2SCREEN(TEXT("player hp: %d"), hp);
	}
	else {
		hp=0;
		//GetController()->UnPossess();
	}
}

