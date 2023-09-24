// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingDoor.h"
#include <Components/CapsuleComponent.h>

// Sets default values
AMovingDoor::AMovingDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapRoot"));
	SetRootComponent(CapsuleComp);
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	SwitchComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	meshComp->SetupAttachment(RootComponent);
	SwitchComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMovingDoor::BeginPlay()
{
	Super::BeginPlay();
	SwitchComp->OnComponentBeginOverlap.AddDynamic(this, &AMovingDoor::OnOverlapBegin);
	Startloc=meshComp->GetRelativeLocation();
	Endloc=Startloc;
	Endloc.Z+=Zdist;
}

// Called every frame
void AMovingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (boverlap)
	{
		curtime+=DeltaTime;
		if (curtime<movingtime)
		{ 
			FVector NewLoc = Startloc;
			NewLoc.Z += curtime * (Zdist / movingtime);
			meshComp->SetRelativeLocation(NewLoc);
		}
		else
		{
			meshComp->SetRelativeLocation(Endloc);
			boverlap=false;
			curtime=0;
		}
	}
}

void AMovingDoor::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//문움직여라
	UE_LOG(LogTemp,Warning,TEXT("OverlapDoor"));
	boverlap=true;
}

