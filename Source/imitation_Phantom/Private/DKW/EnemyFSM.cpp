// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "VRCharacter.h"
#include "DKW/EnemyAnim.h"
#include "DKW/Enemy.h"
#include <AIModule/Classes/AIController.h>
#include <NavigationSystem/Public/NavigationSystem.h>
#include <Components/CapsuleComponent.h>
#include "../imitation_Phantom.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "VRPawn.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// target
	// me
	// EnemyAnim
	target = Cast<AVRPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRPawn::StaticClass()));
	me	   = Cast<AEnemy>(GetOwner());
	anim   = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	
	// AIController
	ai = Cast<AAIController>(me->GetController());
	me->GetCharacterMovement()->MaxWalkSpeed = speed;

	// ������ġ ����
	//refPositions.Add("StartPos", me);
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ���ݹ��� �ð�ȭ 
	if (bDebugAttackRange) {
		DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 20, FColor::Red, false, -1, 0, 2);
	}

	// ���� ���� ���
	FString strState;
	UEnum::GetValueAsString(mState, strState);
	PRINT2SCREEN(TEXT("%s"), *strState);
	PRINT2SCREEN(TEXT("hp : %d"), me->hp);


	// FSM ����
	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Search:
		SearchState();
		break;
	case EEnemyState::RadioReport:
		RadioReportState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	}
}

// �����ð��� ������ ���¸� �̵����� �ٲٰ� �ʹ�
// �ʿ�Ӽ� : ���ð�, ����ð�
void UEnemyFSM::IdleState()
{
	if (isEnemyActive) {
		currentTime += GetWorld()->DeltaTimeSeconds;
		if (currentTime > idleDelayTime)
		{
			mState = EEnemyState::Move;
			currentTime = 0;
			anim->animState = mState;
			curDestinationName = "SearchPos";
		}
	}

	// ���⼭ �÷��̾� �Ҹ��� �鸮�� ���ݻ��·� ��ȯ
	// ����̿��� �ֳ�?


}

// Ÿ������ �̵��ϰ� �ʹ�
// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
void UEnemyFSM::MoveState()
{
	if(!refPositions[curDestinationName])
	{
		//PRINT2SCREEN(TEXT("No CurDestination Pos"));
		return;
	}
	FVector dest = refPositions[curDestinationName]->GetActorLocation();

	EPathFollowingRequestResult::Type isAlreadyGoal = EPathFollowingRequestResult::Failed;
	
	FPathFindingResult r;
	FindPathByAI(dest, r);
	// Ÿ���������̵��� �� ������ �ű�� ����
	if (r.Result == ENavigationQueryResult::Success) {
		isAlreadyGoal = ai->MoveToLocation(dest);
		UE_LOG(LogTemp,Warning,TEXT("%f"),me->GetCharacterMovement()->Velocity.Length());
	}
	PRINT2SCREEN(TEXT("dst : %f"), FVector::Dist(me->GetActorLocation(), dest));

	// �����ߴٸ� ��ǥ������ ���� �ٸ��� �ൿ
	//if (isAlreadyGoal == EPathFollowingRequestResult::AlreadyAtGoal) {
	if(FVector::Dist(me->GetActorLocation(), dest)< 100.0f ){
		if (curDestinationName.Contains("SearchPos")) {
			mState = EEnemyState::Search;
		}
		else if(curDestinationName.Contains("EndPos")){
			currentTime=0;
			mState = EEnemyState::Idle;
		}
	}
}



void UEnemyFSM::SearchState()
{
	// Ž�������� ���� ���� ������
	if (!isTurningFinished) {
		//PRINT2SCREEN(TEXT("_Turning"));
		me->firsttick=true;
		FRotator curRot = me->GetActorRotation();
		FRotator aimRot = refPositions[curDestinationName]->GetActorForwardVector().ToOrientationRotator();

		currentTime += GetWorld()->DeltaTimeSeconds;

		float alpha = currentTime * 0.5;
		FRotator nowRot = FRotator(FQuat::Slerp(curRot.Quaternion(), aimRot.Quaternion(), alpha));

		me->SetActorRotation(nowRot);

		if (currentTime > 2) {
			isTurningFinished = true;
			currentTime = 0;
		}
		return;
	}

	// ã�� ����� �Ѵ�
	me->SearchPlayer();
	//anim->PlaySearchAnim("FindAnim");
	//PRINT2SCREEN(TEXT("_SearchingAnim"));

	// Ž���ð����� ã�� ���Ѵٸ� ���ư��� ���
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > searchTime) {
		currentTime = 0;
		mState = EEnemyState::RadioReport;
		anim->animState = mState;
	}

	// �ִϸ��̼��� �ϴ� �߿� cone �� �÷��̾ �ɸ��ٸ�
	if (bIsFoundPlayer) {
		target->Exposed();
		mState = EEnemyState::Attack;
		anim->animState = mState;
	}
}

