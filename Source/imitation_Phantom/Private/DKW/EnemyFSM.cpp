// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/EnemyFSM.h"
#include <Kismet/GameplayStatics.h>
#include "VRCharacter.h"
#include "DKW/EnemyAnim.h"
#include "DKW/Enemy.h"
#include <AIModule/Classes/AIController.h>
#include <NavigationSystem/Public/NavigationSystem.h>
#include <Components/CapsuleComponent.h>

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

	// 타겟찾기
	target = Cast<AVRCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRCharacter::StaticClass()));

	// 나 
	me = Cast<AEnemy>(GetOwner());

	// EnemyAnim가 있어야 한다
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	
	// 초기 HP
	hp = initialHP;

	// AIController 할당하기
	ai = Cast<AAIController>(me->GetController());
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
	//PRINT2SCREEN(TEXT("%s"), * strState);

	// FSM 목차
	switch (mState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
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

	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > idleDelayTime)
	{
		mState = EEnemyState::Move;
		currentTime = 0;
		
		anim->animState = mState;
		GetRandomPosInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

// 타겟으로 이동하고 싶다
// 필요속성 : 타겟, 이동속도
void UEnemyFSM::MoveState()
{
	FVector dest = target->GetActorLocation();
	FVector Direction = dest - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// navi 시스템 세팅
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetGoalLocation(dest);
	req.SetAcceptanceRadius(3);

	ai->BuildPathfindingQuery(req, query);
	auto r = ns->FindPathSync(query);

	// 타겟쪽으로이동할 수 있으면
	if (r.Result == ENavigationQueryResult::Success) {
		// 거기로 간다
		ai->MoveToLocation(dest);
	}
	// 그렇지 않으면
	else {
		// 랜덤한 위치를 찾아서 그리로 이동하고 싶다
		auto result = ai->MoveToLocation(randomPos);
		// 도착했다면
		if (result == EPathFollowingRequestResult::AlreadyAtGoal) {
			GetRandomPosInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}

	// 공격범위 안에 들어오면
	if (distance < attackRange) {
		// 상태를 공격으로 전환하고 싶다
		mState = EEnemyState::Attack;
		anim->animState = mState;
		currentTime = attackDelayTime;

		ai->StopMovement();
	}
}

// 일정시간에 한번씩 공격하고 싶다
void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		//PRINT_LOG(TEXT("Attack!!!!!!!!!!!!!"));
		anim->bAttackPlay = true;
	}

	// 타겟이 도망가면 상태를 이동으로 전환하고 싶다.
	float distance = FVector::Dist(target->GetActorLocation(), me->GetActorLocation());
	
	// -> 타겟이 공격범위를 벗어나면
	if (distance > attackRange)
	{
		mState = EEnemyState::Move;
		currentTime = 0;
		anim->animState = mState;
		GetRandomPosInNavMesh(me->GetActorLocation(), 500, randomPos);
	}
}

void UEnemyFSM::DamageState()
{
	// 1. 시간이 흘렀으니까
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. 경과시간이 대기시간을 초과했으니까
	if (currentTime > damageDelayTime)
	{
		// 3. 상태를 대기로 전환하고 싶다.
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
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
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	// 그렇지 않으면 
	else
	{
		// 상태를 Die 로 전환하고 싶다.
		mState = EEnemyState::Die;
		// 콜리전 꺼주기
		
		me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		anim->PlayDamageAnim(TEXT("Die"));
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

