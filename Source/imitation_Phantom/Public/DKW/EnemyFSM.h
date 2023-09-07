// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// enemy 의 상태를 나타낼 enum 자료형 만들기
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle State"),
	Move,
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	EEnemyState mState = EEnemyState::Idle;

	// 필요속성 : 대기시간, 경과시간
	UPROPERTY(EditAnywhere, Category = "FSM")
	float idleDelayTime = 2;
	float currentTime = 0;

	//---------------------------------//
	// 이동
	// 필요속성 : 타겟, 이동속도
	UPROPERTY()
	class AVRCharacter* target;
	
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

	//----------------------------------------//
	// 공격
	// 필요속성 : 공격대기시간
	// 필요속성 : 초기값 체력
	// 필요속성 : 현재 체력
	UPROPERTY(EditAnywhere, Category="FSM")
	float attackDelayTime = 2;
	UPROPERTY(EditAnywhere, Category="FSM")
	int32 initialHP = 5;
	UPROPERTY(EditAnywhere, Category="FSM")
	int32 hp = 0;

	// 피격
	// 필요속성 : 피격대기시간
	// 필요속성 : 아래로 이동하는 속도
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
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

	// 피격시 호출될 콜백(이벤트) 함수
	void OnDamageProcess();
	
	FVector randomPos;

	// 패트롤 하기 위해서 이동가능 위치를 랜덤하게 찾아주기
	bool GetRandomPosInNavMesh(FVector center, float radius, FVector& dest);
};
