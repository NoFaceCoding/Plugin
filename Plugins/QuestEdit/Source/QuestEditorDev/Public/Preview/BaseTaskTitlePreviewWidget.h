//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "Preview/BasePreviewWidget.h"
#include "BaseTaskTitlePreviewWidget.generated.h"


class UBaseQuestEditor;

/**
 *  Base widget that contains logic for all task title preview
 */
UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "TaskTitlePreview"))
class QUESTEDITORDEV_API UBaseTaskTitlePreviewWidget : public UBasePreviewWidget
{
	GENERATED_BODY()
	
public:


};
