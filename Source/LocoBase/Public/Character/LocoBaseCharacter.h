// Copyright is Bullshit!  Do as you will with these files.

#pragma once

#include "LocoBase.h"
#include "LocoBaseEnum.h"
#include "LocoBaseStruct.h"
#include "LocoBaseMovement.h"

#include <GameFramework/Character.h>
#include <Components/TimelineComponent.h>
#include "LocoBaseCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UTimelineComponent;
class UPlayerInfoWidget;
class UHealthComponent;
class UPointOfInterest;

 
UCLASS()
class LOCOBASE_API ALocoBaseCharacter : public ACharacter {

	GENERATED_BODY()

public:

	/** Base constructor */
	ALocoBaseCharacter(const FObjectInitializer& ObjectInitializer);

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** Called for property replication */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;


private:
	////////////////////////////////////////////////////////////////////
	//
	//   Native event handling
	//
	////////////////////////////////////////////////////////////////////

	/** Handles native begin crouch event */
	virtual void OnStartCrouch(float HHAdjust, float SHHAdjust) override;

	/** Handles native end crouch event */
	virtual void OnEndCrouch(float HHAdjust, float SHHAdjust) override;

	/** Handle native landing event */
	virtual void Landed(const FHitResult& Hit) override;

	/** Handles native movement mode change event */
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCustomMode) override;


private:
	////////////////////////////////////////////////////////////////////
	//
	//   Player input handling
	//
	////////////////////////////////////////////////////////////////////

	/** Handle forward/backward control input */
	void MoveForward(float Value);

	/** Handle right/left */
	void MoveRight(float Value);

	/** Handle turn-rate control input */
	void TurnAtRate(float Rate);

	/** Handle lookup-rate control input */
	void LookUpAtRate(float Rate);

	/** Handle jump control input */
	void JumpAction();

	/** Handle stance change control input */
	void StanceAction();

	/** Handle walk control input */
	void WalkAction();

	/** Handle sprint control input */
	void BeginSprint();

	/** Handle sprint control input */
	void EndSprint();

	/** Handle aim down sights control input */
	void BeginAiming();

	/** Handle aim down sights control input */
	void EndAiming();

	/** Handle rotation mode control input */
	void SwitchRotationMode();

	/** Handle ragdoll mode control input*/
	void RagdollAction();

	/** Handle forward foot control input */
	void SwitchForwardFoot();

	/** Handle camera action control input */
	void BeginSwitchCamera();

	/** Handle camera action control input */
	void EndSwitchCamera();

	/** Handle camera action control input */
	void SwitchCamera();


public:
	////////////////////////////////////////////////////////////////////
	//
	//   Character getter functions, based on state
	//
	////////////////////////////////////////////////////////////////////

	/** Get camera settings based on character animation state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	FCameraSettings GetCameraTargetSettings() const;

	/** Get character velocity based on animation state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	FVector GetCharacterVelocity() const;

	/** Determine rotation rate from character state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetCharacterRotationRate(float SlowSpeed, float SlowRate, float FastSpeed, float FastRate);

	/** Get current aiming state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	bool IsAiming() const;

	/** Get current running state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	bool IsRunning() const;

	/** Get current sprinting state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	bool IsSprinting() const;

	/** Get current stance */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	EStance GetStance() const;

	/** Get current gait mode */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	EGaitMode GetGaitMode() const;

	/** Get current rotation mode */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	ERotationMode GetRotationMode() const;

	/** Get current locomotion mode */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	ELocomotionMode GetLocomotionMode() const;

	/** Get crouched speed configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetCrouchingSpeed() const;

	/** Get walking speed configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetWalkingSpeed() const;

	/** Get running speed configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetRunningSpeed() const;

	/** Get sprinting speed configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetSprintingSpeed() const;

	/** Get walk acceleration configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetWalkAcceleration() const;
	
	/** Get run acceleration configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetRunAcceleration() const;
	
	/** Get walk deceleration configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetWalkDeceleration() const;
	
	/** Get run deceleration configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetRunDeceleration() const;
	
	/** Get walk ground friction configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetWalkGroundFriction() const;
	
	/** Get run ground friction configuration input */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Getters")
	float GetRunGroundFriction() const;


