// Copyright is Bull****!  Do as you will with these files.  InfiniteSpaceGames 2019

#pragma once

#include "CoreMinimal.h"
#include <Components/ActorComponent.h>
#include "PointOfInterest.generated.h"

class UTexture2D;


/** 
  Implements a POI for use within maps
*/
UCLASS( ClassGroup = (MiniMap), meta=(BlueprintSpawnableComponent))
class MINIMAPMODULE_API UPointOfInterest : public UActorComponent {

	GENERATED_BODY()

public:
	
	/** Sets default values for this component's properties */
	UPointOfInterest();

protected:

	/** Called when the game starts or spawn */
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointOfInterest")
	bool bAlwaysShow = true; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointOfInterest")
	FColor IconTint = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PointOfInterest")
	UTexture2D* IconTexture = nullptr;
};
