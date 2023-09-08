// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(PhantomMsg, Log, All)

// ȣ��� �Լ���ġ, �ٹ�ȣ���� ���ڿ��� ��������
#define APPINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// �־��� �������� �޽����� ��� ��ũ��
#define PRINT_LOG(fmt, ...) UE_LOG(PhantomMsg, Warning, TEXT("%s %s"), *APPINFO, *FString::Printf(fmt, ##__VA_ARGS__))

#define PRINT2SCREEN(fmt, ...) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(fmt, ##__VA_ARGS__))
