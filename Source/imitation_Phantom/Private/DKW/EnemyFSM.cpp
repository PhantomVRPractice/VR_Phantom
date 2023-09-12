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
	target = Cast<AVRCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRCharacter::StaticClass()));
	me	   = Cast<AEnemy>(GetOwner());
	anim   = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	
	// AIController
	ai = Cast<AAIController>(me->GetController());
	me->GetCharacterMovement()->MaxWalkSpeed = speed;

	// 초기 HP
	hp = initialHP;

	// 시작위치 저장
	//refPositions.Add("StartPos", me);
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 공격범위 시각화 
	if (bDebugAttackRange) {
		DrawDebugSphere(GetWorld(), me->GetActorLocation(), attackRange, 20, FColor::Red, false, -1, 0, 2);
	}

	// 현재 상태 출력
	FString strState;
	UEnum::GetValueAsString(mState, strState);
	PRINT2SCREEN(TEXT("%s"), * strState);

	// FSM 목차
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

// 일정시간이 지나면 상태를 이동으로 바꾸고 싶다
// 필요속성 : 대기시간, 경과시간
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

#pragma region OldCode
	//--------------------------------------------
		//currentTime += GetWorld()->DeltaTimeSeconds;
		//if (currentTime > idleDelayTime)
		//{
		//	mState = EEnemyState::Move;
		//	currentTime = 0;
		//	
		//	anim->animState = mState;
		//	GetRandomPosInNavMesh(me->GetActorLocation(), 500, randomPos);
		//}
#pragma endregion
}

// 타겟으로 이동하고 싶다
// 필요속성 : 타겟, 이동속도
void UEnemyFSM::MoveState()
{
	if(!refPositions[curDestinationName])
	{
		PRINT2SCREEN(TEXT("No CurDestination Pos"));
		return;
	}

	FVector dest = refPositions[curDestinationName]->GetActorLocation();

	EPathFollowingRequestResult::Type isAlreadyGoal = EPathFollowingRequestResult::Failed;
	
	FPathFindingResult r;
	FindPathByAI(dest, r);

	// 타겟쪽으로이동할 수 있으면 거기로 간다
	if (r.Result == ENavigationQueryResult::Success) {
		isAlreadyGoal = ai->MoveToLocation(dest);
		PRINT2SCREEN(TEXT("Enemy can reach to dest"));
	}

	// 도착했다면 목표지점에 따라서 다르게 행동
	if (isAlreadyGoal == EPathFollowingRequestResult::AlreadyAtGoal) {
		if (curDestinationName.Contains("SearchPos")) {
			PRINT2SCREEN(TEXT("enemy is on SearchPos"));
			mState = EEnemyState::Search;
		}
		else if(curDestinationName.Contains("EndPos")){
			PRINT2SCREEN(TEXT("enemy is on EndPos"));
			mState = EEnemyState::Idle;
			isEnemyActive = false;
		}
	}
#pragma region Old
		//// -------------------------------------------
			//FVector dest = target->GetActorLocation();
			//FVector Direction = dest - me->GetActorLocation();
			//float distance = Direction.Length();
			//Direction.Normalize();

			//// navi 시스템 세팅
			//auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

			//FPathFindingQuery query;
			//FAIMoveRequest req;
			//req.SetGoalLocation(dest);
			//req.SetAcceptanceRadius(3);

			//ai->BuildPathfindingQuery(req, query);
			//auto r = ns->FindPathSync(query);

			//// 타겟쪽으로이동할 수 있으면
			//if (r.Result == ENavigationQueryResult::Success) {
			//	// 거기로 간다
			//	ai->MoveToLocation(dest);
			//}
			//// 그렇지 않으면
			//else {
			//	// 랜덤한 위치를 찾아서 그리로 이동하고 싶다
			//	auto result = ai->MoveToLocation(randomPos);
			//	// 도착했다면
			//	if (result == EPathFollowingRequestResult::AlreadyAtGoal) {
			//		GetRandomPosInNavMesh(me->GetActorLocation(), 500, randomPos);
			//	}
			//}

			//// 공격범위 안에 들어오면
			//if (distance < attackRange) {
			//	// 상태를 공격으로 전환하고 싶다
			//	mState = EEnemyState::Attack;
			//	anim->animState = mState;
			//	currentTime = attackDelayTime;

			//	ai->StopMovement();
			//}
#pragma endregion
}



