// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_NPCChat.generated.h"

/**
 * 
 */
UCLASS()
class LIVE_ROAD_API UCUserWidget_NPCChat : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetNPCMessage(const FString& Message);

protected:
    UPROPERTY(meta = (BindWidget))
	class UTextBlock* NPCMessage;
	
};
