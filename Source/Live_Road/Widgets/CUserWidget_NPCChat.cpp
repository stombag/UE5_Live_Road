#include "../Widgets/CUserWidget_NPCChat.h"
#include "Components/TextBlock.h"

void UCUserWidget_NPCChat::SetNPCMessage(const FString& Message)
{
	if (NPCMessage) {

		NPCMessage->SetText(FText::FromString(Message));
	}

}
