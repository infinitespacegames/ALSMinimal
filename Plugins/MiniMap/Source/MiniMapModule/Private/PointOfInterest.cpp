// Copyright is Bull****!  Do as you will with these files.  InfiniteSpaceGames 2019

#include "PointOfInterest.h"
#include "MiniMapGameState.h"



// Sets default values for this component's properties
UPointOfInterest::UPointOfInterest() {
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts or spawn
void UPointOfInterest::BeginPlay() {
	Super::BeginPlay();

	if (GetWorld()) {
		auto GameState = Cast<AMiniMapGameState>(GetWorld()->GetGameState());
		if (GameState) {
			GameState->RegisterPointOfInterest(this);
		}
	}
}

