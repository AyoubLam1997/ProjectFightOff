// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseFighter.h"
#include "Engine/GameEngine.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include <Kismet/KismetStringLibrary.h>

FSpecialMoves::FSpecialMoves()
{
	//m_MotionInput->m_MotionInputs = m_RequiredInput;
}

void FSpecialMoves::Init()
{
	m_MotionInput.m_MotionInputs = m_RequiredInput;
}

// Sets default values
ABaseFighter::ABaseFighter()
{
	//FScriptDelegate ScriptDelegate;
	//ScriptDelegate.BindUFunction(this, FName("JumpOverlap"));
	//m_FighterMesh->OnComponentHit.Add(ScriptDelegate);

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	/*comp = CreateDefaultSubobject<USceneComponent>(TEXT("Comp"));
	RootComponent = comp;*/
	
	m_FighterMesh = CreateDefaultSubobject<UBoxComponent>(TEXT("FighterMesh"));
	m_FighterMesh->SetupAttachment(GetMesh());
	//RootComponent = comp;
	//m_FighterMesh->SetupAttachment(comp);

	//SetRootComponent(m_FighterMesh);

	//m_SkeletalMesh = FindComponentByClass<USkeletalMeshComponent>();
	//m_SkeletalMesh->SetupAttachment(m_FighterMesh);
	/*m_SkeletalMesh->AddLocalOffset(FVector(0, 0, 0 - (Hurtbox->GetUnscaledBoxExtent().Z / 2.f)));*/

	m_BufferHandler = new UInputBuffer();

	//m_BufferHandler = NewObject<UInputBuffer>();

	//m_BufferHandler->add(m_MotionInputs);

	m_ForwardInput = EInputType::Right;
	m_BackwardInput = EInputType::Left;
}

// Called when the game starts or when spawned
void ABaseFighter::BeginPlay()
{
	//m_BufferHandler->Initialize();

	Super::BeginPlay();

	//m_Grabbox->AssignHitboxHandler(m_HitboxHandler);

	m_CurrentHealth = m_MaxHealth;
	m_CurrentSuperBar = 0;

	m_State = NewObject <UGroundedState>();
	m_State->AddToRoot();
	m_State->Enter(*this);

	m_Hitbox = FindComponentByClass<UHitbox>();
	//m_Grabbox = Cast<UHitbox>(GetDefaultSubobjectByName(TEXT("GrabBox")));

	m_HitboxHandler = new HitboxHandler();

	m_Hitbox->AssignHitboxHandler(m_HitboxHandler);

	TArray<EInputType> input;

	input.Add(EInputType::Right);
	input.Add(EInputType::Right);

	UMotionInput* dashInput = new UMotionInput();

	dashInput->m_MotionInputs = input;

	m_BufferHandler->m_MotionInputs.Add(dashInput);

	for (int i = 0; i < m_SpecialMoves.Num(); i++)
	{
		m_SpecialMoves[i].Init();
		m_BufferHandler->m_MotionInputs.Add(&m_SpecialMoves[i].m_MotionInput);
	}

	//m_FighterMesh->OnComponentHit.AddDynamic(this, &ABaseFighter::OnHit);
	//Movement = FindComponentByClass<UCharacterMovementComponent>();

	//Movement->UpdatedComponent = Hurtbox;
}

void ABaseFighter::InitializeController()
{
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
			SubSystem->AddMappingContext(MappingContext, 0);
		else
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No enhanced input"));
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No controller to control"));
}

