#include "../Characters/CAnimInstance_Arms.h"
#include "../Global.h"
#include "CPlayer.h"

void UCAnimInstance_Arms::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACPlayer> (TryGetPawnOwner());
	CheckNull(OwnerCharacter); 

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(Weapon);


	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance_Arms::OnWeaponTypeChanged);

}

void UCAnimInstance_Arms::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// 만약 캐릭터가 없어도 실행이 잘동은 안하지만 실행은 되지 않는다 그걸 방지 

	CheckNull(OwnerCharacter);
 } 
void UCAnimInstance_Arms::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{

	WeaponType = InNewType;

}
