// Copyright is Bullshit!  Do as you will with these files.

#include "LocoBaseMovement.h"
#include <GameFramework/Character.h>
#include <UnrealNetwork.h>



// Base constructor
ULocoBaseMovement::ULocoBaseMovement(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

	// Set this character to call Tick() every frame.
	PrimaryComponentTick.bCanEverTick = true;

	bReplicates = true;

	BaseTurnRate = 150.0f;
	BaseLookUpRate = 150.0f;
	CrouchSpeed = 150.0f;
	WalkSpeed = 165.0;
	RunSpeed = 380.0f;
	SprintSpeed = 650.0f;
	WalkAcceleration = 800.0f;
	RunAcceleration = 1000.0f;
	WalkDeceleration = 800.0f;
	RunDeceleration = 800.0f;
	WalkGroundFriction = 8.0f;
	RunGroundFriction = 6.0f;
	RotationMode = ERotationMode::eLookingMode;

}


// Called every frame
void ULocoBaseMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetControlRotation();
	}

	// Update based on state
	MaxWalkSpeedCrouched = GetCrouchedSpeed();
	GroundFriction = GetGroundFriction();

	// Custom acceleration based on movement differential
	if (IsMovingOnGround() && !IsCrouching()) {
		if (IsRunning() || IsSprinting()) {
			SetCustomAcceleration();
		}
	}
}


// Called for property replication 
void ULocoBaseMovement::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	DOREPLIFETIME(ULocoBaseMovement, bWantsToRun);
	DOREPLIFETIME(ULocoBaseMovement, bWantsToSprint);
	DOREPLIFETIME(ULocoBaseMovement, bWantsToAim);
	DOREPLIFETIME(ULocoBaseMovement, MovementInput);
	DOREPLIFETIME(ULocoBaseMovement, ControlRotation);
	DOREPLIFETIME(ULocoBaseMovement, CrouchSpeed);
	DOREPLIFETIME(ULocoBaseMovement, WalkSpeed);
	DOREPLIFETIME(ULocoBaseMovement, RunSpeed);
	DOREPLIFETIME(ULocoBaseMovement, SprintSpeed);
	DOREPLIFETIME(ULocoBaseMovement, WalkAcceleration);
	DOREPLIFETIME(ULocoBaseMovement, RunAcceleration);
	DOREPLIFETIME(ULocoBaseMovement, WalkDeceleration);
	DOREPLIFETIME(ULocoBaseMovement, RunDeceleration);
	DOREPLIFETIME(ULocoBaseMovement, WalkGroundFriction);
	DOREPLIFETIME(ULocoBaseMovement, RunGroundFriction);
	DOREPLIFETIME(ULocoBaseMovement, RotationMode);
}


// Set input flags on character from saved inputs - Client only
void ULocoBaseMovement::UpdateFromCompressedFlags(uint8 Flags) {
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.

	bWantsToRun = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
	bWantsToSprint = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
	bWantsToAim = (Flags & FSavedMove_Character::FLAG_Custom_2) != 0;
}


// Client prediction data
FNetworkPredictionData_Client* ULocoBaseMovement::GetPredictionData_Client() const {
	//check(PawnOwner != NULL);

	if (!ClientPredictionData) {
		ULocoBaseMovement* MutableThis = const_cast<ULocoBaseMovement*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_LocoBaseCharacter(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}
	return ClientPredictionData;
}


// Server prediction data
FNetworkPredictionData_Server* ULocoBaseMovement::GetPredictionData_Server() const {
	//check(PawnOwner != NULL);

	if (!ServerPredictionData) {
		ULocoBaseMovement* MutableThis = const_cast<ULocoBaseMovement*>(this);
		MutableThis->ServerPredictionData = new FNetworkPredictionData_Server_LocoBaseCharacter(*this);
	}
	return ServerPredictionData;
}


// Stores movement input before consuming 
FVector ULocoBaseMovement::ConsumeInputVector() {
	FVector Input = Super::ConsumeInputVector();

	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetInputVector(Input);
	}

	return Input;
}


// Get max speed based on state 
float ULocoBaseMovement::GetMaxSpeed() const {
	if (!IsMovingOnGround()) { Super::GetMaxSpeed(); }
	return GetGroundSpeed();
}


// Max acceleration based on state
float ULocoBaseMovement::GetMaxAcceleration() const {
	if (!IsMovingOnGround()) { return Super::GetMaxAcceleration(); }
	if (IsRunning() || IsSprinting()) { return RunAcceleration; }
	return WalkAcceleration;
}


