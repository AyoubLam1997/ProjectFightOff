// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "InputBuffer.h"

#include "BaseStates.generated.h"

class ABaseFighter;
class UHitboxResponder;
enum class EInputType;

/**
 * 
 */

//UINTERFACE(MinimalAPI, Blueprintable)
//class UFightState : public UInterface
//{
//	GENERATED_BODY()
//};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UFightState : public UObject
{
	GENERATED_BODY()
public:

	//UFightState* NextState = nullptr;

	virtual void Enter(ABaseFighter& fighter) {};
	virtual UFightState* HandleInput(ABaseFighter& fighter) { return nullptr; };
	virtual void Update(ABaseFighter& fighter) {};
	virtual void Exit(ABaseFighter& fighter) {};
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UGroundedState : public UFightState
{
	GENERATED_BODY()
public:
	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UCrouchState : public UGroundedState
{
	GENERATED_BODY()
public:
	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API ULayingState : public UGroundedState
{
	GENERATED_BODY()
public:

	ULayingState();
	ULayingState(float duration);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:

	float m_LayingTimer;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UStandingUpState : public UGroundedState
{
	GENERATED_BODY()
public:

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UForwardWalkState : public UGroundedState
{
	GENERATED_BODY()
public:
	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UBackwardWalkState : public UGroundedState
{
	GENERATED_BODY()
public:
	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UDash : public UGroundedState
{
	GENERATED_BODY()

public:

	//Dash();
	//Dash(FVector impulse);

	virtual void Enter(ABaseFighter& fighter) override {};
	virtual UFightState* HandleInput(ABaseFighter& fighter) override { return nullptr; };
	virtual void Update(ABaseFighter& fighter) override {};
	virtual void Exit(ABaseFighter& fighter) override {};

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_AnimationSequence;

	UPROPERTY(EditAnywhere)
	FVector m_DashImpulse;

	UPROPERTY(EditAnywhere)
	int m_DashStartUp;
	UPROPERTY(EditAnywhere)
	int m_DashStartRecovery;

	int m_CurrentFrame;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UForwardDash : public UDash
{
	GENERATED_BODY()

public:

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};

//UCLASS(Blueprintable, BlueprintType)
//class MYPROJECT_API UBackwardDash : public UPrimaryDataAsset, public Dash
//{
//	GENERATED_BODY()
//
//public:
//
//	virtual void Enter(ABaseFighter& fighter) override;
//	virtual FightState* HandleInput(ABaseFighter& fighter) override;
//	virtual void Update(ABaseFighter& fighter) override;
//	virtual void Exit(ABaseFighter& fighter) override;
//};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UAirborneState : public UFightState
{
	GENERATED_BODY()
public:

	float m_CurrentFallVelocity;
	float m_MaxFallVelocity = 1500.f;

	UAirborneState();
	UAirborneState(FVector impulse);
	UAirborneState(float velocity);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:

	FVector m_ImpulseDirection;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UNeutralJumpState : public UAirborneState
{
	GENERATED_BODY()
public:

	UNeutralJumpState() {};

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UForwardJumpState : public UAirborneState
{
	GENERATED_BODY()
public:

	float m_ForwardVelocity = 400.f;

	UForwardJumpState();
	UForwardJumpState(FVector impulse);
	UForwardJumpState(float velocity);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:

	FVector m_ImpulseDirection;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UBackwardJumpState :public UAirborneState
{
	GENERATED_BODY()
public:

	float m_BackwardVelocity = -400.f;

	UBackwardJumpState();
	UBackwardJumpState(FVector impulse);
	UBackwardJumpState(float velocity);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:

	FVector m_ImpulseDirection;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UStunState : public UFightState
{
	GENERATED_BODY()
public:

	int m_CurrentStunTime;

	UStunState() {};

	virtual void Enter(ABaseFighter& fighter) override {};
	virtual UFightState* HandleInput(ABaseFighter& fighter) override { return nullptr; };
	virtual void Update(ABaseFighter& fighter) override {};
	virtual void Exit(ABaseFighter& fighter) override {};

private:
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UKnockbackStunState : public UStunState
{
	GENERATED_BODY()
public:

	float m_Pushback;
	int m_StunDuration;

	UKnockbackStunState();
	UKnockbackStunState(float pushback, int duration);

	void Init(float pushback, int duration);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UAirStunState : public UStunState
{
	GENERATED_BODY()
public:

	float m_VerticalKnockBack;
	float m_HorizontalKnockback;

	float m_MaxFallVelocity;
	float m_CurrentFallVelocity;

	UAirStunState();
	UAirStunState(float velocity, float horizontal);

	void Init(float velocity, float horizontal);

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:

	FVector m_ImpulseDirection;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UGrabStartupState : public UFightState
{
	GENERATED_BODY()
public:

	UGrabStartupState();

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

private:

	int m_CurrentFrame;

	int m_MinFrame = 5;
	int m_MaxFrame = 8;
};

// DEV NOTE: Probably make this a uclass that designers can edit
//class MYPROJECT_API GrabbedState : public FightState
//{
//public:
//
//	GrabbedState();
//
//	virtual void Enter(ABaseFighter& fighter) override;
//	virtual FightState* HandleInput(ABaseFighter& fighter) override;
//	virtual void Update(ABaseFighter& fighter) override;
//	virtual void Exit(ABaseFighter& fighter) override;
//
//private:
//};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UGrabState : public UFightState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_Damage;

	UPROPERTY(EditAnywhere)
	int m_DamageFrame;

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

	UFUNCTION(BlueprintCallable)
	void SetEnemy(ABaseFighter* fighter);

private:

	ABaseFighter* m_EnemyFighter;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UGroundedAttackState : public UGroundedState
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_Damage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float KnockBack;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int StunTime;

	UPROPERTY(EditAnywhere)
	int m_MinFrame;
	UPROPERTY(EditAnywhere)
	int m_MaxFrame;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int m_FreezeTime;

	UPROPERTY(EditAnywhere)
	UHitboxResponder* m_Responder;

	UPROPERTY(EditAnywhere)
	UAnimSequence* m_AnimationSequence;

	virtual void Enter(ABaseFighter& fighter) override;
	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
	virtual void Update(ABaseFighter& fighter) override;
	virtual void Exit(ABaseFighter& fighter) override;

	UFUNCTION(BlueprintNativeEvent)
	void AttackStateEnter(ABaseFighter* fighter);
	UFUNCTION(BlueprintNativeEvent)
	UFightState* AttackStateHandleInput(ABaseFighter* fighter);
	UFUNCTION(BlueprintNativeEvent)
	void AttackStateUpdate(ABaseFighter* fighter);
	UFUNCTION(BlueprintNativeEvent)
	void AttackStateExit(ABaseFighter* fighter);

protected:

	int m_CurrentFrame;
};

//TODO: ADJUST THIS TO TRANSITION TO ANY DESIRED STATE (THAT MAKES USE OF THE DATA TABLES)
USTRUCT(Blueprintable, BlueprintType)
struct FStateToTransition
{
	GENERATED_BODY()

public:

	FStateToTransition();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFightState> m_State;
};

USTRUCT(Blueprintable, BlueprintType)
struct FStatesToTransitionButton : public FStateToTransition
{
	GENERATED_BODY()

public:

	FStatesToTransitionButton();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EInputType m_Input;
};

//USTRUCT(Blueprintable, BlueprintType)
//struct FStatesToTransitionMotionInput : public FStateToTransition
//{
//	GENERATED_BODY()
//
//public:
//
//	TArray<EInputType> m_Inputs;
//};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API UGroundedComboAttackState : public UGroundedAttackState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FStatesToTransitionButton m_State;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int m_MinCancelFrame;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int m_MaxCancelFrame;

	virtual UFightState* HandleInput(ABaseFighter& fighter) override;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API USpecialMoveState : public UFightState
{
	GENERATED_BODY()
public:

	USpecialMoveState();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<EInputType> m_RequiredInput;

	UMotionInput* m_MotionInput;

	//UMotionInput* ReturnMotionInput();
};

//UCLASS(Blueprintable, BlueprintType)
//class MYPROJECT_API USpecialMoveState : public UPrimaryDataAsset, public USpecialMoveState
//{
//	GENERATED_BODY()
//
//public:
//};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API USpecialMoveTransitionState : public USpecialMoveState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	FStatesToTransitionButton m_State;

};
