// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//bool�� ��� : �μ����� ture�� �����Ѵ�.
#define CheckTrue(x) {if(x==true) return;}
// x�� ture�̸� RetValue���� �����Ѵ�.
#define CheckTrueResult(x,y) {if(x==true)return y;}
//�������� ��� : x�� nullptr�̸� �����Ѵ�. 
#define CheckFalse(x) {if(x==false) return;}
#define ChcdkFalseResult(x,y) {if(x==false)return y;}

#define CheckNull(x){if(x==nullptr) return;}
#define ChcdkNullResult(x,y) {if(x==nullptr)return y;}
// ��ũ�δ� �����̿����Ѵ�. �׷��� ���������� ����Ѵ�.���������� ���ٷ� �����ϰڴٴ°Ͱ� ����.
/* �׷��� ��ũ�ο��� ���� �̻��϶�  \  �� ����ؼ� ���ٷ� �����.*/
#define CreateTextRenderComponent()\
{\
	CHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root);\
	Text->SetRelativeLocation(FVector(0, 0, 100));\
	Text->SetRelativeRotation(FRotator(0, 180, 0));\
	Text->SetRelativeScale3D(FVector(2));\
	Text->TextRenderColor = FColor::Red;\
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;\
	Text->Text = FText::FromString(GetName().Replace(L"Default__",L""));\
}

class LIVE_ROAD_API CHelpers
{
public:
	// AActor* InActor: � ���Ϳ� ���۳�Ʈ�� ������ �����Ѵ�. ���� �� �Լ��� ȣ�� �Ҷ� �̰��� this�� �ѱ��.
	// T** OutComponent :  ������ ������Ʈ�� ȣ���� �� ������ �Ѱ��ֱ� �����̴�. 
	//                     �Լ� �ȿ��� ���� �����͸� �ܺ� ������ �����Ϸ��� ������ �ʿ��ϴ�. 
	// FName : ���� �̸����� ���� �̸��� �ߺ��� �� �����Ƿ� �ִ´�.
	// �⺻���� nullptr�� ���� ���ϸ� �ڵ����� RootComponentfh �ȴ�     
	// #�߰� : OutComponent�� �ּҰ��� ������ �� Component�� �ּҰ��� ����Ű�� �����Ϳ� ����Ű�� �����Ϳ� �����Ѵ�. 
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr)
	{
		// inName�� ����Ʈ�� ���ӿ� �ִ´�.
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
		if (!!InParent) { //�����Ϳ����� nullptr�̸� false, nullptr�� �ƴϸ� ture�̴� . 
			// !inParent -> �̰� InParent�� nullptr�̸� true�̰�, !!InParent�� �ι� �������� InParent��  nullptr�� �ƴ��� Ȯ���ϴ°��̴�. 
			// inParent != nullptr �� ����. 
			(*OutComponent)->SetupAttachment(InParent);
			// �� InParent�� ���� �ϸ� �� �θ� ���δ�.
			return;

		}
	
		InActor->SetRootComponent(*OutComponent);

	}
	template<typename T>
	static void CreateActorComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr)
	{
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
	}




	template<typename T>
	static void GetAsset(T** Outobject, FString InPath) {
		ConstructorHelpers::FObjectFinder<T> asset(*InPath);
		*Outobject = asset.Object;

	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath) {
		ConstructorHelpers::FClassFinder<T> asset(*InPath);
		*OutClass = asset.Class;

	}
	static void AttachTo(AActor* InActor, USceneComponent* InParaent, FName InSocketName) 
	{
		InActor->AttachToComponent(InParaent, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);

	}

	
	static void Log() {
		UE_LOG(LogTemp, Warning, TEXT("Hello Check"));
	}


	

	template<typename T>
	static T* GetComponent(AActor* InActor) 
	{

		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
}; 