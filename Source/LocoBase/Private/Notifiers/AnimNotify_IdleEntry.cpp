// Copyright is Bullshit!  Do as you will with these files.

#include "AnimNotify_IdleEntry.h"
#include "LocoBaseAnimInstance.h"


void UAnimNotify_IdleEntry::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	ULocoBaseAnimInstance* AInst = Cast<ULocoBaseAnimInstance>(MeshComp->GetAnimInstance());
	if (!AInst) { return; }

	AInst->IdleEntry_Notify(IdleEntryState);
}
 