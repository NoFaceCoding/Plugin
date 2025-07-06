//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


#include "UObject/ObjectMacros.h"
#include "Templates/SubclassOf.h"
#include "Factories/Factory.h"
#include "Engine/Blueprint.h"


#include "QuestFactory_QuestTasksErrors.generated.h"

/**
 * 
 */

class UBaseQuestTask;

//----------------------------------------------------------------------
//-------------------------------QuestTasks
//----------------------------------------------------------------------

UCLASS()
class QUESTEDITORDEV_API UQuestFactory_QuestTasksErrors : public UFactory
{
	GENERATED_BODY()

public:

	UQuestFactory_QuestTasksErrors(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool IsMacroFactory() const { return false; }

public:


	// The type of blueprint that will be created
	UPROPERTY(EditAnywhere, Category = "GameWidgetBlueprintFactory")
		TEnumAsByte<enum EBlueprintType> BlueprintType;

	// The parent class of the created blueprint
	UPROPERTY(EditAnywhere, Category = "GameWidgetBlueprintFactory", meta = (AllowAbstract = ""))
		TSubclassOf<class UObject> ParentClass;

	//~ Begin UFactory Interface
	/* New assets that don't override this function are automatically placed into the "Miscellaneous" category in the editor */
	virtual FText GetDisplayName() const override;
	virtual uint32 GetMenuCategories() const override;
	virtual bool ConfigureProperties() override;
	virtual bool ShouldShowInNewMenu() const override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

};
 