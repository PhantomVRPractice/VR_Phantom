// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpNo.h"
#include "VRPawn.h"

APickUpNo::APickUpNo()
{
	PrimaryActorTick.bCanEverTick = true;
	rightScene = CreateDefaultSubobject<USceneComponent>(TEXT("Right Scene"));
	leftScene = CreateDefaultSubobject<USceneComponent>(TEXT("Left Scene"));

	rightScene->SetupAttachment(RootComponent);
	leftScene->SetupAttachment(RootComponent);

}

void APickUpNo::BeginPlay()
{
	Super::BeginPlay();
	meshComp->OnComponentBeginOverlap.AddDynamic(this, &APickUpNo::OnOverlapBegin);
	meshComp->OnComponentEndOverlap.AddDynamic(this, &APickUpNo::OnOverlapEnd);
}

void APickUpNo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (binwater)
	{
		if (player!=nullptr)
		{
			if (bfirstTick) // 첫틱
			{
				bfirstTick = false;
				bright=true;
				FVector mv = rightScene->GetComponentLocation() - leftScene->GetComponentLocation();
				if (mv.Z >= 0)
				{
					bright = false;
				}

				if (bright)
				{		//오른쪽 첫틱
					CurVec = rightScene->GetComponentLocation();
				}
				else
				{		//왼쪽첫틱
					CurVec = leftScene->GetComponentLocation();
				} 
			}
			else {
				PreVec=CurVec;
				if (bright)
				{
					//오른쪽으로 젓기
					PreVec = CurVec;
					CurVec = rightScene->GetComponentLocation();
					//(전틱씬콤 위치 - 현재 씬콤 위치)노말라이즈+(반대방향) 방향으로 add 임펄스
					FVector minusVec=PreVec-CurVec;
					minusVec.Z=0;
					double VecPower=minusVec.Size();
					minusVec.Normalize();
					//지금 플레이어는 forward가 오른쪽임(right는 뒤쪽)
					FVector LeftVec= -1*player->boatMesh->GetForwardVector();;
					LeftVec.Z=0;
					FVector direction= minusVec+LeftVec;
					player->boatMesh->AddForce(direction * VecPower *3000);
				}
				else
				{
					//왼쪽으로 젓기
					PreVec = CurVec;
					CurVec = leftScene->GetComponentLocation();
					//(전틱씬콤 위치 - 현재 씬콤 위치)노말라이즈+(반대방향) 방향으로 add 임펄스
					FVector minusVec = PreVec - CurVec;
					minusVec.Z = 0;
					double VecPower = minusVec.Size();
					minusVec.Normalize();
					//지금 플레이어는 forward가 오른쪽임(right는 뒤쪽)
					FVector RightVec = player->boatMesh->GetForwardVector();;
					RightVec.Z = 0;
					FVector direction = minusVec + RightVec;
					player->boatMesh->AddForce(direction * VecPower * 3000);
				}
			}
		}
	}
}

void APickUpNo::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("overstart"));
	binwater=true;
	bfirstTick=true;
}

void APickUpNo::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("overend"));
	binwater=false;
}
