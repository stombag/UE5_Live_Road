// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//bool일 경우 : 인수값이 ture면 종료한다.
#define CheckTrue(x) {if(x==true) return;}
// x가 ture이면 RetValue값을 리턴한다.
#define CheckTrueResult(x,y) {if(x==true)return y;}
//포인터의 경우 : x가 nullptr이면 종료한다. 
#define CheckFalse(x) {if(x==false) return;}
#define ChcdkFalseResult(x,y) {if(x==false)return y;}

#define CheckNull(x){if(x==nullptr) return;}
#define ChcdkNullResult(x,y) {if(x==nullptr)return y;}
// 매크로는 한줄이여야한다. 그래서 역슬래쉬를 사용한다.역슬레위는 한줄로 생각하겠다는것과 같다.
/* 그래서 매크로에서 한줄 이상일때  \  을 사용해서 한줄로 만든다.*/
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
	// AActor* InActor: 어떤 액터에 컴퍼넌트를 붙일지 지정한다. 보통 이 함수를 호출 할때 이곳을 this로 넘긴다.
	// T** OutComponent :  생성된 컴포넌트를 호출한 쪽 변수에 넘겨주기 위함이다. 
	//                     함수 안에서 만든 포인터를 외부 변수에 대입하려면 참조가 필요하다. 
	// FName : 고유 이름으로 서브 이름이 중복될 수 있으므로 넣는다.
	// 기본값은 nullptr로 전달 안하면 자동으로 RootComponentfh 된다     
	// #추가 : OutComponent에 주소값을 넣으면 그 Component의 주소값을 가리키는 포인터에 가리키는 포인터에 접근한다. 
	template<typename T>
	static void CreateComponent(AActor* InActor, T** OutComponent, FName InName, USceneComponent* InParent = nullptr)
	{
		// inName를 디폴트로 네임에 넣는다.
		*OutComponent = InActor->CreateDefaultSubobject<T>(InName);
		if (!!InParent) { //포인터에서는 nullptr이면 false, nullptr가 아니면 ture이다 . 
			// !inParent -> 이건 InParent가 nullptr이면 true이고, !!InParent는 두번 부정으로 InParent가  nullptr이 아닌지 확인하는것이다. 
			// inParent != nullptr 와 같다. 
			(*OutComponent)->SetupAttachment(InParent);
			// 즉 InParent가 존재 하면 그 부모에 붙인다.
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