void UEnemyFSM::RadioReportState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > searchTime) {
		currentTime = 0;
		mState = EEnemyState::Move;
		curDestinationName = "EndPos";

		anim->animState = mState;
	}
}

// �����ð��� �ѹ��� �����ϰ� �ʹ�
void UEnemyFSM::AttackState()
{
	// ���� : ������ �� curtime = 0 �ΰ�
	//		: isTurnFinished�� false �ΰ�

	// �÷��̾������� �ٶ󺻴�
	//	- �÷��̾� �� ������ ���Ѵ�
	//	- z �� �ʱ�ȭ ��Ų��
	//	- Make rot From
	//	- lerp �� �ٶ󺻴�
	
	FVector aimDir = target->GetActorLocation() - me->GetActorLocation();
	aimDir.Z = 0;

	//FRotator lookRot = lookDir.Rotation();
	FRotator aimRot = UKismetMathLibrary::MakeRotFromX(aimDir);
	me->SetActorRotation(aimRot);

#pragma region Lerp Rot to Player
	// Lerp ����� ��� ����
	/*FRotator curRot = me->GetActorRotation();

	float alpha = currentTime;
	FRotator nowRot = FRotator(FQuat::Slerp(curRot.Quaternion(), aimRot.Quaternion(), alpha));*/
#pragma endregion
	
	// attack time ���� attack �� �ݺ��ϰ� �ʹ� 
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		//PRINT_LOG(TEXT("Attack!!!!!!!!!!!!!"));

		me->Fire();
		if (fireSoundFactory!=nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSoundFactory, me->GetActorLocation());
		}
		//anim->bAttackPlay = true;
	}

	// Player �� ���°� hidden �̶�� search ����
	if (!bIsFoundPlayer) {
		mState = EEnemyState::Search;
		//anim->animState = mState;
	}

}

void UEnemyFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		//mState = EEnemyState::Idle;
		if(!bIsFoundPlayer)
		ChangeEnemyStateToAttack();
		else
		mState = EEnemyState::Attack;
		//anim->animState = mState;
	}
}


void UEnemyFSM::DieState()
{
	// �����ص� �ȴٰ� ���ϸ� �Ʒ����� �������� ����.
	//if (anim->isDiePlaying == false)
	//{
	//	return;
	//}

	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > dieDelayTime)
	{	
		if (bIsFoundPlayer) {
			target->KillExposeEnemy();
		}
		// ���ְ� �ʹ�.
		me->Destroy();
	}
}



// �ǰݽ� ȣ��� �ݹ�(�̺�Ʈ) �Լ�
void UEnemyFSM::OnDamageProcess()
{
	currentTime = 0;
	// ü���� 0 �̻��̸�
	if (me->hp > 1)
	{
		PRINT2SCREEN(TEXT("OnDamageProcess"));
		me->hp--;
		mState = EEnemyState::Damage;
		//anim->bDamage = true;
		
		// �ǰ� �ִϸ��̼Ǹ�Ÿ�� ���
		//int32 index = FMath::RandRange(0, 1);s
		//FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		//anim->PlayDamageAnim(FName(*sectionName));
	}
	// �׷��� ������ 
	else
	{	
		me->hp = 0;
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		mState = EEnemyState::Die;

		//anim->PlayDamageAnim(TEXT("Die"));
	}
	//anim->animState = mState;
	ai->StopMovement();
}

bool UEnemyFSM::GetRandomPosInNavMesh(FVector center, float radius, FVector& dest)
{
	UNavigationSystemV1* ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;

	bool result = ns->GetRandomReachablePointInRadius(center, radius, loc);
	dest = loc.Location;
	return true;
}

void UEnemyFSM::FindPathByAI(FVector destination, FPathFindingResult& result)
{
	// navigation �ý��� ����
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetGoalLocation(destination);
	req.SetAcceptanceRadius(3);

	ai->BuildPathfindingQuery(req, query);
	result = ns->FindPathSync(query);
}

void UEnemyFSM::ChangeEnemyStateToAttack()
{
	if(mState== EEnemyState::Attack)
	return;
	

	bIsFoundPlayer = true;
	target->Exposed();
	mState = EEnemyState::Attack;
}

