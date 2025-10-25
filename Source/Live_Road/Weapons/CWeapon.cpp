

#include "../Weapons/CWeapon.h"
#include "../Characters/CPlayer.h"
#include "../Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Components/TimelineComponent.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundWave.h"
#include "GameFramework/SpringArmComponent.h"
void FWeaponAimData::SetData(ACharacter* InOwner)
{
	USpringArmComponent* springArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	springArm->TargetArmLength = TargetArmLength;
	springArm->SocketOffset = SoketOffset;
	springArm->bEnableCameraLag = bEnableCameraLag;
}


void FWeaponAimData::SetDataByNoneCurve(ACharacter* InOwner)
{
	SetData(InOwner);
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(InOwner);
	camera->FieldOfView = FielOfView;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);

	CHelpers::CreateActorComponent<UTimelineComponent>(this, &Timeline, "Timeline");

	CHelpers::GetAsset<UMaterialInstanceConstant>(&HitDecal, "/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_Decal_Inst.M_Decal_Inst'");
	CHelpers::GetAsset<UParticleSystem>(&HitParticle, "/Script/Engine.ParticleSystem'/Game/Effects/P_Impact_Default.P_Impact_Default'");
	CHelpers::GetAsset<UParticleSystem>(&FlashParticle, "/Script/Engine.ParticleSystem'/Game/Effects/P_Impact_Muzzle.P_Impact_Muzzle'");
		
	CHelpers::GetAsset<UParticleSystem>(&EJectParticle, "/Script/Engine.ParticleSystem'/Game/Effects/P_Eject_bullet.P_Eject_bullet'");
	CHelpers::GetAsset<USoundWave>(&FireSound, "/Script/Engine.SoundWave'/Game/Audio/GunsSound/5_56_M4_Rifle/5_56_M4_Rifle_Gunshots/5_56_M4_Rifle_-__Gunshot_A_001.5_56_M4_Rifle_-__Gunshot_A_001'");

	CHelpers::GetAsset<UCurveFloat>(&AimCurve, "/Script/Engine.CurveFloat'/Game/Blueprints/Weapons/Cureve_Aim.Cureve_Aim'");

}


void ACWeapon::BeginPlay()
{

	Super::BeginPlay();

	Owner = Cast<ACPlayer>(GetOwner());

	if (HolsterSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);

	BaseData.SetDataByNoneCurve(Owner);

	if (!!AimCurve) {
		FOnTimelineFloat timeline;
		timeline.BindUFunction(this, "OnAiming");

		Timeline->AddInterpFloat(AimCurve, timeline);
		Timeline->SetLooping(false);
		Timeline->SetPlayRate(AimingSpeed);
	}

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
	// ���� ��鸮�� �ϴ� �ڵ�
	direction = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(direction, RecoilAnlgle);
	FVector end = start + direction * HitDistance;

	//DrawDebugLine(GetWorld(), start, end, FColor::Red, true, 5);

	FHitResult hitResult;
	TArray<AActor*> ignores;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end, ETraceTypeQuery::TraceTypeQuery1, false, ignores, EDrawDebugTrace::None, hitResult, true);

	if (hitResult.bBlockingHit)
	{
	
		if (!!HitDecal) {
		FRotator rotator = (-hitResult.ImpactNormal).Rotation();
		// �����δ� �׷����� ������ ȭ�� ������� ������ �ʴ´�
		UDecalComponent* decal= UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(5), hitResult.Location,rotator, 10);
		// �׷��� ��ũ�� ����� ���� ���̵� �������µ� �װ� ���°Ŵ�
		decal->SetFadeScreenSize(0); 

		}

		if (!!HitParticle) {

			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(hitResult.Location, hitResult.TraceStart);
			
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, hitResult.Location, rotator);
		}
		
	}

	if (!!FlashParticle)
		UGameplayStatics::SpawnEmitterAttached(FlashParticle, Mesh, "Muzzle", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	if (!!EJectParticle)
		UGameplayStatics::SpawnEmitterAttached(EJectParticle, Mesh, "EJect", FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
	FVector muzzleLocaion = Mesh->GetSocketLocation("Muzzle");
	if (!!FireSound)
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSound, muzzleLocaion);

}

void ACWeapon::End_Fire()
{


}

bool ACWeapon::CanAim()
{
	bool b = false;
	b |= bEquipping;
	b |= bInAim;

	return  b== false;
}

void ACWeapon::Begin_Aim()
{
	bInAim = true;

	if (!!AimCurve) {
		Timeline->PlayFromStart();
		AimData.SetData(Owner);
		return;
	}
	AimData.SetDataByNoneCurve(Owner);


} 

void ACWeapon::End_Aim()
{
	CheckFalse(bInAim);
	bInAim = false;

	if (!!AimCurve) {
		Timeline->ReverseFromEnd();
		BaseData.SetData(Owner);
		return;
	}
	BaseData.SetDataByNoneCurve(Owner);
}

void ACWeapon::OnAiming(float Output)
{
	UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(Owner);

	camera->FieldOfView = FMath::Lerp(AimData.FielOfView, BaseData.FielOfView,Output);
}

