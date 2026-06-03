
#include "../Widgets/CUserWidget_PlayerChat.h"
#include "Components/TextBlock.h"
void UCUserWidget_PlayerChat::SetPlayerMessage(const FText& Message)
{
	
	if (PlayerMessage)
	{
		PlayerMessage->SetText(Message);
	}
}
