// Copyright is Bull****!  Do as you will with these files.  InfiniteSpaceGames 2019

#include "MiniMapWidget.h"
#include "MapIconWidget.h"

#include <Components/Image.h>
#include <Components/WidgetSwitcher.h>
#include <Kismet/GameplayStatics.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Materials/MaterialParameterCollection.h>
#include <Materials/MaterialParameterCollectionInstance.h>


static const auto MiniMapDataCollectionName = TEXT("/MiniMap/ParameterCollections/MiniMap_Data");
 


// Called every frame
void UMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	UUserWidget::NativeTick(MyGeometry, InDeltaTime);

	// Set appropriate map shape
	if (MapSwitcher) {
		MapSwitcher->SetActiveWidgetIndex((int32)IsRound);
	}

	// Set dynamic material texture zoom
	MapData->SetScalarParameterValue("Zoom", Zoom);

	// Get player character
	AActor* PlayerActor = (AActor*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerActor) { return; }

	// Set dynamic material texture position
	auto Location = PlayerActor->GetActorLocation();
	MapData->SetScalarParameterValue("Xord", Location.X);
	MapData->SetScalarParameterValue("Yord", Location.Y);
}


// Initialize widget
bool UMiniMapWidget::Initialize() {
	if (!UUserWidget::Initialize()) { return false; }
	if (!GetWorld()) { return false; }

	// Get material parameter collection data
	UMaterialParameterCollection* MapParams = Cast<UMaterialParameterCollection>
		(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL, MiniMapDataCollectionName));
	if (!ensure(MapParams)) { return false; }

	MapData = GetWorld()->GetParameterCollectionInstance(MapParams);
	if (!ensure(MapData)) { return false; }

	// Set material collection parameters
	MapData->SetScalarParameterValue("Dimensions", Dimensions);
	MapData->SetScalarParameterValue("MapHalfSize", HalfSize);
	MapData->SetScalarParameterValue("Zoom", Zoom);

	if (!ensure(SquareMap)) { return false; }
	SquareMapMaterial = SquareMap->GetDynamicMaterial();
	SquareMapMaterial->SetTextureParameterValue("MapImage", MapImage);

	// Verify map widgets and get dynamic materials
	if (!ensure(RoundMap)) { return false; }
	RoundMapMaterial = RoundMap->GetDynamicMaterial();
	RoundMapMaterial->SetTextureParameterValue("MapImage", MapImage);

	// Set appropriate map shape
	if (MapSwitcher) {
		MapSwitcher->SetActiveWidgetIndex((int32)IsRound);
	}

	// Player icon
	AddPlayerIcon();
	return true;
}