// Called every frame
void ABaseFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*GroundedState* ground = Cast<GroundedState>(m_State);

	bool exists = (ground != nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, UKismetStringLibrary::Conv_BoolToString(exists));*/

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(m_CurrentHealth));

	m_BufferHandler->BufferUpdate();
	m_BufferHandler->UpdateMotion(m_FacingRight);

	for (int i = 0; i < m_BufferHandler->m_InputBufferItems.Num(); i++)
	{
		if (m_BufferHandler->m_InputBufferItems[i]->InputDirection == EInputType::LightPunch)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, UKismetStringLibrary::Conv_BoolToString(m_BufferHandler->m_InputBufferItems[i]->m_Buffer[0].m_IsUsed));
		}
	}

	if(m_FreezeTime > 0)
	{
		m_FreezeTime -= 1;

		GetMesh()->GlobalAnimRateScale = 0.f;
		GetCharacterMovement()->Velocity = FVector::Zero();

		return;
	}

	if(GetMesh()->GlobalAnimRateScale == 0.f)
		GetMesh()->GlobalAnimRateScale = 1.f;

	/*for (int i = 0; i < m_BufferHandler->m_InputBufferItems.Num(); i++)
	{
		UInputBufferItem* item = m_BufferHandler->m_InputBufferItems[i];

		if (item->InputDirection == EInputType::LightPunch && item->m_Buffer[0].m_HoldTime == 1)
		{
			ChangeToStunState(125, 50);
		}
	}*/

	if (IsValid(m_State))
	{
		m_State->Update(*this);

		UFightState* newState = m_State->HandleInput(*this);

		if (IsValid(newState))
		{
			m_State->Exit(*this);

			m_State->RemoveFromRoot();

			UFightState* stateToDestroy = m_State;
			m_State = nullptr;
			m_State = newState;

			m_State->AddToRoot();
			m_State->Enter(*this);
		}
	}

	SetActorRotation(FQuat(0, 0, 0, 0));

	FVector loc = GetActorLocation();

	/*if (loc.Y > 2000.f)
	{
		loc.Y = 2000.f;

		m_FighterMesh->GetBodyInstance()->bLockYTranslation = true;
	}
	if (loc.Y < -2000.f)
	{
		loc.Y = -2000.f;
	}*/

	SetActorLocation(loc);
}

// Called to bind functionality to input
void ABaseFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	for (int i = 0; i < MappingContext->GetMappings().Num(); i++)
	{
		UInputBufferItem* item = new UInputBufferItem();

		EInputType input = InputFromString(MappingContext->GetMappings()[i].Action.GetName());

		if(input != EInputType::None)
		{
			item->AssignDirection(input);

			m_BufferHandler->m_InputBufferItems.Add(item);

			EnhancedInput->BindAction(MappingContext->GetMappings()[i].Action, ETriggerEvent::Triggered, this, &ABaseFighter::ButtonPressed, m_BufferHandler->m_InputBufferItems.Num() - 1);
			EnhancedInput->BindAction(MappingContext->GetMappings()[i].Action, ETriggerEvent::Completed, this, &ABaseFighter::ButtonPressed, m_BufferHandler->m_InputBufferItems.Num() - 1);
		}
	}
}

void ABaseFighter::WalkForward()
{
	if (m_FighterMesh != nullptr)
	{
		float dir = 0;

		if (m_FacingRight)
			dir = m_ForwardWalkSpeed;
		else if (!m_FacingRight)
			dir = -m_ForwardWalkSpeed;

		//m_FighterMesh->SetPhysicsLinearVelocity(FVector(0, dir, 0));

		GetCharacterMovement()->Velocity = (FVector(0, dir, 0));
	}
}

void ABaseFighter::WalkBackward()
{
	if (m_FighterMesh != nullptr)
	{
		float dir = 0;

		if (m_FacingRight)
			dir = -m_BackwardWalkSpeed;
		else if (!m_FacingRight)
			dir = m_BackwardWalkSpeed;

		//m_FighterMesh->SetPhysicsLinearVelocity(FVector(0, dir, 0));

		GetCharacterMovement()->Velocity = (FVector(0, dir, 0));
	}
}

void ABaseFighter::RotateTowardsDirection()
{
	if (m_FacingRight)
	{
		GetCapsuleComponent()->SetWorldScale3D(FVector(1, 1, 1));
	}
	else if (!m_FacingRight)
	{
		GetCapsuleComponent()->SetWorldScale3D(FVector(1, -1, 1));
	}
}

void ABaseFighter::ResetPlayer()
{
	m_CurrentHealth = m_MaxHealth;

	ChangeState(NewObject<UGroundedState>());
}

