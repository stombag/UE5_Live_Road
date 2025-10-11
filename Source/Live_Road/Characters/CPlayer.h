

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class LIVE_ROAD_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class ACWeapon> WeaponClass;
private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D PitchAngle = FVector2D(-40, +40);
// springArm �̶�� �̸��� USpringArmComponent��ü�� ����Ű�� ������ ���� ����
	// �̷� Ŭ������ �ִٴ� �� ���� �˷��ִ� �����̴�.  
	// ��� ������ �߰����� �ʾƵ� �ȴ�. cpp���Ͽ��� �ص� �ȴ�.

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

public:
	FORCEINLINE class ACWeapon* GetWeapon() { return Weapon; }

public:
	ACPlayer();

protected:


	virtual void BeginPlay() override;

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);
	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);

	void OnRun();
	void OffRun();

private:
	class ACWeapon* Weapon;

}; 