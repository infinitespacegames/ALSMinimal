// Copyright is Bull****!  Do as you will with these files.  InfiniteSpaceGames 2019

#pragma once

#include "CoreMinimal.h"
#include <Blueprint/UserWidget.h>
#include "MapIconWidget.generated.h"

class UImage;
class UThrobber;
class UMapWidget;


/**
 * Implements widget for icon display on map  
 */
UCLASS(Category = "MiniMap")
class MINIMAPMODULE_API UMapIconWidget : public UUserWidget {

	GENERATED_BODY()

public:

	/** Called every frame */
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** Setup map icon parameters */
	void Setup(const FColor& IconTint, UTexture2D* IconImage, UMapWidget* InMap, AActor* InActor, bool bAlwaysShow);

protected:

	/** Initialize user widget */
	virtual bool Initialize() override;

private:

	/** Determines if icon disappears when off map */
	UPROPERTY()
	bool bAlwaysRelevant = true;

	/** Selected map icon */
	UPROPERTY()
	UWidget* CurrentIcon;

	/** Default map icon */
	UPROPERTY(meta = (BindWidget))
	UThrobber* DefaultIcon = nullptr;

	/** Custom map icon image */
	UPROPERTY(meta = (BindWidget))
	UImage* CustomIcon = nullptr;

	/** Owning actor */
	UPROPERTY()
	AActor* MyActor = nullptr;

	/** Map this icon is attached to */
	UPROPERTY()
	UMapWidget* MyMap = nullptr;
};

