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

	// ���� ������� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="FSM")
	bool bAttackPlay = false;

	// ���� ���� �̺�Ʈ�� ȣ��Ǹ� ó���� �Լ�
	UFUNCTION(BlueprintCallable, Category="FSM")
	void OnAttackEndEvent();

	// �ǰ� ���� ��ȯ ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FSM")
	bool bDamage = false;

	// �ǰ� ��Ÿ�� ó���Լ� ����
	UFUNCTION(BlueprintImplementableEvent, Category="FSM")
	void PlayDamageAnim(FName sectionName);
	
	// DieEndNoti �̺�Ʈ�� ������ �׶����� DieState �� �����ϵ��� �ϰ��ʹ�.
	UFUNCTION()
	void AnimNotify_DieEndNoti();

	// DieState �����ص� �Ǵ��� ����
	bool isDiePlaying = false;
};