#include "../Weapons/CWeapon.h"
#include "../Global.h"

#include "CBullet.h"
#include "../Characters/CPlayer.h"
#include "../Weapons/CMagazine.h"
#include "../Widgets/CUserWidget_CrossHair.h"
#include "../EventActor/CTargetActor.h"

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

	CHelpers::GetClass<UCUserWidget_CrossHair>(&CrossHairClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_CrossHair.WB_CrossHair_C'");
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

	CurrentMagazinCount = MaxMagazineCount; 

	if (!!CrossHairClass)
	{
		CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController*>(Owner->GetController<APlayerController>(), CrossHairClass);
	}

}

void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LastAddSpreadTime >= 0.0f) {
		if (GetWorld() -> GetTimeSeconds()- LastAddSpreadTime >= AutoFireInterval + 0.25f)
	{
		CurrentSpreadRadius = 0.0f;
		LastAddSpreadTime = 0.0f;
		if (!!CrossHair)
			CrossHair->UpdateSpreadRange(CurrentSpreadRadius);
		}
	}
}

bool ACWeapon::CanEquip()
{
	bool b = false;
	b |= bEquipping;
	b |= bFiring;
	b |= bReload;
	// 달릴때 장전 X
	if (Owner)
        b |= Owner->IsRunning();

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

	if (!!CrossHair) {
		// 제거가 안된 경우 제거하고 추가한다. 
		if (CrossHair->IsInViewport())
			CrossHair->RemoveFromParent();

		CrossHair->AddToViewport();
		CrossHair->SetMaxSpreadRadius(MaxSpreadRadius);
		CrossHair->UpdateSpreadRange(0.0f);
	}

}

bool ACWeapon::CanUnequip()
{	
	bool b = false;
	b |= bEquipping;
	b |= bFiring;
	b |= bReload;
	if (Owner)
        b |= Owner->IsRunning();

	return b == false;
}

void ACWeapon::Unequip()
{
	if (HolsterSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);
	if (!!CrossHair)
		CrossHair->RemoveFromParent();

}

void ACWeapon::ToggleAutoFire()
{
	CheckTrue(bFiring);
	bAutoFire = !bAutoFire;
}

bool ACWeapon::CanFire()
{
	bool b = false;
	b |= bEquipping;
	b |= bFiring;
	b |= bReload;

	//달릴때 사격 X
	if (Owner)
        b |= Owner->IsRunning();
	// 점프 할때도 사격 X



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
		
		// 과녁 맟추기 
		AActor* hitActor = hitResult.GetActor();
		if (!!hitActor)
		{
			ACTargetActor* target = Cast<ACTargetActor>(hitActor);
			if (target)
			{
				target->HitTarget();
			}
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

	// 총알 날라가는 위치 
	if (!!BulletClass)
	{
		FVector location = Mesh->GetSocketLocation("Muzzle"); 

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ACBullet* bullet = GetWorld()->SpawnActor<ACBullet>(BulletClass, location, direction.Rotation(), params);
		
		if(!!bullet)
			bullet->Shoot(direction);
	}
	CurrentMagazinCount--;
	if (CurrentMagazinCount == 0) {
		// 재장전 
		if (CanReload())
			Reload();

	}
	if (CurrentSpreadRadius <= 1.0f)
	{
		CurrentSpreadRadius += SpreadSpeed * GetWorld()->GetDeltaSeconds();

		if (!!CrossHair)
			CrossHair->UpdateSpreadRange(CurrentSpreadRadius);
		
	}
	LastAddSpreadTime = GetWorld()->GetTimeSeconds();


}

bool ACWeapon::CanAim()
{
	bool b = false;
	b |= bEquipping;
	b |= bInAim;
	b |= bReload;

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

bool ACWeapon::CanReload()
{
	bool b = false;
	b |= bEquipping;
	b |= bReload;
	// 달릴때 장전 X
	if (Owner)
        b |= Owner->IsRunning();
	return b == false;
}
void ACWeapon::Reload() 
{
	bReload = true;
	End_Aim();
	End_Fire();

	if (!!ReloadMontage)
		Owner->PlayAnimMontage(ReloadMontage, ReloadMontage_PlayRate);
}

void ACWeapon::Eject_Magazine()
{
	if (HideMagazinBoneName.IsValid()) {
		Mesh->HideBoneByName(HideMagazinBoneName, EPhysBodyOp::PBO_None);
	}

	CheckNull(MagazineClass);

	FTransform transform = Mesh->GetSocketTransform(HideMagazinBoneName);
	ACMagazine* magazine = GetWorld()->SpawnActorDeferred<ACMagazine>(MagazineClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	magazine->SetEject();
	magazine->SetLifeSpan(5);
	magazine->FinishSpawning(transform); 


}

void ACWeapon::Spawn_Magazine()
{
	CheckNull(MagazineClass);

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Magazine = GetWorld()->SpawnActor<ACMagazine>(MagazineClass, params);
	CHelpers::AttachTo(Magazine, Owner->GetMesh(), SpawnMagazineSocketName);
}

void ACWeapon::Load_Magazine()
{
	CurrentMagazinCount = MaxMagazineCount;

	if (HideMagazinBoneName.IsValid()) {
		Mesh->UnHideBoneByName(HideMagazinBoneName);
	}
	if (!!Magazine)
		Magazine->Destroy();
}

void ACWeapon::End_Reload()
{
	bReload = false;
}




