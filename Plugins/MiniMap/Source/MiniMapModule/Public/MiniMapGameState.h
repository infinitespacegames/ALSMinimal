// Copyright is Bull****!  Do as you will with these files.  InfiniteSpaceGames 2019

#pragma once

#include "CoreMinimal.h"
#include <GameFramework/GameStateBase.h>
#include "MiniMapGameState.generated.h"

class UPointOfInterest;
class UMapWidget;


/**
 *  Implements base GameState for storing global information across server / clients
 */
UCLASS( ClassGroup = (MiniMap) )
class MINIMAPMODULE_API AMiniMapGameState : public AGameStateBase {

	GENERATED_BODY()

public:

	/** Any registered POI will show up on any registered maps */
	UFUNCTION(BlueprintCallable)
	void RegisterPointOfInterest(UPointOfInterest* POI);

	/** Any registered map will display all registered POI's */
	UFUNCTION(BlueprintCallable)
	void RegisterMap(UMapWidget* Map);

private:

	UPROPERTY()
	TArray<UPointOfInterest*> PointsOfInterest;

	UPROPERTY()
	TArray<UMapWidget*> Maps;

};

