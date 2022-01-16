// Copyright Yazj, Inc. All Rights Reserved.


#include "DarkSoulGameModeBase.h"
#include "DarkSoulGameState.h"
#include "DarkSoulPlayerController.h"

ADarkSoulGameModeBase::ADarkSoulGameModeBase()
{
	GameStateClass = ADarkSoulGameState::StaticClass();
	PlayerControllerClass = ADarkSoulPlayerController::StaticClass();
	bGameOver = false;
}

void ADarkSoulGameModeBase::ResetLevel()
{
	K2_DoRestart();
}

bool ADarkSoulGameModeBase::HasMatchEnded() const
{
	return bGameOver;
}

void ADarkSoulGameModeBase::GameOver()
{
	if (bGameOver == false)
	{
		K2_OnGameOver();
		bGameOver = true;
	}
}
