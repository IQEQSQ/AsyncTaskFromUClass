#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_CallFunction.h"
#include "K2Node_AsyncTaskFromUObject.generated.h"


class FBlueprintActionDatabaseRegistrar;
class FKismetCompilerContext;
class UEdGraph;
class UEdGraphPin;
class UEdGraphSchema_K2;
class UK2Node_CustomEvent;
class UK2Node_TemporaryVariable;

/** struct to remap pins for Async Tasks.
 * a single K2 node is shared by many proxy classes.
 * This allows redirecting pins by name per proxy class.
 * Add entries similar to this one in Engine.ini:
 * +K2AsyncTaskPinRedirects=(ProxyClassName="AbilityTask_PlayMontageAndWait", OldPinName="OnComplete", NewPinName="OnBlendOut")
 */

struct FAsyncTaskFromUObjectPinRedirectMapInfo
{
	TMap<FName, TArray<UClass*> > OldPinToProxyClassMap;
};

/** !!! The proxy object should have RF_StrongRefOnFrame flag. !!! */

UCLASS()
class K2NODEASYNCTASK_API UK2Node_AsyncTaskFromUObject : public UK2Node
{
	GENERATED_UCLASS_BODY()

	//~ Begin UEdGraphNode Interface.
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	// virtual void EarlyValidation(class FCompilerResultsLog& MessageLog) const override;
	//~ End UEdGraphNode Interface.
	
	
	// UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	
	virtual void GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const override;
	// End of UEdGraphNode interface

	// UK2Node interface
	// virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	// virtual bool HasExternalDependencies(TArray<class UStruct*>* OptionalOutput) const override;
	virtual FName GetCornerIcon() const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	/** Whether or not two pins match for purposes of reconnection after reconstruction.  This allows pins that may have had their names changed via reconstruction to be matched to their old values on a node-by-node basis, if needed*/
	virtual ERedirectType DoPinsMatchForReconstruction(const UEdGraphPin* NewPin, int32 NewPinIndex, const UEdGraphPin* OldPin, int32 OldPinIndex) const override;
	// End of UK2Node interface

protected:

	/** Determines what the possible redirect pin names are **/
	// virtual void GetRedirectPinNames(const UEdGraphPin& Pin, TArray<FString>& RedirectPinNames) const;

	// The type of proxy object that will be created
	UPROPERTY()
	UClass* ProxyClass;

	struct K2NODEASYNCTASK_API FBaseAsyncTaskHelper
	{
		struct FOutputPinAndLocalVariable
		{
			UEdGraphPin* OutputPin;
			UK2Node_TemporaryVariable* TempVar;

			FOutputPinAndLocalVariable(UEdGraphPin* Pin, UK2Node_TemporaryVariable* Var) : OutputPin(Pin), TempVar(Var) {}
		};

		static bool ValidDataPin(const UEdGraphPin* Pin, EEdGraphPinDirection Direction);
		static bool CreateDelegateForNewFunction(UEdGraphPin* DelegateInputPin, FName FunctionName, UK2Node* CurrentNode, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext);
		static bool CopyEventSignature(UK2Node_CustomEvent* CENode, UFunction* Function, const UEdGraphSchema_K2* Schema);
		static bool HandleDelegateImplementation(
			FMulticastDelegateProperty* CurrentProperty, const TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs,
			UEdGraphPin* ProxyObjectPin, UEdGraphPin*& InOutLastThenPin,
			UK2Node* CurrentNode, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext);

		static const FName GetAsyncTaskProxyName();
	};

	// Pin Redirector support
	static TMap<FName, FAsyncTaskFromUObjectPinRedirectMapInfo> AsyncTaskPinRedirectMap;
	static bool bAsyncTaskPinRedirectMapInitialized;

private:
	/** Invalidates current pin tool tips, so that they will be refreshed before being displayed: */
	void InvalidatePinTooltips() { bPinTooltipsValid = false; }

	void ResetOutputDelegatePin();
	bool IsSpawnVarPin(UEdGraphPin* Pin) const;
	virtual UEdGraphPin* GetObjectPin(const TArray<UEdGraphPin*>* InPinsToSearch /*= NULL*/) const;
	UClass* GetObjectClassToExpand(const TArray<UEdGraphPin*>* InPinsToSearch /*=NULL*/) const;
	
	// UEdGraphPin* GenerateAssignmentNodes(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph, UK2Node_CallFunction* CallBeginSpawnNode, UEdGraphNode* SpawnNode, UEdGraphPin* CallBeginResult, const UClass* ForClass ) const;
	/**
	* Creates hover text for the specified pin.
	*
	* @param   Pin				The pin you want hover text for (should belong to this node)
	*/
	void GeneratePinTooltip(UEdGraphPin& Pin) const;

	void OnObjectPinChanged();
	

	/** Flag used to track validity of pin tooltips, when tooltips are invalid they will be refreshed before being displayed */
	mutable bool bPinTooltipsValid;
};