void ABaseFighter::ChangeState(UFightState* state)
{
	m_State->Exit(*this);

	m_State->RemoveFromRoot();

	UFightState* stateToDestroy = m_State;
	m_State = nullptr;
	m_State = state;

	m_State->AddToRoot();
	m_State->Enter(*this);
}

//void ABaseFighter::ChangeToStunState()
//{
//	//ChangeState(NewObject<UKnockbackStunState>());
//
//	UAirStunState* state = NewObject<UAirStunState>();
//	//state->Init(1500, 400);
//	//m_State->NextState = state;
//
//	ChangeState(NewObject<UKnockbackStunState>());
//}

void ABaseFighter::ChangeToStunState(float knockback, int duration)
{
	//ChangeState(NewObject<UKnockbackStunState>());

	UKnockbackStunState* state = NewObject<UKnockbackStunState>();
	//state->Init(1500, 400);
	//m_State->NextState = state;
	state->Init(knockback, duration);
	ChangeState(state);
}

void ABaseFighter::ChangeToKnockbackState(float launch, float knockback)
{
	//ChangeState(NewObject<UKnockbackStunState>());

	UAirStunState* state = NewObject<UAirStunState>();
	//state->Init(1500, 400);
	//m_State->NextState = state;
	state->Init(launch, knockback);
	ChangeState(state);
}

void ABaseFighter::ChangeToGrabState()
{
	ChangeState(DuplicateObject(m_GrabState.GetDefaultObject(), nullptr));
}

void ABaseFighter::ChangeToGrabbedState()
{
	//ChangeState(new KnockbackStunState());
}

void ABaseFighter::ButtonPressed(const FInputActionValue& value, const int index)
{
	const bool pressed = value.Get<bool>();

	m_BufferHandler->m_InputBufferItems[index]->SetInputActionPressed(pressed);
}

void ABaseFighter::AddSuperBar(float value)
{
	m_CurrentSuperBar += value;

	if (m_CurrentSuperBar >= m_MaxSuperBar)
		m_CurrentSuperBar = m_MaxSuperBar;
}

void ABaseFighter::SetFreezeTime(int duration)
{
	m_FreezeTime = duration;
}

void ABaseFighter::TakeDamage(float damage)
{
	if (damage < 0)
		return;

	m_CurrentHealth -= damage;
}

bool ABaseFighter::InputCheck(EInputType input)
{
	for (int i = 0; i < ReturnInputBuffer()->m_InputBufferItems.Num(); i++)
	{
		if (ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num() > 0)
		{
			if (ReturnInputBuffer()->m_InputBufferItems[i]->InputDirection == input)
			{
				//DEV NOTE: 0 is the unnused state for the buffer. Start with 1 for input buffer check
				for (int j = 1; j < ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer.Num(); j++)
				{
					if (ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[j].CanExecute())
					{
						ReturnInputBuffer()->m_InputBufferItems[i]->m_Buffer[j].SetUsedTrue();

						// For debugging purposes
						int value = m_BufferHandler->m_InputBufferItems[i]->m_Buffer[j].m_HoldTime;
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Entering attack state in buffer ") + InputToString(input) + ": Index " + FString::FromInt(j));

						return 1;
					}
				}
			}
			else
				continue;
		}
	}

	return 0;
}

UFightState* ABaseFighter::ReturnSpecialMoveByMotion()
{
	for (int i = 0; i < m_SpecialMoves.Num(); i++)
	{
		if(m_SpecialMoves[i].m_MotionInput.MotionComplete())
			return DuplicateObject(m_SpecialMoves[i].m_State.GetDefaultObject(), nullptr);
	}

	return nullptr;
}

const bool ABaseFighter::HasHitEnemy()
{
	if (m_HitboxHandler->ReturnCollidedActors().Num() > 0)
		return 1;

	return 0;
}

UFightState* ABaseFighter::GetCurrentState()
{
	return m_State;
}