public:
	////////////////////////////////////////////////////////////////////
	//
	//   Character setter functions
	//
	////////////////////////////////////////////////////////////////////

	/** Set the character rotation */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetCharacterRotation(FRotator NewRotation, bool bDoInterp, float InterpSpeed);

	/** Add given amount to the character rotation */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void AddCharacterRotation(FRotator AdditiveRotation);

	/** Limit amount of yaw per frame */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void LimitCharacterRotation(float AimYawLimit, float InterpSpeed);

	/** Called to update camera view mode */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetCameraMode(ECameraMode NewMode);

	/** Called to update camera gait mode for target settings */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetCameraGaitMode(float Speed);

	/** Interface callback for updating camera view shoulder */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetRightShoulder(bool bRight);

	/** Interface callback for updating aiming state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetAiming(bool bAiming);

	/** Called to update character rotation mode */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetRotationMode(ERotationMode NewMode);

	/** Called to update character locomotion mode */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetLocomotionMode(ELocomotionMode NewMode);

	/** Called to update character crouched speed */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetCrouchSpeed(float NewSpeed);

	/** Called to update character walking speed */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetWalkSpeed(float NewSpeed);

	/** Called to update character running speed */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetRunSpeed(float NewSpeed);

	/** Called to update character sprinting speed */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetSprintSpeed(float NewSpeed);

	/** Called to display debug traces */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "LocoBase|Character|Server")
	void ServerSetShowSettings(bool bShow);

	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetShowSettings(bool bShow);

	/** Called to display debug traces */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "LocoBase|Character|Server")
	void ServerSetShowTraces(bool bShow);

	/** Interface callback for updating forward foot */
	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "LocoBase|Character|Server")
	void ServerSetForwardFoot(bool bRightFoot);

	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetForwardFoot(bool bRightFoot);
	

public:
	////////////////////////////////////////////////////////////////////
	//
	//   Character utility functions
	//
	////////////////////////////////////////////////////////////////////

	/** Callback for camera lerp curves*/
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	void CameraLerpCallback(float Alpha);

	/** Updates capsule and arrow visibility */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	void UpdateCapsuleVisibility();

	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Setters")
	void SetShowTraces(bool bShow);
	
	/** Displays character state information on screen */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	void PrintCharacterInfo();

	/** Interface callback for updating camera  position */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	void UpdateCamera(UCurveFloat* LerpCurve);

	/** Create arrow scene components */
	void CreateArrowComponents();

	/** Called to update arrow positions */
	void UpdateArrowPositions();

	/** Called to update arrow vectors */
	void UpdateArrowComponents(bool bAlwaysUpdate = false);


public:
	////////////////////////////////////////////////////////////////////
	//
	//   Character calculation / management functions
	//
	////////////////////////////////////////////////////////////////////

	/** Handles adjustments to character rotation */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	void ManageCharacterRotation();

	/** Calculates the character current state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	void CalculateStateVariables();

	/** Determines the looking yaw offset */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	FRotator CalculateLookingDirection(float NEAngle, float NWAngle, float SEAngle, float SWAngle, float Buffer, float InterpSpeed);

	/** Determines if within range of given cardinal direction with a tolerance */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	float WithinCardinalRange(float Value, float Min, float Max, float Tol, ECardinalDirection Cardinal);

	/** Handle animNotify turn in place event */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	void DelayedRotation_Notify(FRotator AdditiveRotation, float DelayTime);

	/** Handle animNotify camera shake event */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Utility")
	void CameraShake_Notify(TSubclassOf<UCameraShake> ShakeClass, float ShakeScale);


