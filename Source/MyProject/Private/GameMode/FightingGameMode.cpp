// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/FightingGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

void AFightingGameMode::BeginPlay()
{
	//UWorld::GetFirstLocalPlayerFromController();

	FVector pos1 = FVector(0, -125, 0);
	FVector pos2 = FVector(0, 125, 0);
	FRotator rot1 = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters(); 
	SpawnInfo.bNoFail = true;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	m_P1Fighter = GetWorld()->SpawnActor<ABaseFighter>(m_FighterToSpawn, pos1, rot1, SpawnInfo);
	// DEV NOTE: This isn't the way to set the camera to the player. Update in the future
	GetWorld()->GetPlayerControllerIterator()->Get(0)->Possess(m_P1Fighter);
	m_P1Fighter->InitializeController();

	m_P1Fighter->StopInput();

	m_P2Fighter = GetWorld()->SpawnActor<ABaseFighter>(m_FighterToSpawn, pos2, rot1, SpawnInfo);
	//GetWorld()->GetPlayerControllerIterator()->Get(1)->Possess(m_P2Fighter);

	TArray<APlayerController> cons;

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController, &cons);

	m_P2Fighter->PossessedBy(m_P2Fighter->AIControllerClass.GetDefaultObject());
	
	m_P2Fighter->InitializeController();

	m_P2Fighter->StopInput();

	float y = (m_P1Fighter->GetActorLocation().Y + m_P2Fighter->GetActorLocation().Y) / 2.f;

	FVector cameraPos = FVector(-500, y, 90);

	FTransform transform(FRotator{ -5.f, 0.f, 0.f }, cameraPos, FVector{ 1.f, 1.f, 1.f });
	UWorld* poWorld = GetWorld();
	m_Camera = poWorld->SpawnActor<ACameraActor>();
	m_Camera->SetActorTransform(transform);

	UCameraComponent* cam = m_Camera->FindComponentByClass<UCameraComponent>();

	/*cam->ProjectionMode = ECameraProjectionMode::Orthographic;
	cam->OrthoWidth = 2000.f;*/

	// PERSONAL NOTE: I HATE HOW UNREAL HANDLES CAMERAS. THIS DUMB & STUPID & DUMB
	APlayerController* playerController = Cast<APlayerController>(m_P1Fighter->Controller);

	playerController->SetViewTarget(m_Camera);

	m_CurrentTimer = 1.5f;

	m_GameState = EFighGameState::Countdown;

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	Super::BeginPlay();
}

void AFightingGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch(m_GameState)
	{
	case EFighGameState::Countdown:
	{
		m_CurrentTimer -= DeltaTime;

		if (m_CurrentTimer <= 0)
			SwitchGameState(EFighGameState::Fighting);
	}
		break;
	case EFighGameState::Fighting:

		if (!m_P1Fighter->IsAlive() || !m_P2Fighter->IsAlive())
		{
			m_CurrentTimer = 1.5f;

			SwitchGameState(EFighGameState::RoundWin);
		}

		break;
	case EFighGameState::RoundWin:

		m_CurrentTimer -= DeltaTime;

		if (m_CurrentTimer <= 0)
		{
			m_CurrentTimer = 1.5f;

			m_P1Fighter->ResetPlayer();
			m_P2Fighter->ResetPlayer();

			SetPlayerRoundStart();

			SwitchGameState(EFighGameState::Countdown);
		}

		break;
	}

	if(m_P1Fighter->GetActorLocation().Y < m_P2Fighter->GetActorLocation().Y && !m_P1Fighter->IsFacingRight())
	{
		m_P1Fighter->SetFacingRight(1);
		m_P2Fighter->SetFacingRight(0);
	}
	else if (m_P1Fighter->GetActorLocation().Y > m_P2Fighter->GetActorLocation().Y && m_P1Fighter->IsFacingRight())
	{
		m_P1Fighter->SetFacingRight(0);
		m_P2Fighter->SetFacingRight(1);
	}

	FVector loc1 = m_P1Fighter->GetActorLocation();
	FVector loc2 = m_P2Fighter->GetActorLocation();

	float dis = fabsf(loc1.Y - loc2.Y);

	if(dis > 2600.f)
	{
		if (loc1.Y >= loc2.Y)
		{
			loc1.Y = m_Camera->GetActorLocation().Y + 1300.f;
			loc2.Y = m_Camera->GetActorLocation().Y - 1300.f;
		}
		else if (loc1.Y <= loc2.Y)
		{
			loc1.Y = m_Camera->GetActorLocation().Y - 1300.f;
			loc2.Y = m_Camera->GetActorLocation().Y + 1300.f;
		}
	}

	if (loc1.Y > 1200.f)
		loc1.Y = 1200.f;
	if (loc1.Y < -1200.f)
		loc1.Y = -1200.f;
	if (loc2.Y > 1200.f)
		loc2.Y = 1200.f;
	if (loc2.Y < -1200.f)
		loc2.Y = -1200.f;

	m_P1Fighter->SetActorLocation(loc1);
	m_P2Fighter->SetActorLocation(loc2);

	float totalLocY = (loc1.Y + loc2.Y) / 2.f;

	if (totalLocY > -MaxLevelSize && totalLocY < MaxLevelSize)
	{
		float y = (m_P1Fighter->GetActorLocation().Y + m_P2Fighter->GetActorLocation().Y) / 2.f;

		FVector cameraPos = FVector(-500, y, 90);

		m_Camera->SetActorLocation(cameraPos);
	}
	else
	{
		if (totalLocY < -MaxLevelSize)
		{
			FVector cameraPos = FVector(-500, -MaxLevelSize, 90);

			m_Camera->SetActorLocation(cameraPos);
		}
		else if (totalLocY > MaxLevelSize)
		{
			FVector cameraPos = FVector(-500, MaxLevelSize, 90);

			m_Camera->SetActorLocation(cameraPos);
		}
	}

	//AirToGroundCollisionCheck();
}

