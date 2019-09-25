// Copyright is Bullshit!  Do as you will with these files.

#pragma once

#include "LocoBase.h"
#include "LocoBaseEnum.h"

#include <GameFramework/CharacterMovementComponent.h>
#include "LocoBaseMovement.generated.h"

class FNetworkPredictionData_Client;
class FNetworkPredictionData_Server;

 

/**
 *  Implements a custom character movement component
 */
UCLASS()
class LOCOBASE_API ULocoBaseMovement : public UCharacterMovementComponent {

	GENERATED_BODY()

	friend class FSavedMove_LocoBaseCharacter;

public:

	/** Base constructor */
	ULocoBaseMovement(const FObjectInitializer& ObjectInitializer);

	///////////////////////////////////////////////////////////////////////////////////////
	//
	//  Parent override functions
	//
	///////////////////////////////////////////////////////////////////////////////////////

	/** Called every frame */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Called for property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/** Set input flags on character from saved inputs - Client only */
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	/** Client prediction data */
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	/** Server prediction data */
	virtual FNetworkPredictionData_Server* GetPredictionData_Server() const override;

	/** Stores movement input before consuming */
	virtual FVector ConsumeInputVector() override;

	/** Max speed based on state */
	virtual float GetMaxSpeed() const override;
	
	/** Max acceleration based on state */
	virtual float GetMaxAcceleration() const override;
	
	/** Max deceleration based on state */
	virtual float GetMaxBrakingDeceleration() const override;


	///////////////////////////////////////////////////////////////////////////////////////
	//
	//  Getter functions
	//
	///////////////////////////////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	bool IsRunning() const { return bWantsToRun; }

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	bool IsSprinting() const { return bWantsToSprint; }

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	bool IsAiming() const { return bWantsToAim; }

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	FVector GetMovementInput() const { return MovementInput; }
	
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	FRotator GetControlRotation() const { return ControlRotation; };

	/** Crouch speed configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetCrouchSpeed() const { return CrouchSpeed; };

	/** Walk speed configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetWalkSpeed() const { return WalkSpeed; };

	/** Run speed configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetRunSpeed() const { return RunSpeed; };

	/** Sprint speed configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetSprintSpeed() const { return SprintSpeed; };

	/** Walk acceleration configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetWalkAcceleration() const { return WalkAcceleration; };

	/** Run acceleration configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetRunAcceleration() const { return RunAcceleration; };

	/** Walk deceleration configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetWalkDeceleration() const { return WalkDeceleration; };

	/** Run deceleration configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetRunDeceleration() const { return RunDeceleration; };

	/** Walk ground friction configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetWalkGroundFriction() const { return WalkGroundFriction; };

	/** Run ground friction configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetRunGroundFriction() const { return RunGroundFriction; };

	/** Speed determined by gait, locomotion mode not considered */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetGroundSpeed() const;

	/** Crouched speed based on state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetCrouchedSpeed() const;
	
	/** Friction based on state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	float GetGroundFriction() const;

	/** Stance of character: TODO: Remove, just use IsCrouching */
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	EStance GetStance() const;

	/** Current gait mode based on state*/
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	EGaitMode GetGaitMode() const;

	/** Current locomotion mode based on state*/
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	ELocomotionMode GetLocomotionMode() const;

	/** Current locomotion mode based on state*/
	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	ELocomotionMode ConvertMovementMode(EMovementMode Mode) const;

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	ERotationMode GetRotationMode() const { return RotationMode; };


public:
	///////////////////////////////////////////////////////////////////////////////////////
	//
	//  Networked setter functions
	//
	///////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void WantsToRun(bool bRunning) { bWantsToRun = bRunning; };

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void WantsToSprint(bool bSprinting) { bWantsToSprint = bSprinting; };

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void WantsToAim(bool bAiming) { bWantsToAim = bAiming; };

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCrouchSpeed(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetCrouchSpeed(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetWalkSpeed(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetWalkSpeed(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetRunSpeed(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetRunSpeed(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSprintSpeed(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetSprintSpeed(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetWalkAcceleration(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetWalkAcceleration(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetRunAcceleration(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetRunAcceleration(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetWalkDeceleration(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetWalkDeceleration(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetRunDeceleration(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetRunDeceleration(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetWalkGroundFriction(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetWalkGroundFriction(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetRunGroundFriction(float NewValue);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetRunGroundFriction(float NewValue);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetRotationMode(ERotationMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetRotationMode(ERotationMode NewMode);

	UFUNCTION(BlueprintCallable, Category = "LocoBase")
	void SetCustomAcceleration();



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float BaseTurnRate;


protected:
	///////////////////////////////////////////////////////////////////////////////////////
	//
	//  Utility functions
	//
	///////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetInputVector(const FVector& Input);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetControlRotation();


protected:
	////////////////////////////////////////////////////////////////////
	//
	//   User configuration variables
	//
	////////////////////////////////////////////////////////////////////

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float CrouchSpeed;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float WalkSpeed;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float RunSpeed;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float SprintSpeed;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float WalkAcceleration;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float RunAcceleration;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float WalkDeceleration;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float RunDeceleration;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float WalkGroundFriction;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Character Movement: LocoBase")
	float RunGroundFriction;


protected:
	////////////////////////////////////////////////////////////////////
	//
	//   Internal state variables
	//
	////////////////////////////////////////////////////////////////////

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement: LocoBase|Internal")
	FVector_NetQuantize100 MovementInput;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement: LocoBase|Internal")
	FRotator ControlRotation;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Character Movement: LocoBase|Internal")
	ERotationMode RotationMode;

private:

	/** Custom state flags */
	UPROPERTY(Replicated)
	uint8 bWantsToRun : 1;

	UPROPERTY(Replicated)
	uint8 bWantsToSprint : 1;

	UPROPERTY(Replicated)
	uint8 bWantsToAim : 1;

};



/** 
 *  Implements custom saved move class
 */
class FSavedMove_LocoBaseCharacter : public FSavedMove_Character {

public:

	typedef FSavedMove_Character Super;

	/** Resets all saved variables */
	virtual void Clear() override;

	/** Store input commands in the compressed flags */
	virtual uint8 GetCompressedFlags() const override;

	/** This is used to check whether or not two moves can be combined into one
	    Basically you just check to make sure that the saved variables are the same */
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

	/** Sets up the move before sending it to the server */
	virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
	
	/** Sets variables on character movement component before making a predictive correction */
	virtual void PrepMoveFor(class ACharacter* Character) override;

	uint8 bSavedWantsToRun : 1;
	uint8 bSavedWantsToSprint : 1;
	uint8 bSavedWantsToAim : 1;
};



/**
 *  Implements custom client prediction data class
 */
class FNetworkPredictionData_Client_LocoBaseCharacter : public FNetworkPredictionData_Client_Character {

public:

	typedef FNetworkPredictionData_Client_Character Super;

	FNetworkPredictionData_Client_LocoBaseCharacter(const UCharacterMovementComponent& ClientMovement);

	virtual FSavedMovePtr AllocateNewMove() override;
};



/**
 *  Implements cusstom server prediction data class
 */
class FNetworkPredictionData_Server_LocoBaseCharacter : public FNetworkPredictionData_Server_Character {

public:

	typedef FNetworkPredictionData_Server_Character Super;

	FNetworkPredictionData_Server_LocoBaseCharacter(const UCharacterMovementComponent& ServerMovement);
};

