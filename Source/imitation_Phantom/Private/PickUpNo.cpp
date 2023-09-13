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
			if (bfirstTick) // ùƽ
			{
				bfirstTick = false;
				bright=true;
				FVector mv = rightScene->GetComponentLocation() - leftScene->GetComponentLocation();
				if (mv.Z >= 0)
				{
					bright = false;
				}

				if (bright)
				{		//������ ùƽ
					CurVec = rightScene->GetComponentLocation();
				}
				else
				{		//����ùƽ
					CurVec = leftScene->GetComponentLocation();
				} 
			}
			else {
				PreVec=CurVec;
				if (bright)
				{
					//���������� ����
					PreVec = CurVec;
					CurVec = rightScene->GetComponentLocation();
					//(��ƽ���� ��ġ - ���� ���� ��ġ)�븻������+(�ݴ����) �������� add ���޽�
					FVector minusVec=PreVec-CurVec;
					minusVec.Z=0;
					double VecPower=minusVec.Size();
					minusVec.Normalize();
					//���� �÷��̾�� forward�� ��������(right�� ����)
					FVector LeftVec= -1*player->boatMesh->GetForwardVector();;
					LeftVec.Z=0;
					FVector direction= minusVec+LeftVec;
					player->boatMesh->AddForce(direction * VecPower *3000);
				}
				else
				{
					//�������� ����
					PreVec = CurVec;
					CurVec = leftScene->GetComponentLocation();
					//(��ƽ���� ��ġ - ���� ���� ��ġ)�븻������+(�ݴ����) �������� add ���޽�
					FVector minusVec = PreVec - CurVec;
					minusVec.Z = 0;
					double VecPower = minusVec.Size();
					minusVec.Normalize();
					//���� �÷��̾�� forward�� ��������(right�� ����)
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
