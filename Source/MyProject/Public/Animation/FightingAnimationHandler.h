// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"

class ABaseFighter;

/**
 * 
 */
class MYPROJECT_API FightingAnimationHandler
{
public:
	FightingAnimationHandler();
	~FightingAnimationHandler();

	void UpdateAnimation(ABaseFighter& fighter);
};
