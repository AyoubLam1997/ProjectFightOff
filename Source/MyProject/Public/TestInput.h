// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"

#include "TestInput.generated.h"

UCLASS()
class MYPROJECT_API ATestInput : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATestInput();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enhanced Input")
	UInputAction* LightAttackAction;


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void wow(const FInputActionValue& value);
};
