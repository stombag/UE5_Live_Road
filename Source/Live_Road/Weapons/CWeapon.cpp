

#include "../Weapons/CWeapon.h"
#include "../Characters/CPlayer.h"
#include "../Global.h"
#include "CBullet.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Components/TimelineComponent.h"
#include "Animation/AnimMontage.h"
#include "Camera/CameraComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundWave.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraShakeBase.h"

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

	CHelpers::GetClass<UCameraShakeBase>(&CameraShakeClass, "/Script/Engine.Blueprint'/Game/Blueprints/Weapons/BP_CamaraShake_AR4.BP_CamaraShake_AR4_C'");
	CHelpers::GetClass<ACBullet>(&BulletClass, "/Script/Engine.Blueprint'/Game/Blueprints/Weapons/BP_CBullet.BP_CBullet_C'");


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
	b |= bFiring;

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
	b |= bFiring;

	return b == false;
}

void ACWeapon::Unequip()
{
	if (HolsterSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);
}

void ACWeapon::ToggleAutoFire()
{
	bAutoFire = !bAutoFire;
}

bool ACWeapon::CanFire()
{
	bool b = false;
	b |= bEquipping;
	b |= bFiring;

	return b == false;
}

void ACWeapon::Begin_Fire()
{
	bFiring = true;
	if (bAutoFire) {

		GetWorld()->GetTimerManager().SetTimer(AutoFireHandle, this, &ACWeapon::OnFiring, AutoFireInterval, true, 0.0f);
		return;
	}
	OnFiring();
}

void ACWeapon::End_Fire()
{
	CheckFalse(bFiring);
	if (GetWorld()->GetTimerManager().IsTimerActive(AutoFireHandle))
		GetWorld()->GetTimerManager().ClearTimer(AutoFireHandle);
	bFiring = false;

}

void ACWeapon::OnFiring()
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
	
		if (!!HitDecal) {
		FRotator rotator = (-hitResult.ImpactNormal).Rotation();
		// 실제로는 그려지고 있지만 화면 사이즈로 보이지 않는다
		UDecalComponent* decal= UGameplayStatics::SpawnDecalAtLocation(GetWorld(), HitDecal, FVector(5), hitResult.Location,rotator, 10);
		// 그래서 스크린 사이즈에 의해 페이드 가려지는데 그걸 끄는거다
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

	if (!!CameraShakeClass) {
		//카메라 컨트롤이 변경 될 수 있기 때문에 아래 코드는 잘 사용 되지 않는다 그렇기 때문에 하나씩 사용한다.
		//UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0); 

		APlayerController* controller = Owner->GetController<APlayerController>();

		if (!!controller)//카메라체크
			controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);

	}

	Owner->AddControllerPitchInput(-RecoilPitch * UKismetMathLibrary::RandomFloatInRange(0.8f, 1.2f));

	if (!!BulletClass)
	{
		FVector location = Mesh->GetSocketLocation("MUzzle_Bullet"); 

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACBullet* bullet = GetWorld()->SpawnActor<ACBullet>(BulletClass, location, direction.Rotation(), params);
		
		if(!!bullet)
			bullet->Shoot(direction);
	}

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

