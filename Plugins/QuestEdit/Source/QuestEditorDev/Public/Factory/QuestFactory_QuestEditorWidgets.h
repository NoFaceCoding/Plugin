//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "Factories/Factory.h"
#include "Engine/Blueprint.h"


#include "QuestFactory_QuestEditorWidgets.generated.h"

/**
 * 
 */



//----------------------------------------------------------------------
//-------------------------------Game widget factory
//----------------------------------------------------------------------

UCLASS()
class QUESTEDITORDEV_API UQuestFactory_QuestEditorWidgets : public UFactory
{
	GENERATED_BODY()

public:

	UQuestFactory_QuestEditorWidgets(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool IsMacroFactory() const { return false; }

public:


	// The type of blueprint that will be created
	UPROPERTY(EditAnywhere, Category = "EditorWidgetBlueprintFactory")
		TEnumAsByte<enum EBlueprintType> BlueprintType;

	// The parent class of the created blueprint
	UPROPERTY(EditAnywhere, Category = "EditorWidgetBlueprintFactory", meta = (AllowAbstract = ""))
		TSubclassOf<class UUserWidget> ParentClass;

	//~ Begin UFactory Interface
	/* New assets that don't override this function are automatically placed into the "Miscellaneous" category in the editor */
	virtual FText GetDisplayName() const override;
	virtual uint32 GetMenuCategories() const override;
	virtual bool ConfigureProperties() override;
	virtual bool ShouldShowInNewMenu() const override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
 
