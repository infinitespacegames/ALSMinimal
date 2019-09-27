// Copyright is Bull****!  Do as you will with these files.  InfiniteSpaceGames 2019

#include "MapWidget.h"
#include "MapIconWidget.h"
#include "PointOfInterest.h"

#include <Components/Image.h>
#include <Components/Overlay.h>
#include <Components/OverlaySlot.h>
#include <Kismet/GameplayStatics.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Materials/MaterialParameterCollection.h>
#include <Materials/MaterialParameterCollectionInstance.h>

static const auto MapDataCollectionName = TEXT("/MiniMap/ParameterCollections/Map_Data");



// Called every frame
void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime){
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Set dynamic material texture zoom
	MapData->SetScalarParameterValue("Zoom", Zoom);
}


// Initialize widget
bool UMapWidget::Initialize() {
	if (!Super::Initialize()) { return false; }
	if (!GetWorld()) { return false; }

	// Get material parameter collection data
	UMaterialParameterCollection* MapParams = Cast<UMaterialParameterCollection>
		(StaticLoadObject(UMaterialParameterCollection::StaticClass(), NULL, MapDataCollectionName));
	if (!ensure(MapParams)) { return false; }

	// Get map data from collection
	MapData = GetWorld()->GetParameterCollectionInstance(MapParams);
	if (!ensure(MapData)) { return false; }

	// Set material collection parameters
	MapData->SetScalarParameterValue("Dimensions", Dimensions);
	MapData->SetScalarParameterValue("MapHalfSize", HalfSize);
	MapData->SetScalarParameterValue("Zoom", Zoom);

	// Set dynamic material for square map
	if (!ensure(SquareMap)) { return false; }
	SquareMapMaterial = SquareMap->GetDynamicMaterial();
	SquareMapMaterial->SetTextureParameterValue("MapImage", MapImage);

	// Player icon
	AddPlayerIcon();
	return true;
}


// Adds player icon to map
void UMapWidget::AddPlayerIcon() {

	// Create
	UMapIconWidget* PlayerIconWidget = CreateWidget<UMapIconWidget>(GetWorld(), MapIconClass);
	UOverlaySlot* OverlaySlot = MapOverlay->AddChildToOverlay(PlayerIconWidget);

	// Align
	if (OverlaySlot) {
		OverlaySlot->SetHorizontalAlignment(HAlign_Center);
		OverlaySlot->SetVerticalAlignment(VAlign_Center);
	}

	// Get player actor, ok if null
	AActor* PlayerActor = (AActor*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	// Setup
	if (PlayerIconWidget) {
		PlayerIconWidget->Setup(PlayerIconTint, PlayerIcon, this, PlayerActor, true);
	}
}


//  Adds point of interest icon to map
void UMapWidget::AddMapIcon(const FColor& IconTint, UTexture2D* IconTexture, AActor* Actor, bool bAlwaysShow) {
	if (!Actor) { return; }

	// Local player doesn't need POI icon
	if (GetPlayerContext().IsValid() && GetPlayerContext().IsFromLocalPlayer(Actor)) { return; }

	// Create
	UMapIconWidget* MapIconWidget = CreateWidget<UMapIconWidget>(GetWorld(), MapIconClass);
	UOverlaySlot* OverlaySlot = MapOverlay->AddChildToOverlay(MapIconWidget);
	
	// Align
	OverlaySlot->SetHorizontalAlignment(HAlign_Center);
	OverlaySlot->SetVerticalAlignment(VAlign_Center);

	// Setup
	MapIconWidget->Setup(IconTint, IconTexture, this, Actor, bAlwaysShow);
}


// Convert world position to map coordinates
bool UMapWidget::GetMapLocation(const FVector& WorldLocation, FVector2D& MapLocation) {
	FVector2D Center;

	// Get map center position
	MapData->GetScalarParameterValue("Xord", Center.X);
	MapData->GetScalarParameterValue("Yord", Center.Y);

	// Coordinate scale factor
	float Scale = (Zoom * Dimensions / (HalfSize * 2.0f));

	FVector2D World2D = (FVector2D(WorldLocation) - Center) / Scale;
	bool OnMap = World2D.Size() <= HalfSize;

	// Perpendicular because of texture orientation
	FVector2D Map2D(World2D.Y, -World2D.X);
	
	// Clamp to map bounds
	if (IsRound) {
		MapLocation = Map2D.GetSafeNormal() * FMath::Clamp(Map2D.Size(), 0.0f, HalfSize);
	}
	else {
		MapLocation = Map2D.ClampAxes(-HalfSize, HalfSize);
	}

	// Map coordinates
	return OnMap;
}


void UMapWidget::AddPointOfInterest(UPointOfInterest* POI) {
	AddMapIcon(POI->IconTint, POI->IconTexture, POI->GetOwner(), POI->bAlwaysShow);
}
