// Copyright Yazj, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DarkSoulGameModeBase.generated.h"

/**
 *	Base class for GameMode, should be blueprinted
 */
UCLASS()
class DARKSOUL_API ADarkSoulGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
		
public:
	/** Constructor */
	ADarkSoulGameModeBase();

	virtual void ResetLevel() override;

	/** Returns true if GameOver() has been called, false otherwise */
	virtual bool HasMatchEnded() const override;

	/** Called when the game is over i.e. the player dies, the time runs out or the game is finished */
	UFUNCTION(BlueprintCallable, Category = Game)
	virtual void GameOver();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category = Game, meta = (DisplayName = "DoRestart", ScriptName = "DoRestart"))
	void K2_DoRestart();

	UFUNCTION(BlueprintImplementableEvent, Category = Game, meta = (DisplayName = "OnGameOver", ScriptName = "OnGameOver"))
	void K2_OnGameOver();

	// bit-field
	UPROPERTY(BlueprintReadOnly, Category = Game)
	uint32 bGameOver : 1;
};
