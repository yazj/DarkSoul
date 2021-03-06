// Copyright Yazj, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

/** Module interface for this game's loading screens */
class IDarkSoulLoadingScreenModule : public IModuleInterface
{
public: 
	static inline IDarkSoulLoadingScreenModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IDarkSoulLoadingScreenModule>("DarkSoulLoadingScreen");
	}

	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartInGameLoadingScreen(bool bPlayUntilStopped, float PlayTime) = 0;

	/** Stops the loading screen */
	virtual void StopInGameLoadingScreen() = 0;
};