// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/EnemyAnim.h"

void UEnemyAnim::OnAttackEndEvent()
{
	bAttackPlay = false;
}

void UEnemyAnim::AnimNotify_DieEndNoti()
{
	isDiePlaying = true;
}