public:
	////////////////////////////////////////////////////////////////////
	//
	//   Ragdoll management functions
	//
	////////////////////////////////////////////////////////////////////

	/** Handles the manipulation of the ragdoll state */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Ragdoll")
	void ManageRagdoll();

	/** Called to update ragdoll state information */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "LocoBase|Character|Server")
	void MulticastRagdollUpdate(bool bGrounded, FVector DollLocation, FVector NewLocation, FRotator NewRotation);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "LocoBase|Character|Server")
	void ServerRagdollUpdate(bool bGrounded, FVector DollLocation, FVector NewLocation, FRotator NewRotation);

	/** Called to enter character into ragdoll state */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "LocoBase|Character|Server")
	void MulticastEnterRagdoll();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "LocoBase|Character|Server")
	void ServerEnterRagdoll();

	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Ragdoll")
	void EnterRagdoll();

	/** Called to exit character from ragdoll state */
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "LocoBase|Character|Server")
	void MulticastExitRagdoll();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category = "LocoBase|Character|Server")
	void ServerExitRagdoll();

	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Ragdoll")
	void ExitRagdoll();

	/** Determines what it says it does :) */
	UFUNCTION(BlueprintCallable, Category = "LocoBase|Character|Ragdoll")
	bool RagdollLineTrace(FVector InLocation, FRotator InRotation, FVector& OutLocation, FRotator& OutRotation);


public:
	////////////////////////////////////////////////////////////////////
	//
	//   Character state variables
	//
	////////////////////////////////////////////////////////////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocoBase")
	FName FPCameraSocketName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocoBase")
	FName PelvisBoneName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LocoBase")
	FName RagdollPoseName;

	// TODO: Update character if changed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase")
	float HalfHeight = 90.0f;

	// TODO: Update character if changed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase")
	float CrouchedHalfHeight = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase")
	bool bToggleSprint;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase")
	bool bRightFootForward;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase|Debug")
	bool bShowSettings;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase|Debug")
	bool bShowTraces;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase|Camera")
	bool bRightShoulder;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase|Camera")
	ECameraMode CameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase|Camera")
	FCameraTargetSettings CameraTargets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LocoBase|Camera")
	TArray<UCurveFloat*> CameraLerpCurves;


public:
	////////////////////////////////////////////////////////////////////
	//
	//   Internally used character variables
	//
	////////////////////////////////////////////////////////////////////

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	bool bRagdollGrounded;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	bool bIsMoving;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	bool bHasMovementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	bool bIsRagdoll;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	ECardinalDirection CardinalDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	ELocomotionMode PrevLocomotionMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	EGaitMode CameraGaitMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	float AimYawDelta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	float AimYawRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	float MovementDifferential;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	float RotationDifferential;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	float YawDifferential;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	float RotationOffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	float RotationRateMultiplier;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FVector MovementInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FVector RagdollLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FRotator CharacterRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FRotator LookRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FRotator TargetRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FRotator JumpRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FRotator PrevVelocityRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "LocoBase|InternalVariables")
	FRotator PrevMovementRotation;

	UPROPERTY()
	FCameraSettings CurrentCameraSettings;


protected:
	////////////////////////////////////////////////////////////////////
	//
	//   Character components
	//
	////////////////////////////////////////////////////////////////////

	/** 3P camera boom */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* TP_SpringArm;

	/** Third person following camera view */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* TP_Camera;

	/** First person camera view */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* FP_Camera;

	/** Scene arrow components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Arrows;

	/** Looking direction vector */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* LookingRotationArrow;

	/** Target direction vector */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* TargetRotationArrow;

	/** Character direction vector */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* CharacterRotationArrow;

	/** Movement input direction vector */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* MovementInputArrow;

	/** Previous movement input vector */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* PrevMovementInputArrow;

	/** Velocity direction vector */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* VelocityArrow;

	/** Previous velocity vector */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* PrevVelocityArrow;

	/** Player character info display widget */
	UPROPERTY(BlueprintReadWrite)
	UPlayerInfoWidget* PlayerInfoWidget;


private:

	/** General timer for held inputs */
	FTimerHandle InputTimer;

	/** Used in playing blueprint curves */
	FOnTimelineFloat onCameraLerpCallback;

	/** Camera timeline */
	UPROPERTY()
	UTimelineComponent* CameraLerpTimeline;

public:

	FORCEINLINE ULocoBaseMovement* GetLocoBaseMovement() const { return Cast<ULocoBaseMovement>(GetCharacterMovement()); };

};

