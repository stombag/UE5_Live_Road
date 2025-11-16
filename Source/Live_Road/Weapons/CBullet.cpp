#include "../Weapons/CBullet.h"
#include "../Global.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
ACBullet::ACBullet()
{
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Mesh",Capsule);

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this,&Projectile,"Projectile");

	Capsule->SetRelativeRotation(FRotator(90, 0, 0));
	Capsule->SetCapsuleHalfHeight(50);
	Capsule->SetCapsuleRadius(2);
	Capsule->SetCollisionProfileName("BlockAllDynamic");


	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "/Script/Engine.StaticMesh'/Game/Meshs/Sphere.Sphere'");

	Mesh->SetStaticMesh(mesh);
	Mesh->SetRelativeScale3D(FVector(1, 0.025f, 0.025f));
	Mesh->SetRelativeRotation(FRotator(90, 0, 0));
	

	UMaterialInstanceConstant* material;
	CHelpers::GetAsset<UMaterialInstanceConstant>(&material, "/Script/Engine.MaterialInstanceConstant'/Game/Materials/M_Bullet_Inst.M_Bullet_Inst'");
	Mesh->SetMaterial(0, material);


	Projectile->InitialSpeed = 2e+4f;
	Projectile->MaxSpeed = 2e+4f;
	Projectile->ProjectileGravityScale = 0.0f;




}

void ACBullet::BeginPlay()
{
	Super::BeginPlay();

	Projectile->SetActive(false);
	Capsule->OnComponentHit.AddDynamic(this, &ACBullet::OnHit);
	//AddDynamic 이 오류 났다면 그건 OnHit때문이다
}

void ACBullet::Shoot(const FVector& InDirection)
{

	//  총알 살아있는 시간 3초 뒤에 없어진다.
	SetLifeSpan(3);

	Projectile->Velocity = InDirection * Projectile->InitialSpeed;
	Projectile->SetActive(true);

}

void ACBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();

}


