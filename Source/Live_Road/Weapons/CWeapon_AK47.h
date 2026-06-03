#pragma once

#include "CoreMinimal.h"
#include "CWeapon.h"
#include "CWeapon_AK47.generated.h"

UCLASS(Blueprintable)
class LIVE_ROAD_API ACWeapon_AK47 : public ACWeapon
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName LeftHandSocketName;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Sight;

public:

	ACWeapon_AK47();

public:

	void Begin_Equip() override;
	void End_Equip() override;

	void Begin_Aim() override;
	void End_Aim() override;
};
