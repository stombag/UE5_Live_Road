// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CAnimInstance.h"
#include "../Global.h"
#include "CPlayer.h"
#include "../Weapons/CWeaponComponent.h"


void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACPlayer> (TryGetPawnOwner());
	CheckNull(OwnerCharacter); 

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(Weapon);


	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// ���� ĳ���Ͱ� ��� ������ �ߵ��� �������� ������ ���� �ʴ´� �װ� ���� 

	CheckNull(OwnerCharacter);
	// ������ �̵��ӵ�
	Speed = OwnerCharacter->GetVelocity().Size2D();

// ���� ĳ������ �̵� ����(�ӵ� ����)�� ȸ����(FRotator)���� ��ȯ�Ͽ� ����
// ��: ��(+X)���� �̵� �� Yaw=0��, ������(+Y) �� Yaw=90��
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator target = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation,target, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	CheckNull(Weapon);

	bUseIK = Weapon->IsUnarmedMode() == false;
	LeftHandLocation = Weapon->GetLeftHandLocation();
} 

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{

	WeaponType = InNewType;

}
