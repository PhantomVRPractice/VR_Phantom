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

	// Ÿ��ã��
	target = Cast<AVRCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AVRCharacter::StaticClass()));

	// �� 
	me = Cast<AEnemy>(GetOwner());

	// EnemyAnim�� �־�� �Ѵ�
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	
	// �ʱ� HP
	hp = initialHP;

	// AIController �Ҵ��ϱ�
	ai = Cast<AAIController>(me->GetController());
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
	//PRINT2SCREEN(TEXT("%s"), * strState);

	// FSM ����
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

// �����ð��� ������ ���¸� �̵����� �ٲٰ� �ʹ�
// �ʿ�Ӽ� : ���ð�, ����ð�
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

// Ÿ������ �̵��ϰ� �ʹ�
// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
void UEnemyFSM::MoveState()
{
	FVector dest = target->GetActorLocation();
	FVector Direction = dest - me->GetActorLocation();
	float distance = Direction.Length();
	Direction.Normalize();

	// navi �ý��� ����
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetGoalLocation(dest);
	req.SetAcceptanceRadius(3);

	ai->BuildPathfindingQuery(req, query);
	auto r = ns->FindPathSync(query);

	// Ÿ���������̵��� �� ������
	if (r.Result == ENavigationQueryResult::Success) {
		// �ű�� ����
		ai->MoveToLocation(dest);
	}
	// �׷��� ������
	else {
		// ������ ��ġ�� ã�Ƽ� �׸��� �̵��ϰ� �ʹ�
		auto result = ai->MoveToLocation(randomPos);
		// �����ߴٸ�
		if (result == EPathFollowingRequestResult::AlreadyAtGoal) {
			GetRandomPosInNavMesh(me->GetActorLocation(), 500, randomPos);
		}
	}

	// ���ݹ��� �ȿ� ������
	if (distance < attackRange) {
		// ���¸� �������� ��ȯ�ϰ� �ʹ�
		mState = EEnemyState::Attack;
		anim->animState = mState;
		currentTime = attackDelayTime;

		ai->StopMovement();
	}
}

// �����ð��� �ѹ��� �����ϰ� �ʹ�
void UEnemyFSM::AttackState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;
	if (currentTime > attackDelayTime)
	{
		currentTime = 0;
		//PRINT_LOG(TEXT("Attack!!!!!!!!!!!!!"));
		anim->bAttackPlay = true;
	}

	// Ÿ���� �������� ���¸� �̵����� ��ȯ�ϰ� �ʹ�.
	float distance = FVector::Dist(target->GetActorLocation(), me->GetActorLocation());
	
	// -> Ÿ���� ���ݹ����� �����
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
	// 1. �ð��� �귶���ϱ�
	currentTime += GetWorld()->DeltaTimeSeconds;
	// 2. ����ð��� ���ð��� �ʰ������ϱ�
	if (currentTime > damageDelayTime)
	{
		// 3. ���¸� ���� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Idle;
		currentTime = 0;
		anim->animState = mState;
	}
}


void UEnemyFSM::DieState()
{
	// �����ص� �ȴٰ� ���ϸ� �Ʒ����� �������� ����.
	if (anim->isDiePlaying == false)
	{
		return;
	}
	//�Ʒ��� ��� �̵��ϴٰ� P = P0 + vt
	FVector P = me->GetActorLocation() + FVector::DownVector * dieMoveSpeed * GetWorld()->DeltaTimeSeconds;
	me->SetActorLocation(P);

	//���� �Ⱥ��̴� �������� ��������
	// Z ���� -100 �Ʒ��� ��������
	if (P.Z < -100)
	{
		// ���ְ� �ʹ�.
		me->Destroy();
	}
}

// �ǰݽ� ȣ��� �ݹ�(�̺�Ʈ) �Լ�
void UEnemyFSM::OnDamageProcess()
{
	// ü���� ���ҽ�Ű��
	hp--;

	currentTime = 0;
	// ü���� 0 �̻��̸�
	if (hp > 0)
	{
		// ���¸� �ǰ����� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Damage;
		// �ִϸ��̼� �ǰ����� ������ȯ
		anim->bDamage = true;
		// �ǰ� �ִϸ��̼Ǹ�Ÿ�� ���
		int32 index = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), index);
		anim->PlayDamageAnim(FName(*sectionName));
	}
	// �׷��� ������ 
	else
	{
		// ���¸� Die �� ��ȯ�ϰ� �ʹ�.
		mState = EEnemyState::Die;
		// �ݸ��� ���ֱ�
		
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

