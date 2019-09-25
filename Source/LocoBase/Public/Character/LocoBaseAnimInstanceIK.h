// Copyright is Bullshit!  Do as you will with these files.

#pragma once

#include "LocoBase.h"
#include "LocoBaseInterfaceABPIK.h"

#include <Animation/AnimInstance.h>
#include "LocoBaseAnimInstanceIK.generated.h"

class ALocoBaseCharacter;



/**
 *  Implements interface between character and IK animation blueprint
 */
UCLASS()
class LOCOBASE_API ULocoBaseAnimInstanceIK : public UAnimInstance, public ILocoBaseInterfaceABPIK {

	GENERATED_BODY()

public:
	
	/** Setup default properties */
	ULocoBaseAnimInstanceIK();
	
	/** Called at start of play */
	virtual void NativeInitializeAnimation() override;

	/** Called every frame */
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;


protected:
	////////////////////////////////////////////////////////////////////
	//
	//   Inverse kinematic functions, can be overriden in blueprint or C++
	//
	////////////////////////////////////////////////////////////////////
	
	/** Handles IK for normal modes */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "LocoBase|Animation")
	void FootIK();
	virtual void FootIK_Implementation();

	/** Handles IK when ragdolling */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "LocoBase|Animation")
	void RagdollIK();
	virtual void RagdollIK_Implementation();


public:
	////////////////////////////////////////////////////////////////////
	//
	//   Interface functions to needed character variables
	//
	////////////////////////////////////////////////////////////////////

	/** Calls all interface functions to synchronize animation state with character*/
	UFUNCTION(BlueprintCallable, Category = "LocoBase|ABPIK_Interface")
	void UpdateAnimationState();

	/** Called to update animation debug traces state */
	virtual void OnSetShowTraces_Implementation(bool bShow) { bShowTraces = bShow; };

	/** Called to update animation debug traces state */
	virtual void OnSetIsRagdoll_Implementation(bool bRagdoll) { bIsRagdoll = bRagdoll; };

	/** Called to update animation stance */
	virtual void OnSetStance_Implementation(EStance NewStance) { Stance = NewStance; };

	/** Called to update animation locomotion mode */
	virtual void OnSetLocomotionMode_Implementation(ELocomotionMode NewMode) { LocomotionMode = NewMode; };


protected:
	////////////////////////////////////////////////////////////////////
	//
	//   User variables
	//
	////////////////////////////////////////////////////////////////////
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocoBase")
	ALocoBaseCharacter* Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocoBase")
	FName LeftFootBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocoBase")
	FName RightFootBoneName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocoBase")
	float FootIKAlpha;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocoBase")
	float RotationInterpolationSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocoBase")
	float Z_InterpolationSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocoBase|Linetrace")
	float TraceHeightAboveFoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocoBase|Linetrace")
	float TraceHeightBelowFoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocoBase|Limits")
	FVector StandingMinLimits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocoBase|Limits")
	FVector StandingMaxLimits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocoBase|Limits")
	FVector CrouchingMinLimits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocoBase|Limits")
	FVector CrouchingMaxLimits;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|CharacterState")
	bool bShowTraces;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|CharacterState")
	bool bIsRagdoll;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|CharacterState")
	EStance Stance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|CharacterState")
	ELocomotionMode LocomotionMode;


protected:
	////////////////////////////////////////////////////////////////////
	//
	//   Internal animation blueprint variables
	//
	////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	bool bEnableFootIK;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	float PelvisOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FVector LeftFootOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FVector RightFootOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FTransform LeftFootTransform;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FTransform RightFootTransform;

};
