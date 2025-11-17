// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Weapons/CWeaponComponent.h"
#include "CUserWidget_HUD.generated.h"

UCLASS()
class LIVE_ROAD_API UCUserWidget_HUD : public UUserWidget
{
	GENERATED_BODY()
public:

	//UFUNCTION(BlueprintImplementableEvent)
	//void OnAutoFire();
	//// 위젯은 보통 코드에서 함수를 만들고 블프로 가서 그 함수를 사용한다.

	//UFUNCTION(BlueprintImplementableEvent)
	//void OffAutoFire(); 

	UFUNCTION(BlueprintImplementableEvent)
	void DrawAutoFire(bool bAutoFire);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMagazineCount(uint8 InCurent, uint8 InMax);



	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWeaponType(EWeaponType InType);


	 
};
