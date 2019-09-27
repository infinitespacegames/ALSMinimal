// Copyright is Bull****!  Do as you will with these files.  InfiniteSpaceGames 2019

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


class FMiniMapModule : public IModuleInterface {
public:

	/// IModuleInterface implementation 
	/** Called after module is loaded into memory */
	virtual void StartupModule() override;

	/** Called during shutdown or unloading to clean up the module */
	virtual void ShutdownModule() override;
};