// Max deceleration based on state 
float ULocoBaseMovement::GetMaxBrakingDeceleration() const {
	if (!IsMovingOnGround()) { return Super::GetMaxBrakingDeceleration(); }
	if (IsRunning() || IsSprinting()) { return RunDeceleration; }
	return WalkDeceleration;
}



///////////////////////////////////////////////////////////////////////////////////////
//
//  Getter functions
//
///////////////////////////////////////////////////////////////////////////////////////

// 	Speed determined by gait, locomotion mode not considered 
float ULocoBaseMovement::GetGroundSpeed() const {
	
	if (IsCrouching()) { return GetCrouchedSpeed(); }
	
	if (IsAiming()) {
		if (IsSprinting()) { return RunSpeed; }
		return WalkSpeed;
	}
	
	if (IsSprinting()) { 
		if (RotationMode == ERotationMode::eVelocityMode) { return SprintSpeed; }
		return FMath::Abs((MovementInput.Rotation() - ControlRotation).GetNormalized().Yaw) > 50.0f ? RunSpeed : SprintSpeed;
	}
	
	if (IsRunning()) { return RunSpeed; }
	return WalkSpeed;
}


// Get character crouched speed based on state
float ULocoBaseMovement::GetCrouchedSpeed() const {
	if (IsSprinting()) { return CrouchSpeed + 50.0f; }
	if (IsRunning()) { return CrouchSpeed; }
	return CrouchSpeed - 50.0f;
}
 

// Get character deceleration based on state
float ULocoBaseMovement::GetGroundFriction() const {
	if (IsRunning() || IsSprinting()) { return RunGroundFriction; }
	return WalkGroundFriction;
}


// Stance of character, depreciate
EStance ULocoBaseMovement::GetStance() const {
	if (IsCrouching()) { return EStance::eCrouching; }
	return EStance::eStanding;
}


// Get current gait mode based on state
EGaitMode ULocoBaseMovement::GetGaitMode() const {
	if (IsSprinting()) { return EGaitMode::eSprinting; }
	if (IsRunning()) { return EGaitMode::eRunning; }
	return EGaitMode::eWalking;
}


// Current locomotion mode based on state 
ELocomotionMode ULocoBaseMovement::GetLocomotionMode() const {
	
	// UE4 Movement mode conversion
	switch (MovementMode) {
	case EMovementMode::MOVE_None:
	case EMovementMode::MOVE_Flying:
	case EMovementMode::MOVE_Swimming:
	case EMovementMode::MOVE_Custom:
		break;
	case EMovementMode::MOVE_Walking:
	case EMovementMode::MOVE_NavWalking:
		return ELocomotionMode::eGrounded;
	case EMovementMode::MOVE_Falling:
		return ELocomotionMode::eFalling;
	}
	return ELocomotionMode::eNone;
}


ELocomotionMode ULocoBaseMovement::ConvertMovementMode(EMovementMode Mode) const {

	// UE4 Movement mode conversion
	switch (Mode) {
	case EMovementMode::MOVE_None:
	case EMovementMode::MOVE_Flying:
	case EMovementMode::MOVE_Swimming:
	case EMovementMode::MOVE_Custom:
		break;
	case EMovementMode::MOVE_Walking:
	case EMovementMode::MOVE_NavWalking:
		return ELocomotionMode::eGrounded;
	case EMovementMode::MOVE_Falling:
		return ELocomotionMode::eFalling;
	}
	return ELocomotionMode::eNone;
}



///////////////////////////////////////////////////////////////////////////////////////
//
//  Networked setter functions
//
///////////////////////////////////////////////////////////////////////////////////////

// Crouch speed
void ULocoBaseMovement::ServerSetCrouchSpeed_Implementation(float NewValue) {
	CrouchSpeed = NewValue;
}


bool ULocoBaseMovement::ServerSetCrouchSpeed_Validate(float NewValue) {
	return true;
}


void ULocoBaseMovement::SetCrouchSpeed(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetCrouchSpeed(NewValue);
	}
}


// Walk speed
void ULocoBaseMovement::ServerSetWalkSpeed_Implementation(float NewValue) {
	WalkSpeed = NewValue;
}


bool ULocoBaseMovement::ServerSetWalkSpeed_Validate(float NewValue) {
	return true;
}


