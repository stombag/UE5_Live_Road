#include "../Characters/CPlayer.h"
#include "../Global.h"
#include "CAnimInstance.h"
#include "../Weapons/CWeapon.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"


ACPlayer::ACPlayer()
{
	//SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	//SpringArm->SetupAttachment(GetCapsuleComponent());
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);

	//ConstructorHelpers::FObjectFinder<USkeletalMesh>asset(L"");

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Characters/Heroes/Mannequin/Meshes/SKM_Manny.SKM_Manny'");
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/Characters/ABP_CPlayer.ABP_CPlayer'");
	

	bUseControllerRotationYaw = false; //z���� ������� �ʴ´�
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 400;

	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

//�׽�Ʈ
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	// ī�޶� �ޱ� ����
	GetController<APlayerController>()->PlayerCameraManager->ViewPitchMin = PitchAngle.X;
	GetController<APlayerController>()->PlayerCameraManager->ViewPitchMax = PitchAngle.Y;

	if (!!WeaponClass) 
	{
		FActorSpawnParameters params;
		params.Owner = this;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Weapon = GetWorld()->SpawnActor<ACWeapon>(WeaponClass, params);
	}
}



void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	PlayerInputComponent->BindAction("Run",EInputEvent::IE_Pressed, this, &ACPlayer::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released,this, &ACPlayer::OffRun);

	CheckNull(Weapon);
	PlayerInputComponent->BindAction("AR4", EInputEvent::IE_Pressed, Weapon, &ACWeapon::Equip);

}

void ACPlayer::OnMoveForward(float InAxisValue)
{
	FRotator rotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotation).GetForwardVector();

	AddMovementInput(direction, InAxisValue);
}

void ACPlayer::OnMoveRight(float InAxisValue)
{
	FRotator rotation = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotation).GetRightVector();
	
	AddMovementInput(direction, InAxisValue);
}

void ACPlayer::OnHorizontalLook(float InAxisValue)
{
	AddControllerYawInput(InAxisValue);

}

void ACPlayer::OnVerticalLook(float InAxisValue)
{
	AddControllerPitchInput(InAxisValue);
}

void ACPlayer::OnRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;

}

void ACPlayer::OffRun()
{

	GetCharacterMovement()->MaxWalkSpeed = 400;
}




