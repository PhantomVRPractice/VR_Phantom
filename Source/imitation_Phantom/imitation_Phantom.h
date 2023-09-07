// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(PhantomMsg, Log, All)

// 호출된 함수위치, 줄번호까지 문자열로 가져오기
#define APPINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

// 주어진 포맷으로 메시지를 찍는 매크로
#define PRINT_LOG(fmt, ...) UE_LOG(PhantomMsg, Warning, TEXT("%s %s"), *APPINFO, *FString::Printf(fmt, ##__VA_ARGS__))

#define PRINT2SCREEN(fmt, ...) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, FString::Printf(fmt, ##__VA_ARGS__))
