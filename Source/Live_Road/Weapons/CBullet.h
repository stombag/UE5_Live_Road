// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBullet.generated.h"

UCLASS()
class LIVE_ROAD_API ACBullet : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

private:
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* Projectile;

public:	
	ACBullet();

protected:
	virtual void BeginPlay() override;

public:	
	void Shoot(const FVector& InDirection);

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
