// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

// enemy �� ���¸� ��Ÿ�� enum �ڷ��� �����
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

	// �ʿ�Ӽ� : ���ð�, ����ð�
	UPROPERTY(EditAnywhere, Category = "FSM")
	float idleDelayTime = 2;
	float currentTime = 0;

	//---------------------------------//
	// �̵�
	// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
	UPROPERTY()
	class AVRCharacter* target;
	
	UPROPERTY(EditAnywhere, Category = "FSM")
	float speed = 500;
	// ���� �����ϰ� �ִ� ����
	UPROPERTY()
	class AEnemy* me;

	// �ʿ�Ӽ� : ���ݹ��� 
	UPROPERTY(EditAnywhere, Category="FSM")
	float attackRange = 206;

	// ���ݹ��� �ð�ȭ����
	UPROPERTY(EditAnywhere, Category="FSM")
	bool bDebugAttackRange = false;

	//----------------------------------------//
	// ����
	// �ʿ�Ӽ� : ���ݴ��ð�
	// �ʿ�Ӽ� : �ʱⰪ ü��
	// �ʿ�Ӽ� : ���� ü��
	UPROPERTY(EditAnywhere, Category="FSM")
	float attackDelayTime = 2;
	UPROPERTY(EditAnywhere, Category="FSM")
	int32 initialHP = 5;
	UPROPERTY(EditAnywhere, Category="FSM")
	int32 hp = 0;

	// �ǰ�
	// �ʿ�Ӽ� : �ǰݴ��ð�
	// �ʿ�Ӽ� : �Ʒ��� �̵��ϴ� �ӵ�
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

public:	// �����Լ� 
	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

	// �ǰݽ� ȣ��� �ݹ�(�̺�Ʈ) �Լ�
	void OnDamageProcess();
	
	FVector randomPos;

	// ��Ʈ�� �ϱ� ���ؼ� �̵����� ��ġ�� �����ϰ� ã���ֱ�
	bool GetRandomPosInNavMesh(FVector center, float radius, FVector& dest);
};