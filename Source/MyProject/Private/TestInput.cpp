// Fill out your copyright notice in the Description page of Project Settings.


#include "TestInput.h"

#include "Engine/GameEngine.h"

#include "InputBuffer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ATestInput::ATestInput()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Input has been spotted"));
}

// Called when the game starts or when spawned
void ATestInput::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestInput::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestInput::wow(const FInputActionValue& value)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Input has been spotted"));
}

// Called to bind functionality to input
void ATestInput::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//InputBuffer::InputBufferItem* item{};

	//Input->BindAction(LightAttackAction, ETriggerEvent::Triggered, item, &InputBuffer::InputBufferItem::InputCheck);
}