void AFightingGameMode::SetPlayerRoundStart()
{
	FVector pos1 = FVector(0, -200, 0);
	FVector pos2 = FVector(0, 200, 0);

	m_P1Fighter->SetActorLocation(pos1);
	m_P2Fighter->SetActorLocation(pos2);
}

void AFightingGameMode::AirToGroundCollisionCheck()
{
	ABaseFighter* top = nullptr;
	ABaseFighter* bottom = nullptr;

	if ((m_P1Fighter->GetActorLocation().Z + m_P1Fighter->m_FighterMesh->GetUnscaledBoxExtent().Z) > (m_P2Fighter->GetActorLocation().Z + m_P2Fighter->m_FighterMesh->GetUnscaledBoxExtent().Z))
	{
		top = m_P1Fighter;
		bottom = m_P2Fighter;
	}
	else if ((m_P2Fighter->GetActorLocation().Z + m_P2Fighter->m_FighterMesh->GetUnscaledBoxExtent().Z) > (m_P1Fighter->GetActorLocation().Z + m_P1Fighter->m_FighterMesh->GetUnscaledBoxExtent().Z))
	{
		top = m_P2Fighter;
		bottom = m_P1Fighter;
	}

	if (top == nullptr)
		return;

	TArray<AActor*> actors;

	top->m_FighterMesh->GetOverlappingActors(actors);

	if(actors.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Top detecting collision"));

		if(actors.Contains(bottom))
		{
			FString s = FString::SanitizeFloat(top->GetActorLocation().Z + top->m_FighterMesh->GetUnscaledBoxExtent().Z);
			FString s2 = FString::SanitizeFloat(bottom->GetActorLocation().Z + bottom->m_FighterMesh->GetUnscaledBoxExtent().Z);

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *s);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, *s2);

			// TODO: Adjust distance based on the size of the pushbox & the distance between the fighters
			if(top->GetActorLocation().Z == bottom->GetActorLocation().Z)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Heights are equal"));
			}
			if ((top->GetActorLocation().Y < bottom->GetActorLocation().Y) && (top->GetActorLocation().Z != bottom->GetActorLocation().Z))
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Higher"));

				auto leftLoc = top->GetActorLocation();
				float leftY = leftLoc.Y - top->m_FighterMesh->GetUnscaledBoxExtent().Y;

				auto rightLox = bottom->GetActorLocation();
				float rightY = rightLox.Y + bottom->m_FighterMesh->GetUnscaledBoxExtent().Y;

				leftLoc.Y = leftY;
				rightLox.Y = rightY;

				top->SetActorLocation(leftLoc);
				bottom->SetActorLocation(rightLox);
			}
			else if (top->GetActorLocation().Y > bottom->GetActorLocation().Y && top->GetActorLocation().Z != bottom->GetActorLocation().Z)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Lower"));

				auto leftLoc = top->GetActorLocation();
				float leftY = leftLoc.Y + top->m_FighterMesh->GetUnscaledBoxExtent().Y;

				auto rightLox = bottom->GetActorLocation();
				float rightY = rightLox.Y - bottom->m_FighterMesh->GetUnscaledBoxExtent().Y;

				leftLoc.Y = leftY;
				rightLox.Y = rightY;

				top->SetActorLocation(leftLoc);
				bottom->SetActorLocation(rightLox);
			}
		}

		actors.Empty();
	}
}

void AFightingGameMode::SwitchGameState(EFighGameState state)
{
	if(state == EFighGameState::Fighting)
	{
		m_P1Fighter->StartInput();
		m_P2Fighter->StartInput();
	}
	else
	{
		m_P1Fighter->StopInput();
		m_P2Fighter->StopInput();
	}

	m_GameState = state;
}


void APlayArea::BeginPlay()
{
	FVector pos1 = FVector(0, -200, 0);
	FVector pos2 = FVector(0, 200, 0);
	FRotator rot1 = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnInfo = FActorSpawnParameters();

	m_P1Fighter = GetWorld()->SpawnActor<ABaseFighter>(m_FighterToSpawn, pos1, rot1, SpawnInfo);
	// DEV NOTE: This isn't the way to set the camera to the player. Update in the future
	GetWorld()->GetPlayerControllerIterator()->Get(0)->Possess(m_P1Fighter);
	m_P1Fighter->InitializeController();

	m_P1Fighter->StartInput();
}

void APlayArea::Tick(float DeltaTime)
{

}
