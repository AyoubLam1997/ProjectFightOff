// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Characters/BaseFighter.h"
#include "Camera/CameraActor.h"

#include "FightingGameMode.generated.h"

enum EFighGameState
{
	P1Intro = 0,
	P2Intro,
	Countdown,
	Fighting,
	RoundWin,
	GameEnd,
	Paused
};

/**
 * 
 */
UCLASS()
class MYPROJECT_API AFightingGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void SetPlayerRoundStart();

	void AirToGroundCollisionCheck();

	void SwitchGameState(EFighGameState state);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseFighter> m_FighterToSpawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ABaseFighter* m_P1Fighter;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ABaseFighter* m_P2Fighter;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ACameraActor* m_Camera;

	EFighGameState m_GameState;

	float m_CurrentTimer;
	float m_MaxTimer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxLevelSize;
};

UCLASS()
class MYPROJECT_API APlayArea : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseFighter> m_FighterToSpawn;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ABaseFighter* m_P1Fighter;

	void BeginPlay() override;
	void Tick(float DeltaTime) override;
};