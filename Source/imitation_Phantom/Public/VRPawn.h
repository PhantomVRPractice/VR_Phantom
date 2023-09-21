// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

UCLASS()
class IMITATION_PHANTOM_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UStaticMeshComponent* boatMesh;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USceneComponent* NoScene;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USceneComponent* GunScene;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USceneComponent* MyAmmoScene;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UCameraComponent* hmdCam;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UStaticMeshComponent* hmdMesh;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UMotionControllerComponent* leftMotionController;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USkeletalMeshComponent* leftHand;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UTextRenderComponent* leftLog;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UMotionControllerComponent* rightMotionController;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class USkeletalMeshComponent* rightHand;

	UPROPERTY(EditAnywhere, Category="MySettings|Components")
	class UTextRenderComponent* rightLog;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class UMoveComponent* moveComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class UGrabComponent* grabComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class UNoComponent* NoComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class UPlayerFire* FireComp;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	class UNiagaraComponent* lineFx;


	UPROPERTY(EditAnywhere, Category="MySettings|Inputs")
	class UInputMappingContext* imc_VRmap;

	UPROPERTY(EditAnywhere, Category="MySettings|Inputs")
	TArray<class UInputAction*> inputActions;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Movement")
	bool bIsTesting = true;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Inputs")
	bool bIsGunMode = false;
	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Inputs")
	bool bIsNoMode = false;


	class APlayerController* pc;
	class ABallActor* ball;

	//PKMWrite
	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	TSubclassOf<class APickUpNo> NoActor;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	TSubclassOf<class APickUpMyGun> GunActor;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	TSubclassOf<class APickUpAmmo> AmmoActor;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Components")
	int32 hp=10;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Hide")
	bool bhide=false;

	//플레이어를 발견한 에너미수
	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Hide")
	int32 exposedEnemy=0;
	//발각됐니
	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Hide")
	bool bexposed=false;

	void Exposed();
	void KillExposeEnemy();

	UFUNCTION()
	void Damaged(int32 value);
	
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=UI)
	TSubclassOf<class UExposedUI> Screen_Widget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category=UI)
	class UExposedUI* Screen_UI;*/

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Screen")
	TSubclassOf<class UUserWidget> ScreenUI;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Screen")
	class UWidgetComponent* MyWidget;

	UPROPERTY(EditDefaultsOnly, Category = "MySettings|Screen")
	class UExposedUI* ExposeUI;


};
