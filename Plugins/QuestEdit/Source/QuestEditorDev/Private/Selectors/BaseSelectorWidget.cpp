//Copyright 2020 Marchetti S. Alfredo I. All Rights Reserved.


#include "Selectors/BaseSelectorWidget.h"

UDataTable* UBaseSelectorWidget::FindDTFromName(const FString& DTObjectNameIn)
{
	UDataTable* DT = nullptr;

	//If all fails will show first DT valid
	if (Datatables.IsValidIndex(0)) DT = Datatables[0];
	else return DT;

	for (int i = 0; i < Datatables.Num(); i++)
	{
		if (Datatables[i]->GetName() == DTObjectNameIn)
		{
			DT = Datatables[i];
			break;
		}
	}

	return DT;
}

UDataTable* UBaseSelectorWidget::FindDTFromPath(const TSoftObjectPtr <UDataTable>& DTObjectPathIn)
{
	UDataTable* DT = nullptr;

	//If all fails will show first DT valid
	if (Datatables.IsValidIndex(0)) DT = Datatables[0];
	else return DT;

	for (int i = 0; i < Datatables.Num(); i++)
	{
		if (Datatables[i] == DTObjectPathIn)
		{
			DT = Datatables[i];
			break;
		}			
	}

	return DT;
}
