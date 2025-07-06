//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "Preview/BasePreviewWidget.h"

#include "BaseTaskContentPreviewWidget.generated.h"

/**
 *  Base widget that contains logic for all content task preview 
 */
UCLASS(Abstract, Blueprintable, Meta = (DisplayName = "TaskContentPreview"))
class QUESTEDITORDEV_API UBaseTaskContentPreviewWidget : public UBasePreviewWidget
{
	GENERATED_BODY()
	
public:


	UBaseTaskContentPreviewWidget();

	virtual void NativeConstruct() override;
	   
};
