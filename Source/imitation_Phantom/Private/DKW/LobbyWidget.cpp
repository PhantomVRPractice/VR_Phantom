// Fill out your copyright notice in the Description page of Project Settings.


#include "DKW/LobbyWidget.h"
#include "Components/Button.h"

#include "../imitation_Phantom.h"

void ULobbyWidget::NativeConstruct()
{
	Btn_Campaign->OnClicked.AddDynamic(this, &ULobbyWidget::ChangeToMainMap);
}

void ULobbyWidget::ChangeToMainMap()
{
	PRINT2SCREEN(TEXT("Change to map"));
}
