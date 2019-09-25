// Copyright is Bullshit!  Do as you will with these files.

#include "AnimNotify_Pivot.h"
#include "LocoBaseAnimInstance.h"


void UAnimNotify_Pivot::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	ULocoBaseAnimInstance* AInst = Cast<ULocoBaseAnimInstance>(MeshComp->GetAnimInstance());
	if (!AInst) { return; }

	AInst->Pivot_Notify(PivotParams);
}
 