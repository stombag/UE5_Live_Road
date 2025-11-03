// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeaponComponent.h"
#include "CWeapon.generated.h"


USTRUCT()
struct FWeaponAimData
{
	GENERATED_BODY()
	// 언리얼에서는 자료형을 묶어서 관리하기 위해서 구조체에 똑같이 GENERATED_BODY() 을 넣는다.

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength;
	
	UPROPERTY(EditAnywhere)
	FVector SoketOffset;

	UPROPERTY(EditAnywhere)
	float FielOfView;

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag;
public:
	
	void SetData(class ACharacter* InOwner);
	void SetDataByNoneCurve(class ACharacter* InOwner);
};


UCLASS(Abstract, NotBlueprintable)
class LIVE_ROAD_API ACWeapon : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	EWeaponType Type = EWeaponType::Max;

protected: // 설정
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName HolsterSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	class UAnimMontage* EquipMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	float EquipMontage_PlayRate = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName RightHandSocketName;


	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FVector LeftHandLocation;
	//IA 적용할때 넣을값

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDistance = 3000;
	// 발사되는 거리 

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	class UMaterialInstanceConstant* HitDecal;

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	class UParticleSystem* HitParticle;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float RecoilAnlgle;
	// 에임 흔들리는거

	UPROPERTY(EditDefaultsOnly, Category = "Fire") 
	class UParticleSystem* FlashParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire") 
	class UParticleSystem* EJectParticle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire") 
	class USoundWave* FireSound;

	UPROPERTY(EditDefaultsOnly, Category = "Fire") 
	float RecoilAngle;

	UPROPERTY(EditDefaultsOnly, Category = "Fire") 
	float AutoFireInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Fire") 
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;



protected:
	UPROPERTY(EditDefaultsOnly, Category = "Aim") 
	FWeaponAimData BaseData;

	UPROPERTY(EditDefaultsOnly, Category = "Aim") 
	FWeaponAimData AimData;

	UPROPERTY(EditDefaultsOnly, Category ="Aim")
	class UCurveFloat* AimCurve;


	UPROPERTY(EditDefaultsOnly, Category ="Aim")
	float AimingSpeed = 200;


private: // 컴퍼넌트
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

protected:// 메쉬	
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;
protected:
	UPROPERTY(VisibleAnywhere)
	class UTimelineComponent* Timeline;


public:
	FORCEINLINE const EWeaponType GetType() { return Type; }
	FORCEINLINE FVector GetLeftHandLocation() { return LeftHandLocation; }

public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool CanEquip();
	void Equip();
	void Begin_Equip();
	void End_Equip();

	bool CanUnequip();
	void Unequip();

public:
	bool CanFire();
	void Begin_Fire();
	void End_Fire();

private:
	UFUNCTION()
	void OnFiring();

public:
	bool CanAim();
	void Begin_Aim();
	void End_Aim();

private:
	UFUNCTION()
	void OnAiming(float Output);

private:
	class ACPlayer* Owner;

private:
	bool bEquipping;
	bool bInAim;
	bool bFiring;
	bool bAutoFire = true;
private:
	FTimerHandle AutoFireHandle;
	
};
