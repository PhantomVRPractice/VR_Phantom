// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class IMITATION_PHANTOM_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSM")
	EEnemyState animState;

	// 공격 재생할지 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSM")
	bool bAttackPlay = false;

	// 공격 종료 이벤트가 호출되면 처리할 함수
	UFUNCTION(BlueprintCallable, Category="FSM")
	void OnAttackEndEvent();

	// 피격 상태 전환 여부
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bDamage = false;

	// 피격 몽타주 처리함수 선언
	UFUNCTION(BlueprintImplementableEvent, Category="FSM")
	void PlayDamageAnim(FName sectionName);
	
	// DieEndNoti 이벤트가 들어오면 그때서야 DieState 가 동작하도록 하고싶다.
	UFUNCTION()
	void AnimNotify_DieEndNoti();

	// DieState 실행해도 되는지 여부
	bool isDiePlaying = false;
};