void UEnemyFSM::SearchState()
{
	// 탐색지역을 향해 몸을 돌린다
	if (!isTurningFinished) {
		PRINT2SCREEN(TEXT("_Turning"));

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

	// 찾는 모션을 한다
	me->SearchPlayer();
	anim->PlaySearchAnim("FindAnim");
	PRINT2SCREEN(TEXT("_SearchingAnim"));

	// 탐색시간동안 찾지 못한다면 돌아가는 모션
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > searchTime) {
		currentTime = 0;
		mState = EEnemyState::RadioReport;
		anim->animState = mState;
	}

	// 애니메이션을 하던 중에 cone 에 플레이어가 걸린다면
	if (bIsFoundPlayer) {
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

// 일정시간에 한번씩 공격하고 싶다
void UEnemyFSM::AttackState()
{
	// 주의 : 시작할 때 curtime = 0 인가
	//		: isTurnFinished가 false 인가

	// 플레이어쪽으로 바라본다
	//	- 플레이어 쪽 방향을 구한다
	//	- z 를 초기화 시킨다
	//	- Make rot From
	//	- lerp 로 바라본다
	
	FVector aimDir = target->GetActorLocation() - me->GetActorLocation();
	aimDir.Z = 0;

	//FRotator lookRot = lookDir.Rotation();
	FRotator aimRot = UKismetMathLibrary::MakeRotFromX(aimDir);
	me->SetActorRotation(aimRot);

#pragma region Lerp Rot to Player
	// Lerp 기능은 잠시 배제
	/*FRotator curRot = me->GetActorRotation();

	float alpha = currentTime;
	FRotator nowRot = FRotator(FQuat::Slerp(curRot.Quaternion(), aimRot.Quaternion(), alpha));*/
#pragma endregion
	
	// attack time 마다 attack 을 반복하고 싶다 
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		PRINT_LOG(TEXT("Attack!!!!!!!!!!!!!"));

		me->Fire();
		anim->bAttackPlay = true;
	}

	// Player 의 상태가 hidden 이라면 search 진행
	if (!bIsFoundPlayer) {
		mState = EEnemyState::Search;
		anim->animState = mState;
	}

#pragma region Old
	//// 타겟이 도망가면 상태를 이동으로 전환하고 싶다.
	//float distance = FVector::Dist(target->GetActorLocation(), me->GetActorLocation());

	//// -> 타겟이 공격범위를 벗어나면
	//if (distance > attackRange)
	//{
	//	mState = EEnemyState::Move;
	//	currentTime = 0;
	//	anim->animState = mState;
	//	GetRandomPosInNavMesh(me->GetActorLocation(), 500, randomPos);
	//}
#pragma endregion
}

void UEnemyFSM::DamageState()
{
	// 일정시간동안 Damage animation 을 재생할 시간을 준다
	// 일정시간이 지나면 탐색으로 넘어간다
	//currentTime += GetWorld()->DeltaTimeSeconds;
	//if(currentTime > )

#pragma region Old
	// 1. 시간이 흘렀으니까
	// 2. 경과시간이 대기시간을 초과했으니까
	// 3. 상태를 대기로 전환하고 싶다.
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
#pragma endregion	
}


void UEnemyFSM::DieState()
{
	// 실행해도 된다고 안하면 아래내용 실행하지 말자.
	if (anim->isDiePlaying == false)
	{
		return;
	}
	//아래로 계속 이동하다가 P = P0 + vt
	FVector P = me->GetActorLocation() + FVector::DownVector * dieMoveSpeed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(P);

	//눈에 안보이는 시점까지 내려가면
	// Z 값이 -100 아래로 내려가면
	if (P.Z < -100)
	{
		// 없애고 싶다.
		me->Destroy();
	}
}



// 피격시 호출될 콜백(이벤트) 함수
void UEnemyFSM::OnDamageProcess()
{
	// 체력을 감소시키자
	hp--;

	currentTime = 0;
	// 체력이 0 이상이면
	if (hp > 0)
	{
		// 상태를 피격으로 전환하고 싶다.
		mState = EEnemyState::Damage;
		// 애니메이션 피격으로 상태전환
		anim->bDamage = true;
		// 피격 애니메이션몽타주 재생
		int32 index = FMath::RandRange(0, 1);
		//FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		//anim->PlayDamageAnim(FName(*sectionName));
	}
	// 그렇지 않으면 
	else
	{
		// 상태를 Die 로 전환하고 싶다.
		mState = EEnemyState::Die;
		// 콜리전 꺼주기
		
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//anim->PlayDamageAnim(TEXT("Die"));
	}
	anim->animState = mState;
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
	// navigation 시스템 세팅
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetGoalLocation(destination);
	req.SetAcceptanceRadius(3);

	ai->BuildPathfindingQuery(req, query);
	result = ns->FindPathSync(query);
}


