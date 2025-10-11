

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
// springArm 이라는 이름의 USpringArmComponent객체를 가리키는 포인터 변수 선언
	// 이런 클래스가 있다는 걸 먼저 알려주는 선언이다.  
	// 헤더 파일을 추가하지 않아도 된다. cpp파일에서 해도 된다.

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