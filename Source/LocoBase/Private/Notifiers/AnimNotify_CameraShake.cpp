// Copyright is Bullshit!  Do as you will with these files.

#include "AnimNotify_CameraShake.h"
#include "LocoBaseCharacter.h"

#include <Camera/CameraShake.h>


void UAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	ALocoBaseCharacter* MyOwner = Cast<ALocoBaseCharacter>(MeshComp->GetOwner());
	if (!MyOwner) { return; }

	MyOwner->CameraShake_Notify(ShakeClass, ShakeScale);
}
 