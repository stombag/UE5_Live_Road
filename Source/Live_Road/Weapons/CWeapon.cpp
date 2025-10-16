

#include "../Weapons/CWeapon.h"
#include "../Characters/CPlayer.h"
#include "../Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceConstant.h"
ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);

	CHelpers::GetAsset<UMaterialInstanceConstant>(&HitDecal, "/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'");
}

void ACWeapon::BeginPlay()
{

	Super::BeginPlay();

	Owner = Cast<ACPlayer>(GetOwner());

	if (HolsterSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);
}

void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACWeapon::CanEquip()
{
	bool b = false;
	b |= bEquipping;
	return b == false;
}

void ACWeapon::Equip()
{
	bEquipping = true;
	if (!!EquipMontage) {
		Owner->PlayAnimMontage(EquipMontage, EquipMontage_PlayRate);
	}
}

void ACWeapon::Begin_Equip()
{
	if (RightHandSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), RightHandSocketName);
}

void ACWeapon::End_Equip()
{
	bEquipping = false;

}

bool ACWeapon::CanUnequip()
{	
	bool b = false;
	b |= bEquipping;
	return b == false;
}

void ACWeapon::Unequip()
{
	if (HolsterSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);
}

bool ACWeapon::CanFire()
{
	bool b = false;
	b |= bEquipping;
	return b == false;
}

void ACWeapon::Begin_Fire()
{

	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(Owner);
	FVector direction = camera->GetForwardVector();
	FTransform transform = camera->GetComponentToWorld();

	FVector start = transform.GetLocation() + direction;
	// 에임 흔들리게 하는 코드
	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, RecoilAnlgle);
	FVector end = start + direction * HitDistance;

	//DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 5);

	FHitResult hitResult;
	TArray<AActor*> ignores;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::None, hitResult, true);

	if (hitResult.bBlockingHit)
	{
	
		FRotator rotator = (-hitResult.ImpactNormal).Rotation();
		// 실제로는 그려지고 있지만 화면 사이즈로 보이지 않는다
		UDecalComponent* decal= UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(5), hitResult.Location,rotator, 10);
		// 그래서 스크린 사이즈에 의해 페이드 가려지는데 그걸 끄는거다
		decal->SetFadeScreenSize(0); 

	}


}

void ACWeapon::End_Fire()
{

}

