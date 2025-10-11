// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Weapons/CWeapon.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LIVE_ROAD_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	// 블프가 자식이다. protected는 부모자식관계에서 접근가능하다
protected:
	 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	 float Speed;

	 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	 EWeaponType WeaponType = EWeaponType::Max;


public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	class ACPlayer* OwnerCharacter;
};