UGroundedAttackState* ABaseFighter::ReturnAttackState()
{
	// DEV NOTE: Dynamic casting needs to be more tested. Since RTTI is enabled, UE is running a tad slower.
	// The bigger issue, however, is that dynamic cast may cause inaccuracies (incorrect state/data, no state at all).
	// This is now implemented due to UE being extremely difficult with checking & returning what state the player is currently in.
	// TODO: Ask other devs for feedback/help
	UGroundedAttackState* fightState = Cast<UGroundedAttackState>(m_State);
	bool exists = (fightState != nullptr);

	if (exists)
		return fightState;

	return nullptr;
}

UGrabState* ABaseFighter::ReturnGrabState()
{
	// DEV NOTE: Dynamic casting needs to be more tested. Since RTTI is enabled, UE is running a tad slower.
	// The bigger issue, however, is that dynamic cast may cause inaccuracies (incorrect state/data, no state at all).
	// This is now implemented due to UE being extremely difficult with checking & returning what state the player is currently in.
	// TODO: Ask other devs for feedback/help
	UGrabState* fightState = Cast<UGrabState>(m_State);
	bool exists = (fightState != nullptr);

	if (exists)
		return fightState;

	return nullptr;
}

bool ABaseFighter::IsAlive() const
{
	if (m_CurrentHealth > 0)
		return 1;

	return 0;
}

bool ABaseFighter::IsFacingRight() const
{
	return m_FacingRight;
}

UInputBuffer* ABaseFighter::ReturnInputBuffer()
{
	return m_BufferHandler;
}

HitboxHandler* ABaseFighter::ReturnHitboxHandler()
{
	return m_HitboxHandler;
}

EInputType ABaseFighter::ReturnForwardInput()
{
	return m_ForwardInput;
}

EInputType ABaseFighter::ReturnBackwardInput()
{
	return m_BackwardInput;
}

void ABaseFighter::SetFacingRight(bool side)
{
	m_FacingRight = side;

	if (m_FacingRight)
	{
		m_ForwardInput = EInputType::Right;
		m_BackwardInput = EInputType::Left;
	}
	else if (!m_FacingRight)
	{
		m_ForwardInput = EInputType::Left;
		m_BackwardInput = EInputType::Right;
	}
}

bool ABaseFighter::IsGrounded() const
{
	if (m_FighterMesh != nullptr)
	{
		auto loc = GetActorLocation().Z;

		if (loc > 0)
			return 0;
		else if (loc <= 0)
			return 1;
	}

	return 0;
}

const int ABaseFighter::GetCurrentAnimationFrame() const
{
	return (int)(GetMesh()->GetPosition() * 60);
}

void ABaseFighter::StartInput()
{
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Enabling input"));
		EnableInput(playerController);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No input to enable"));
}

void ABaseFighter::StopInput()
{
	if (APlayerController* playerController = Cast<APlayerController>(Controller))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Disabling input"));
		DisableInput(playerController);
	}
	else
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No input to disable"));
}