void ULocoBaseMovement::SetWalkSpeed(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetWalkSpeed(NewValue);
	}
}


// Run speed
void ULocoBaseMovement::ServerSetRunSpeed_Implementation(float NewValue) {
	RunSpeed = NewValue;
}


bool ULocoBaseMovement::ServerSetRunSpeed_Validate(float NewValue) {
	return true;
}


void ULocoBaseMovement::SetRunSpeed(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetRunSpeed(NewValue);
	}
}


// Sprint speed
void ULocoBaseMovement::ServerSetSprintSpeed_Implementation(float NewValue) {
	SprintSpeed = NewValue;
}


bool ULocoBaseMovement::ServerSetSprintSpeed_Validate(float NewValue) {
	return true;
}


void ULocoBaseMovement::SetSprintSpeed(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetSprintSpeed(NewValue);
	}
}


// Walk acceleration
void ULocoBaseMovement::ServerSetWalkAcceleration_Implementation(float NewValue) {
	WalkAcceleration = NewValue;
}


bool ULocoBaseMovement::ServerSetWalkAcceleration_Validate(float NewValue) {
	return true;
}


void ULocoBaseMovement::SetWalkAcceleration(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetWalkAcceleration(NewValue);
	}
}


// Run acceleration
void ULocoBaseMovement::ServerSetRunAcceleration_Implementation(float NewValue) {
	RunAcceleration = NewValue;
}


bool ULocoBaseMovement::ServerSetRunAcceleration_Validate(float NewValue) {
	return true;
}


void ULocoBaseMovement::SetRunAcceleration(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetRunAcceleration(NewValue);
	}
}


// Walk deceleration
void ULocoBaseMovement::ServerSetWalkDeceleration_Implementation(float NewValue) {
	WalkDeceleration = NewValue;
}


bool ULocoBaseMovement::ServerSetWalkDeceleration_Validate(float NewValue){
	return true;
}


void ULocoBaseMovement::SetWalkDeceleration(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetWalkDeceleration(NewValue);
	}
}


// Run deceleration
void ULocoBaseMovement::ServerSetRunDeceleration_Implementation(float NewValue) {
	RunDeceleration = NewValue;
}


bool ULocoBaseMovement::ServerSetRunDeceleration_Validate(float NewValue) {
	return true;

}


void ULocoBaseMovement::SetRunDeceleration(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetRunDeceleration(NewValue);
	}
}


// Walk ground friction
void ULocoBaseMovement::ServerSetWalkGroundFriction_Implementation(float NewValue) {
	WalkGroundFriction = NewValue;
}


bool ULocoBaseMovement::ServerSetWalkGroundFriction_Validate(float NewValue) {
	return true;
}


void ULocoBaseMovement::SetWalkGroundFriction(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetWalkGroundFriction(NewValue);
	}
}


// Run ground friction
void ULocoBaseMovement::ServerSetRunGroundFriction_Implementation(float NewValue) {
	RunGroundFriction = NewValue;
}


bool ULocoBaseMovement::ServerSetRunGroundFriction_Validate(float NewValue) {
	return true;
}


void ULocoBaseMovement::SetRunGroundFriction(float NewValue) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetRunGroundFriction(NewValue);
	}
}


// Get custom acceleration based on input and velocity for pivot 
void ULocoBaseMovement::SetCustomAcceleration() {

	float MovementDifferential = (MovementInput.Rotation() - Velocity.Rotation()).GetNormalized().Yaw;
	float Mult1 = FMath::GetMappedRangeValueClamped(FVector2D(45.0f, 130.0f), FVector2D(1.0, 0.2), FMath::Abs(MovementDifferential));
	float Mult2 = FMath::GetMappedRangeValueClamped(FVector2D(45.0f, 130.0f), FVector2D(1.0, 0.4), FMath::Abs(MovementDifferential));

	MaxAcceleration = GetRunAcceleration() * Mult1;
	GroundFriction = GetRunGroundFriction() * Mult2;
}


void ULocoBaseMovement::ServerSetRotationMode_Implementation(ERotationMode NewMode) {
	RotationMode = NewMode;
}


bool ULocoBaseMovement::ServerSetRotationMode_Validate(ERotationMode NewMode) {
	return true;
}


void ULocoBaseMovement::SetRotationMode(ERotationMode NewMode) {
	if (PawnOwner && PawnOwner->IsLocallyControlled()) {
		ServerSetRotationMode(NewMode);
	}
}



