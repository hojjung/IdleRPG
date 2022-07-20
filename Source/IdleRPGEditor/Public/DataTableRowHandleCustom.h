// My First Hack n Slash

#pragma once




#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "Layout/Margin.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "IPropertyTypeCustomization.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Views/STableViewBase.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/SListView.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "IPropertyUtilities.h"

struct FAssetData;

class FDataTableRowHandleCustom: public IPropertyTypeCustomization
{

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance() 
	{
		return MakeShareable( new FDataTableRowHandleCustom );
	}

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<class IPropertyHandle> InStructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:

	bool ShouldFilterAsset(const FAssetData& AssetData);

	/** Init the contents the combobox sources its data off */
	TSharedPtr<FName> InitWidgetContent();

	/** Returns the ListView for the ComboButton */
	TSharedRef<SWidget> GetListContent();

	/** Delegate to refresh the drop down when the datatable changes */
	void OnDataTableChanged();

	/** Return the representation of the the row names to display */
	TSharedRef<ITableRow> HandleRowNameComboBoxGenarateWidget(TSharedPtr<FName> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	/** Display the current selection */
	FText GetRowNameComboBoxContentText() const;

	/** Update the root data on a change of selection */
	void OnSelectionChanged(TSharedPtr<FName> SelectedItem, ESelectInfo::Type SelectInfo);

	/** Called by Slate when the filter box changes text. */
	void OnFilterTextChanged(const FText& InFilterText);

	void HandleMenuOpen();

	void OnSearchForReferences();

	/** The comboButton objects */
	TSharedPtr<SComboButton> RowNameComboButton;
	TSharedPtr<class SSearchBox> SearchBox;
	TSharedPtr<SListView<TSharedPtr<FName>>> RowNameComboListView;
	TSharedPtr<FName> CurrentSelectedItem;
	/** Handle to the struct properties being customized */
	TSharedPtr<IPropertyHandle> StructPropertyHandle;
	TSharedPtr<IPropertyHandle> DataTablePropertyHandle;
	TSharedPtr<IPropertyHandle> RowNamePropertyHandle;
	/** A cached copy of strings to populate the combo box */
	TArray<TSharedPtr<FName>> RowNames;
	/** The MetaData derived filter for the row type */
	FName RowTypeFilter;
};
