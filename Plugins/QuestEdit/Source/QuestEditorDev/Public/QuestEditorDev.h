//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

#include "Styling/SlateStyle.h"

DECLARE_LOG_CATEGORY_EXTERN(QuestEditorDevLog, Log, All);

class UQuestEditorSettings;

class QuestEditorDev : public IModuleInterface
{
public:

	TSharedPtr<FSlateStyleSet> StyleSet;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline QuestEditorDev& Get()
	{
		return FModuleManager::LoadModuleChecked<QuestEditorDev>("QuestEditorDev");
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("QuestEditorDev");
	}


	/** Getter for internal settings object to support runtime configuration changes */
	UQuestEditorSettings* GetSettings() const;

	/* Register editor widget on the PerProjectUserSettings config to make the widget load on start.*/
	void RegisterQuestEditorWidget();

	/* Register editor widget on the PerProjectUserSettings config to make the widget load on start.*/
	void SubRegisterQuestEditorWidget();

protected:

	/** Quest Editor Dev Module settings */
	UQuestEditorSettings* ModuleSettings;
};