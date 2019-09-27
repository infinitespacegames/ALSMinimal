// Copyright is Bull****!  Do as you will with these files.  InfiniteSpaceGames 2019

#include "MiniMapGameState.h"
#include "PointOfInterest.h"
#include "MapWidget.h"



// Any registered POI will show up on any registered maps 
void AMiniMapGameState::RegisterPointOfInterest(UPointOfInterest* POI) {
	if (!POI) { return; }

	PointsOfInterest.Add(POI);
	for (UMapWidget* Map : Maps) {
		Map->AddPointOfInterest(POI);
	}
}


// Any registered map will display all registered POI's 
void AMiniMapGameState::RegisterMap(UMapWidget* Map) {
	if (!Map) { return; }

	Maps.Add(Map);
	for (UPointOfInterest* POI : PointsOfInterest) {
		Map->AddPointOfInterest(POI);
	}
}

