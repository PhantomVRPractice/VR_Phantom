// Fill out your copyright notice in the Description page of Project Settings.

#include "DKW/Enemy.h"
#include "DKW/EnemyAnim.h"
#include "DKW/EnemyBullet.h"
#include "../imitation_Phantom.h"
#include "VRCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/SceneComponent.h>
#include "DKW/EnemyFSM.h"
#include "VRPawn.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// EnemyMesh �Ҵ�
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/DKW/Enemy/Asset01/Ch15_nonPBR.Ch15_nonPBR'"));

	if (TempMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(TempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
		GetMesh()->SetRelativeScale3D(FVector(.84f));
	}

	// GunMesh
	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMeshComp"));
	gunMeshComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempGunMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/DKW/Enemy/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	
	if (TempGunMesh.Succeeded()) {
		gunMeshComp->SetSkeletalMesh(TempGunMesh.Object);
		gunMeshComp->SetupAttachment(GetMesh(), TEXT("GunPosition"));
		gunMeshComp->SetRelativeRotation(FRotator(180, 0, 0).Quaternion());


		//gunMeshComp->AttachToComponent(GetMesh(),  FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName(TEXT("GunPosition")));
		gunMeshComp->SetRelativeRotation(FRotator(180, 90, 0));
	}
	
	// Bullet
	ConstructorHelpers::FClassFinder<AEnemyBullet> TempBullet(TEXT("/Script/Engine.Blueprint'/Game/DKW/Enemy/Bp_EnemyBullet.BP_EnemyBullet_C'"));
	
	if (TempBullet.Succeeded()) {
		bulletFactory = TempBullet.Class;
	}

	// ABP
	ConstructorHelpers::FClassFinder<UEnemyAnim>TempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/DKW/Enemy/ABP_Enemy.ABP_Enemy_C'"));
	if (TempAnim.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(TempAnim.Class);
	}

	// �̵��������� ȸ���ϱ�
	GetCharacterMovement()->bOrientRotationToMovement = true;

	//FSM
	FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("FSM"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	target = Cast<AVRPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRPawn::StaticClass()));
	//pkm
	CurRotator= GetActorRotation();
	PreRotator = CurRotator;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//pkm
	if (secondtick)
	{
		PreRotator = CurRotator;
		CurRotator = GetActorRotation();
		secondtick=false;
	}
	if(firsttick){
	CurRotator=GetActorRotation();
	firsttick=false;
	secondtick=true;
	}
	/*if (bdebuglenght)
	{
		debugtime+=DeltaTime;
		if (debugtime>1)
		{
			debugtime=0;
			DrawDebugSphere(GetWorld(), GetActorLocation(), lenght, 30, FColor::Green, false, 1.0f);
		}
	}*/
	
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::OnDamage()
{
	FSM->OnDamageProcess();

#pragma region old
	//if (hp > 0) {
	//	hp--;
	//	//PRINT2SCREEN(TEXT("Enemy Damage : %d"), hp);
	//}
	//else {
	//	// animation 
	//	// ������ FSM���� ó���Ѵ�
	//	// ����ó���� ���Ŀ� ����
	//	// FSM->OnDamageProcess();

	//	// �����信�� Destroy�� �ٷ� ���⼭ �Ѵ�
	//	Destroy();
	//}
#pragma endregion
}

void AEnemy::Fire()
{
	// FirePos ������ ��ġ�� �����´�
	// FirePos ���Ͽ� Bullet �� �����Ѵ�
	FTransform firePos = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	firePos.SetRotation((target->GetActorLocation() - firePos.GetLocation()).Rotation().Quaternion());
	AEnemyBullet* EB=GetWorld()->SpawnActor<AEnemyBullet>(bulletFactory, firePos);

	// ����� ����Ʈ ����
	if (bulletSound)  UGameplayStatics::PlaySoundAtLocation(GetWorld(), bulletSound, firePos.GetLocation());
	if (bulletEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletEffect, firePos.GetLocation());
}

void AEnemy::SearchPlayer()
{
	// HandLight�� �չ��� ������ ���� ���� �����̸� 
	// HandLight�� �÷��̾� �������� ray �� ���
	// �÷��̾ �¾�����
	// FSM�� isFoundPlayer = true
	USceneComponent* handLight = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("HandLight")));
	
	if(!target){ 
		//PRINT2SCREEN(TEXT("noTarget"));
		return; 
	}
	if(!handLight){
		//PRINT2SCREEN(TEXT("noHandLight"));
		return;
	} 
	FVector A = handLight->GetForwardVector();
	FVector B = target->GetActorLocation() - GetActorLocation();

	float AngleCosine = FVector::DotProduct(A, B) / (A.Size() * B.Size());
	float AngleRadians = FMath::Acos(AngleCosine);
	float degreeOfPlayer = FMath::RadiansToDegrees(AngleRadians);

	//PRINT2SCREEN(TEXT("PlayerDegree : %f"), degreeOfPlayer);
	
	// �ӽ�
	//PRINT2SCREEN(TEXT("%f"), (target->GetActorLocation() - GetActorLocation()).Size());
	if (!target->bhide&&(target->GetActorLocation()-GetActorLocation()).Size()<=lenght)
	{
		FSM->bIsFoundPlayer = true;
	}
	if (degreeOfPlayer < 180) {
		// Enemy�� ���¸� �������� ����
		// anim�� �������ֱ�
		//FSM->mState = EEnemyState::Attack;


		//// ��򰡿� ���� �ִ°� �Ǵ��Ѵ�
		//FVector startPos = handLight->GetComponentLocation();
		//FVector targetPos = target->GetActorLocation();
		//FVector endPos = startPos + 2*(targetPos - startPos);

		//FHitResult hitInfo;
		//FCollisionQueryParams params;
		//params.AddIgnoredActor(this);

		//bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECC_Visibility, params);

		//if (bhit && hitInfo.GetActor() == target) {
		//	PRINT2SCREEN(TEXT("RayHitPlayer"));
		//	FSM->bIsFoundPlayer = true;
		//}

		//DrawDebugLine(GetWorld(), startPos, hitInfo.Location, FColor(255, 0, 0), true, -1, 0, 12.333);
		//DrawDebugLine(GetWorld(), startPos, endPos, FColor(255, 0, 0), true, -1, 0, 12.333);
	}

	// https://en.wikipedia.org/wiki/Dot_product
}