// TODO: Fix stutter look when players collide during the check
void ABaseFighter::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ABaseFighter* top = nullptr;
	ABaseFighter* bottom = Cast<ABaseFighter>(OtherActor);

	FVector loc = GetActorLocation();

	if(!m_FacingRight && loc.Y >= 2000)
	{
		double dis = fabsf(bottom->GetActorLocation().Y - loc.Y);

		double col = ((m_FighterMesh->GetUnscaledBoxExtent().Y / 2) + (bottom->m_FighterMesh->GetUnscaledBoxExtent().Y / 2));

		double difference = dis - col;

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Box: ") + FString::SanitizeFloat(m_FighterMesh->GetUnscaledBoxExtent().Y));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Distance: ") + FString::SanitizeFloat(dis));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Collision: ") + FString::SanitizeFloat(col));
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Difference: ") + FString::SanitizeFloat(difference));

		if (dis < col)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("lmao 2"));
		{
			bottom->SetActorLocation(FVector(bottom->GetActorLocation().X, GetActorLocation().Y - (m_FighterMesh->GetUnscaledBoxExtent().Y * 2), bottom->GetActorLocation().Z));
			bottom->m_FighterMesh->SetPhysicsLinearVelocity(FVector::Zero());
		}
	}

	//ABaseFighter* temp = Cast<ABaseFighter>(OtherActor);

	//if (temp != nullptr)
	//{
	//	if ((this->GetActorLocation().Z + this->m_FighterMesh->GetUnscaledBoxExtent().Z) > (OtherActor->GetActorLocation().Z + temp->m_FighterMesh->GetUnscaledBoxExtent().Z))
	//	{
	//		top = this;
	//		bottom = temp;
	//	}
	//	else if ((OtherActor->GetActorLocation().Z + temp->m_FighterMesh->GetUnscaledBoxExtent().Z) > (this->GetActorLocation().Z + this->m_FighterMesh->GetUnscaledBoxExtent().Z))
	//	{
	//		top = temp;
	//		bottom = this;
	//	}

	//	if (top == nullptr || top != this)
	//		return;

	//	/*FString s = FString::SanitizeFloat(top->GetActorLocation().Z + top->m_FighterMesh->GetUnscaledBoxExtent().Z);
	//	FString s2 = FString::SanitizeFloat(bottom->GetActorLocation().Z + bottom->m_FighterMesh->GetUnscaledBoxExtent().Z);

	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *s);
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *s2);*/

	//	// TODO: Adjust distance based on the size of the pushbox & the distance between the fighters
	//	if ((top->GetActorLocation().Y < bottom->GetActorLocation().Y) && (top->GetActorLocation().Z != bottom->GetActorLocation().Z) && top->GetCurrentState() == dynamic_cast<UAirborneState*>(top->GetCurrentState()) && (top->m_FighterMesh->GetComponentVelocity().Z < 0))
	//	{
	//		float distance = bottom->GetActorLocation().Y - top->GetActorLocation().Y;

	//		float col = (top->m_FighterMesh->GetUnscaledBoxExtent().Y) + (bottom->m_FighterMesh->GetUnscaledBoxExtent().Y);

	//		float dif = col - distance;

	//		auto leftLoc = top->GetActorLocation();
	//		float leftY = leftLoc.Y - (dif / 2);

	//		auto rightLox = bottom->GetActorLocation();
	//		float rightY = rightLox.Y + (dif / 2);

	//		leftLoc.Y = leftY;
	//		rightLox.Y = rightY;

	//		top->SetActorLocation(leftLoc);
	//		bottom->SetActorLocation(rightLox);
	//	}
	//	else if (top->GetActorLocation().Y > bottom->GetActorLocation().Y && top->GetActorLocation().Z != bottom->GetActorLocation().Z && top->GetCurrentState() == dynamic_cast<UAirborneState*>(top->GetCurrentState()) && (top->m_FighterMesh->GetComponentVelocity().Z < 0))
	//	{
	//		float distance = top->GetActorLocation().Y - bottom->GetActorLocation().Y;

	//		float col = (top->m_FighterMesh->GetUnscaledBoxExtent().Y) + (bottom->m_FighterMesh->GetUnscaledBoxExtent().Y);

	//		float dif = col - distance;

	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(distance));
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(col));
	//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::SanitizeFloat(dif));

	//		auto leftLoc = top->GetActorLocation();
	//		float leftY = leftLoc.Y + (dif / 2);

	//		auto rightLox = bottom->GetActorLocation();
	//		float rightY = rightLox.Y - (dif / 2);

	//		leftLoc.Y = leftY;
	//		rightLox.Y = rightY;

	//		top->SetActorLocation(leftLoc);
	//		bottom->SetActorLocation(rightLox);
	//	}
	//}
	//else
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No temp fighter"));
}

USpecialMoveState* ABaseFighter::ReturnSpecialMove()
{
	/*for(int i = 0; i < m_SpecialMoves.Num(); i++)
	{
		if (m_SpecialMoves[i].GetDefaultObject()->m_MotionInput->MotionComplete())
			return m_SpecialMoves[i];
	}*/

	return nullptr;
}
