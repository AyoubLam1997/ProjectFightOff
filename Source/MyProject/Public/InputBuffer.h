// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "InputMappingContext.h"

#include "InputBuffer.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EInputType
{
	None = 0,
	Top,
	TopRight,
	Right,
	DownRight,
	Down,
	DownLeft,
	Left,
	TopLeft,
	LightPunch,
	MediumPunch,
	HeavyPunch,
	LightKick,
	MediumKick,
	HeavyKick
};

FString InputToString(EInputType type);

EInputType InputFromString(FString string);

class MYPROJECT_API UMotionInput
{
public:

	UMotionInput();
	UMotionInput(TArray<EInputType> motions);
	~UMotionInput() {};

	/*UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EInputType> InputDirection;*/

	TArray<EInputType> m_MotionInputs;

	int BufferTime;
	int IndexCheck;

	bool MotionUsed;

	void BufferCheck();
	void IncreaseIndex();
	void ResetInput();

	bool MotionComplete();
	bool InputCheck(EInputType input);
};

class MYPROJECT_API UInputStateItem
{
public:

	bool CanExecute();
	bool CanMotionExecute();

	void HoldUp();
	void ReleasedUp();
	void SetHoldUsed(int time, bool used, bool motion);
	void SetUsedTrue();
	void SetMotionTrue();

	int m_HoldTime = 0;

	bool m_IsUsed = 0;
	bool m_MotionUsed = 0;
};

class MYPROJECT_API UInputBufferItem
{
public:

	UInputBufferItem();
	UInputBufferItem(EInputType direction);
	~UInputBufferItem() {};

	void AssignDirection(EInputType direction);
	void SetHoldUsed(int index, int time, bool used, bool motion);

	void InputCheck();

	void SetUsedTrue();
	void SetUsedFalse();

	void SetInputActionPressed(bool pressed);

	EInputType InputDirection;

	UInputAction* m_Action;

	bool m_Used = 0;

	bool m_InputActionPressed = 0;

	TArray<UInputStateItem> m_Buffer;

protected:

};

class MYPROJECT_API UInputBuffer
{
public:

	static const int m_BufferWindow = 12;

	UInputBuffer();
	UInputBuffer(TArray<UMotionInput*>& inputs);
	~UInputBuffer();

	TArray<UInputBufferItem*> m_InputBufferItems;
	TArray<UInputAction*> m_InputActions;
	TArray<UMotionInput*> m_MotionInputs;

	void Initialize();
	void BufferUpdate();

	void add(TArray<UMotionInput*>& inputs);

	void UpdateMotion(bool right);
};
