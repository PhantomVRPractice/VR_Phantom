// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class IMITATION_PHANTOM_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = MySettings, meta = (BindWidget))
	class UButton* Btn_Campaign;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void ChangeToMainMap();
};
