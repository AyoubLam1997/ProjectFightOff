// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Animation/BlendSpace1D.h"

#include "InputMappingContext.h"

#include "BaseStates.h"

#include "InputBuffer.h"

#include "Collision/UHitbox.h"
#include "Collision/HitboxHandler.h"

#include "BaseFighter.generated.h"

//TODO: ADJUST THIS TO TRANSITION TO ANY DESIRED STATE (THAT MAKES USE OF THE DATA TABLES)
USTRUCT(Blueprintable, BlueprintType)
struct FSpecialMoves
{
	GENERATED_BODY()

public:

	FSpecialMoves();

	void Init();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UFightState> m_State;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<EInputType> m_RequiredInput;

	UMotionInput m_MotionInput;
};

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API ABaseFighter : public ACharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float m_MaxHealth;
	UPROPERTY(EditAnywhere)
	float m_ForwardWalkSpeed;
	UPROPERTY(EditAnywhere)
	float m_BackwardWalkSpeed;

	UPROPERTY(BlueprintReadOnly)
	float m_CurrentHealth;

	EInputType m_ForwardInput;
	EInputType m_BackwardInput;

	UInputBuffer* m_BufferHandler;

	UFightState* m_State;

	HitboxHandler* m_HitboxHandler;

	bool m_FacingRight = 0;

	int m_FreezeTime = 0;

public:
	// Sets default values for this pawn's properties
	ABaseFighter();

	UFUNCTION(BlueprintCallable)
	void InitializeController();

	void WalkForward();
	void WalkBackward();
	void RotateTowardsDirection();
	void ResetPlayer();

	void ChangeState(UFightState* state);

	/*UFUNCTION(BlueprintCallable)
	void ChangeToStunState();*/
	UFUNCTION(BlueprintCallable)
	void ChangeToStunState(float knockback, int duration);
	UFUNCTION(BlueprintCallable)
	void ChangeToKnockbackState(float launch, float knockback);
	UFUNCTION(BlueprintCallable)
	void ChangeToGrabState();
	UFUNCTION(BlueprintCallable)
	void ChangeToGrabbedState();

	void SetFacingRight(bool side);

	UFUNCTION(BlueprintCallable)
	void StartInput();
	UFUNCTION(BlueprintCallable)
	void StopInput();

	UFUNCTION(BlueprintCallable)
	void AddSuperBar(float value);
	UFUNCTION(BlueprintCallable)
	void SetFreezeTime(int duration);
	UFUNCTION(BlueprintCallable)
	void TakeDamage(float damage);

	UFUNCTION(BlueprintCallable)
	bool InputCheck(EInputType input);

	UFUNCTION(BlueprintCallable)
	UFightState* ReturnSpecialMoveByMotion();

	UFUNCTION(BlueprintCallable)
	const bool HasHitEnemy();

	UFUNCTION(BlueprintCallable)
	UGroundedAttackState* ReturnAttackState();
	UFUNCTION(BlueprintCallable)
	UGrabState* ReturnGrabState();

	UFUNCTION(BlueprintCallable)
	UFightState* GetCurrentState();

	UInputBuffer* ReturnInputBuffer();

	HitboxHandler* ReturnHitboxHandler();

	EInputType ReturnForwardInput();
	EInputType ReturnBackwardInput();

	UFUNCTION(BlueprintCallable)
	bool IsFacingRight() const;

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;
	UFUNCTION(BlueprintCallable)
	bool IsGrounded() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	float m_MaxSuperBar = 100.f;
	float m_CurrentSuperBar;

	//UCharacterMovementComponent* Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHitbox* m_Hitbox;
	UPROPERTY(EditAnywhere)
	UHitbox* m_Grabbox;

	UPROPERTY(EditAnywhere)
	UBoxComponent* m_FighterMesh;

	/*UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* m_SkeletalMesh;*/

	UPROPERTY(EditAnywhere, Category = "Default animations")
	UBlendSpace* m_FallBlend;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_Idle;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_Crouch;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_ForwardWalk;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_BackwardWalk;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_ForwardDash;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_BackwardDash;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_Laying;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* StandingUp;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_Stunned;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_GrabStartUp;
	UPROPERTY(EditAnywhere, Category = "Default animations")
	UAnimSequence* m_GrabAttack;

	UPROPERTY(EditAnywhere)
	UHitboxResponder* m_GrabResponder;

	UPROPERTY(EditAnywhere, Category = "Normal attacks")
	TSubclassOf<UGroundedAttackState> m_LightPunch;
	UPROPERTY(EditAnywhere, Category = "Normal attacks")
	TSubclassOf<UGroundedAttackState> m_MediumPunch;
	UPROPERTY(EditAnywhere, Category = "Normal attacks")
	TSubclassOf<UGroundedAttackState> m_HeavyPunch;
	UPROPERTY(EditAnywhere, Category = "Normal attacks")
	TSubclassOf<UGroundedAttackState> m_LightKick;
	UPROPERTY(EditAnywhere, Category = "Normal attacks")
	TSubclassOf<UGroundedAttackState> m_MediumKick;
	UPROPERTY(EditAnywhere, Category = "Normal attacks")
	TSubclassOf<UGroundedAttackState> m_HeavyKick;

	UPROPERTY(EditAnywhere, Category = "Special moves")
	TArray <FSpecialMoves> m_SpecialMoves;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGrabState> m_GrabState;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UForwardDash> m_DashState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputMappingContext* MappingContext;

	USpecialMoveState* ReturnSpecialMove();

	/*UPROPERTY(Instanced, EditAnywhere, BlueprintReadWrite)
	UMotionInput* m_MotionInput;

	UPROPERTY(EditAnywhere)
	TArray<UMotionInput*> m_MotionInputs;*/

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
	const int GetCurrentAnimationFrame() const;

private:
	
	void ButtonPressed(const FInputActionValue& value, const int index);
};
