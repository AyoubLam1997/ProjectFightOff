// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "TestFightState.generated.h"

/**
 * 
 */

UINTERFACE(MinimalAPI, Blueprintable)
class UTestFightState : public UInterface
{
	GENERATED_BODY()
};

/* Actual Interface declaration. */
class ITestFightState
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Add interface function declarations here
};

UCLASS(Blueprintable, BlueprintType)
class UOtherTestFightState : public UPrimaryDataAsset, public ITestFightState
{
	GENERATED_BODY()

public:

};
