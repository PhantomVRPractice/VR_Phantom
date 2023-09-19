// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DKW/PosRefActor.h"
#include "EnemyFSM.generated.h"

// enemy �� ���¸� ��Ÿ�� enum �ڷ��� �����
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
	// �ֳʹ� Ȱ��ȭ
	// ���� �ӽ� Ȱ��ȭ
	UPROPERTY(EditAnywhere,BlueprintReadWrite ,Category = "FSM")
	bool isEnemyActive = true;
	// Ž�������̳� Ÿ���� ���� �ٶ󺸱�
	UPROPERTY()
	bool isTurningFinished;

public:
	
	// �÷��̾ á�Ҵ��� �Ǵ�
	UPROPERTY(EditAnywhere, Category = "FSM")
	bool bIsFoundPlayer = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FSM")
	EEnemyState mState = EEnemyState::Idle;

	// �ʿ�Ӽ� : ���ð�, ����ð�
	UPROPERTY(EditAnywhere, Category = "FSM|DelayTime")
	float idleDelayTime = 2;
	UPROPERTY(EditAnywhere, Category = "FSM|DelayTime")
	float searchTime = 5;
	UPROPERTY(EditAnywhere, Category = "FSM|DelayTime")
	float radioReportTime = 5;

	UPROPERTY()
	float currentTime = 0;

	
	//---------------------------------//
	// �̵�
	// �ʿ�Ӽ� : Ÿ��, �̵��ӵ�
	UPROPERTY()
	class AVRPawn* target;

	UPROPERTY(EditAnywhere, Category = "FSM")
	TMap<FString, class APosRefActor*> refPositions;
	UPROPERTY(VisibleAnywhere, Category = "FSM")
	FString curDestinationName;

	
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

	// ����
	UPROPERTY(EditAnywhere, Category="FSM")
	float attackDelayTime = 2;
	UPROPERTY(EditAnywhere, Category="FSM")
	int32 initialHP = 5;
	UPROPERTY(EditAnywhere, Category="FSM")
	int32 hp = 0;

	// �ǰ�
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
#pragma region State Functions
	void IdleState();
	void MoveState();
	void SearchState();
	void RadioReportState();
	void AttackState();
	void DamageState();
	void DieState();
#pragma endregion


	// �ǰݽ� ȣ��� �ݹ�(�̺�Ʈ) �Լ�
	void OnDamageProcess();
	
#pragma region AI Navigation
private:
	// Ž��
	FVector randomPos;

	// ��Ʈ�� �ϱ� ���ؼ� �̵����� ��ġ�� �����ϰ� ã���ֱ�
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