///////////////////////////////////////////////////////////////////////////////////////
//
//  Utility functions
//
///////////////////////////////////////////////////////////////////////////////////////

// Store current control rotation
bool ULocoBaseMovement::ServerSetControlRotation_Validate() {
	return true;
}


void ULocoBaseMovement::ServerSetControlRotation_Implementation() {
	ControlRotation = PawnOwner->GetControlRotation().GetNormalized();
}


// Store current movement input
bool ULocoBaseMovement::ServerSetInputVector_Validate(const FVector& Input) {
	return true;
}


void ULocoBaseMovement::ServerSetInputVector_Implementation(const FVector& Input) {
	MovementInput = Input;
}



///////////////////////////////////////////////////////////////////////////////////////
//
//  FSavedMove_LocoBaseCharacter functions
//
///////////////////////////////////////////////////////////////////////////////////////

// Resets all saved variables
void FSavedMove_LocoBaseCharacter::Clear() {
	Super::Clear();

	//Clear variables back to their default values.
	bSavedWantsToRun = false;
	bSavedWantsToSprint = false;
	bSavedWantsToAim = false;
}


// Store input commands in the compressed flags 
uint8 FSavedMove_LocoBaseCharacter::GetCompressedFlags() const {
	uint8 Result = Super::GetCompressedFlags();

	if (bSavedWantsToRun) {
		Result |= FLAG_Custom_0;
	}

	if (bSavedWantsToSprint) {
		Result |= FLAG_Custom_1;
	}

	if (bSavedWantsToAim) {
		Result |= FLAG_Custom_2;
	}

	return Result;
}


// This is used to check whether or not two moves can be combined into one
bool FSavedMove_LocoBaseCharacter::CanCombineWith(const FSavedMovePtr & NewMove, ACharacter * Character, float MaxDelta) const {
	
	//This pretty much just tells the engine if it can optimize by combining saved moves. 
	//There doesn't appear to be any problem with leaving it out, but it seems that it's good practice to implement this anyways.

	if (bSavedWantsToRun != ((FSavedMove_LocoBaseCharacter*)& NewMove)->bSavedWantsToRun) {
		return false;
	}

	if (bSavedWantsToSprint != ((FSavedMove_LocoBaseCharacter*)& NewMove)->bSavedWantsToSprint) {
		return false;
	}

	if (bSavedWantsToAim != ((FSavedMove_LocoBaseCharacter*)& NewMove)->bSavedWantsToAim) {
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}


// Sets up the move before sending it to the server 
void FSavedMove_LocoBaseCharacter::SetMoveFor(ACharacter * Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) {
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	// Take the input from the player and store it in the saved move.
	ULocoBaseMovement* CharMov = Cast<ULocoBaseMovement>(Character->GetCharacterMovement());
	if (CharMov) {
		bSavedWantsToRun = CharMov->bWantsToRun;
		bSavedWantsToSprint = CharMov->bWantsToSprint;
		bSavedWantsToAim = CharMov->bWantsToAim;
	}
}


// Sets variables on character movement component before making a predictive correction 
void FSavedMove_LocoBaseCharacter::PrepMoveFor(class ACharacter* Character) {
	Super::PrepMoveFor(Character);

	ULocoBaseMovement* CharMov = Cast<ULocoBaseMovement>(Character->GetCharacterMovement());
	if (CharMov) {

	}
}



///////////////////////////////////////////////////////////////////////////////////////
//
//  FNetworkPredictionData_Client_LocoBaseCharacter functions
//
///////////////////////////////////////////////////////////////////////////////////////

FNetworkPredictionData_Client_LocoBaseCharacter::FNetworkPredictionData_Client_LocoBaseCharacter(const UCharacterMovementComponent & ClientMovement)
	: Super(ClientMovement) {

}


FSavedMovePtr FNetworkPredictionData_Client_LocoBaseCharacter::AllocateNewMove() {
	return FSavedMovePtr(new FSavedMove_LocoBaseCharacter());
}



///////////////////////////////////////////////////////////////////////////////////////
//
//  FNetworkPredictionData_Server_LocoBaseCharacter functions
//
///////////////////////////////////////////////////////////////////////////////////////

FNetworkPredictionData_Server_LocoBaseCharacter::FNetworkPredictionData_Server_LocoBaseCharacter(const UCharacterMovementComponent& ServerMovement)
	: Super(ServerMovement) {

}

