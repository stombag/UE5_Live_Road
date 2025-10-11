// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CGameModeBase.h"
#include "../Global.h"
ACGameModeBase::ACGameModeBase()
{
	//ConstructorHelpers::FClassFinder<APawn>asset(L"/Script/Engine.Blueprint'/Game/Character/Blueprints/BP_CPlayer.BP_CPlayer_C'");
	//DefaultPawnClass = asset.Class;

	CHelpers::GetClass<APawn>(&DefaultPawnClass, "/Script/Engine.Blueprint'/Game/Characters/BP_CPlayer.BP_CPlayer_C'");


}
