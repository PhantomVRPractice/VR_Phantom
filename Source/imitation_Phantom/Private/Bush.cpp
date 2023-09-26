// Fill out your copyright notice in the Description page of Project Settings.


#include "Bush.h"
#include "VRPawn.h"
#include "ExposedUI.h"
#include <UMG/Public/Components/TextBlock.h>
#include <Kismet/GameplayStatics.h>
#include "Materials/Material.h"
// Sets default values
ABush::ABush()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bushMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere Mesh"));
	bushMesh->SetupAttachment(RootComponent);
	bushSMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bush SSMesh"));
	bushSMesh->SetupAttachment(bushMesh);
}

// Called when the game starts or when spawned
void ABush::BeginPlay()
{
	Super::BeginPlay();
	bushMesh->OnComponentBeginOverlap.AddDynamic(this, &ABush::OnOverlapBegin);
	bushMesh->OnComponentEndOverlap.AddDynamic(this, &ABush::OnOverlapEnd);
}

// Called every frame
void ABush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABush::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVRPawn* player=Cast<AVRPawn>(OtherActor);
	if (player!=nullptr)
	{
		player->bhide=true;
		if (InVisibleMat!=nullptr)
		{
			bushSMesh->SetMaterial(0, InVisibleMat);
		}
		
		UE_LOG(LogTemp,Warning,TEXT("player hide"));
		if (BushSoundFactory != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BushSoundFactory, player->GetActorLocation());
		}
		if (!player->bexposed)
		{
			player->ExposeUI->screenText->SetText(FText::FromString(FString(TEXT("Hidden"))));
			player->ExposeUI->screenText->SetColorAndOpacity(FSlateColor(FLinearColor(0, 0, 1, 1)));
		}
	}
}

void ABush::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AVRPawn* player = Cast<AVRPawn>(OtherActor);
	if (player != nullptr)
	{
		player->bhide = false;
		if (VisibleMat != nullptr)
		{
			bushSMesh->SetMaterial(0, VisibleMat);
		}
		UE_LOG(LogTemp, Warning, TEXT("player open"));
		if (BushSoundFactory != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), OutSoundFactory, player->GetActorLocation());
		}
		if (!player->bexposed)
		{
			player->ExposeUI->screenText->SetText(FText::FromString(FString(TEXT("Exposed"))));
			player->ExposeUI->screenText->SetColorAndOpacity(FSlateColor(FLinearColor(0, 1, 0, 1)));
		}
	}
}

