// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DKW/PosRefActor.h"
#include "EnemyFSM.generated.h"

// enemy 의 상태를 나타낼 enum 자료형 만들기
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle State"),
	Move,
	Search,
	RadioReport,
	Attack,
	Damage,
	Die
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IMITATION_PHANTOM_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 애너미 활성화
	// 지금 임시 활성화
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "FSM")
	bool isEnemyActive = true;
	// 탐색지역이나 타겟을 향해 바라보기
	UPROPERTY()
	bool isTurningFinished;

public:
	
	// 플레이어를 찼았는지 판단
	UPROPERTY(EditAnywhere, Category = "FSM")
	bool bIsFoundPlayer = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	EEnemyState mState = EEnemyState::Idle;

	// 필요속성 : 대기시간, 경과시간
	UPROPERTY(EditAnywhere, Category = "FSM|DelayTime")
	float idleDelayTime = 2;
	UPROPERTY(EditAnywhere, Category = "FSM|DelayTime")
	float searchTime = 5;
	UPROPERTY(EditAnywhere, Category = "FSM|DelayTime")
	float radioReportTime = 5;

	UPROPERTY()
	float currentTime = 0;

	
	//---------------------------------//
	// 이동
	// 필요속성 : 타겟, 이동속도
	UPROPERTY()
	class AVRPawn* target;

	UPROPERTY(EditAnywhere, Category = "FSM")
	TMap<FString, class APosRefActor*> refPositions;
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	FString curDestinationName;

	
	UPROPERTY(EditAnywhere, Category = "FSM")
	float speed = 500;
	// 나를 소유하고 있는 엑터
	UPROPERTY()
	class AEnemy* me;


	// 필요속성 : 공격범위 
	UPROPERTY(EditAnywhere, Category="FSM")
	float attackRange = 206;

	// 공격범위 시각화여부
	UPROPERTY(EditAnywhere, Category="FSM")
	bool bDebugAttackRange = false;

	// 공격
	UPROPERTY(EditAnywhere, Category="FSM")
	float attackDelayTime = 2;
	UPROPERTY(EditAnywhere, Category="FSM")
	int32 initialHP = 5;
	UPROPERTY(EditAnywhere, Category="FSM")
	int32 hp = 0;

	// 피격
	UPROPERTY(EditAnywhere, Category="FSM")
	float damageDelayTime = 2;
	UPROPERTY(EditAnywhere, Category= "FSM")
	float dieMoveSpeed = 100.f;

public:	// Animation
	UPROPERTY()
	class UEnemyAnim* anim;

	// ai controller for pathFinding
	UPROPERTY()
	class AAIController* ai;

public:	// 상태함수 
#pragma region State Functions
	void IdleState();
	void MoveState();
	void SearchState();
	void RadioReportState();
	void AttackState();
	void DamageState();
	void DieState();
#pragma endregion


	// 피격시 호출될 콜백(이벤트) 함수
	void OnDamageProcess();
	
#pragma region AI Navigation
private:
	// 탐색
	FVector randomPos;

	// 패트롤 하기 위해서 이동가능 위치를 랜덤하게 찾아주기
	bool GetRandomPosInNavMesh(FVector center, float radius, FVector& dest);
	// FindPathByAI 
	void FindPathByAI(FVector destination, struct FPathFindingResult& result);
	// LookAtAim
	/*void LookAtAim();
	void LookAtAim(FRotator curRot, FRotator newRot);*/

public:
	// ActivateEnemy
	void ActivateEnemy(){isEnemyActive = true;}
	void ChangeEnemyStateToAttack();

#pragma endregion

};
