#pragma once

#include "RPGSystem.h"
#include "Utils.generated.h"

/*
For more info: https://wiki.unrealengine.com/index.php?title=Logs,_Printing_Class_Name,_Function_Name,_Line_Number_of_your_Calling_Code!
*/

//Current Class Name + Function Name where this is called!
#define PRINT_CUR_CLASS_FUNC (FString(__FUNCTION__))

//Current Class where this is called!
#define PRINT_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Function Name where this is called!
#define PRINT_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))

//Current Line Number in the code where this is called!
#define PRINT_CUR_LINE  (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called!
#define PRINT_CUR_CLASS_LINE (PRINT_CUR_CLASS + "(" + PRINT_CUR_LINE + ")")

//Current Class, Function and Line Number where this is called!
#define PRINT_CUR_CLASS_FUNC_LINE (PRINT_CUR_CLASS_FUNC + "(" + PRINT_CUR_LINE + ")")

//Current Function Signature where this is called!
#define PRINT_CUR_FUNCSIG (FString(__FUNCSIG__))

#define SCREENMSG(Param1)					(GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(Param1)) )

#define SCREENMSG2(Param1,Param2)			(GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(PRINT_CUR_CLASS_FUNC_LINE + ": " + Param1 + " " + Param2)) )

#define SCREENMSG3(Param1, Param2)			(GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *("Object is not set on " + Param1 + " at " + FString::FromInt(Param2))) )

#define SCREENMSG4(Param1, Param2, Param3)	(GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(FString::FromInt(Param1) + ". object is not set on " + Param2 + " at " + FString::FromInt(Param3))) )

#define SCREENMSGF(Param1,Param2)			(GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(PRINT_CUR_CLASS_FUNC_LINE + ": " + Param1 + " " + FString::SanitizeFloat(Param2))) )

//UE LOG!
#define V_LOG(LogBuildingSystem, Param1) 					UE_LOG(LogBuildingSystem,Error,TEXT("%s: %s"), *PRINT_CUR_CLASS_LINE, *FString(Param1))

#define V_LOG2(LogBuildingSystem, Param1,Param2) 			UE_LOG(LogBuildingSystem,Error,TEXT("%s: %s %s"), *PRINT_CUR_CLASS_LINE, *FString(Param1),*FString(Param2))

#define V_LOG3(LogBuildingSystem, Param1,Param2) 			UE_LOG(LogBuildingSystem,Error,TEXT("Object is not set on %s at %d"), *FString(Param1), Param2)

#define V_LOGF(LogBuildingSystem, Param1,Param2) 			UE_LOG(LogBuildingSystem,Error,TEXT("%s: %s %f"), *PRINT_CUR_CLASS_LINE, *FString(Param1),Param2)

#define V_LOGM(LogBuildingSystem, FormatString , ...)		UE_LOG(LogBuildingSystem,Error,TEXT("%s: %s"), *PRINT_CUR_CLASS_LINE, *FString::Printf(TEXT(FormatString), ##__VA_ARGS__ ) )


#define V_LOG(LogBuildingSystem, Param1)					UE_LOG(LogBuildingSystem,Error,TEXT("%s: %s"), *PRINT_CUR_CLASS_LINE, *FString(Param1))

UCLASS()
class UUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	/**
	* Test validity of object
	*
	* @param	Num				Number of function arguments. Must be first argument
	* @param	Uobject*		The objects to test
	* @param	__FUNCTION__	Function Name. Must be last - 1 argument
	* @param	__LINE__		Line of function called. Must be last argument
	* @return	Return true if the object is usable: non-null and not pending kill
	*/
	static bool Test(int32 x, ...)
	{
		va_list args;
		va_start(args, x);

		UPROPERTY()
		TArray<UObject*> Objects;
		UPROPERTY()
		TArray<FString> ObjectsName;

		FString FuncName;
		int32 Line = 0;

		Objects.Empty();
		for (int i = 0; i < x; ++i)
		{
			if (i < x - 2)
				Objects.Add(va_arg(args, UObject*));
			else if (i == x - 2)
				FuncName = FString( va_arg(args, char*) );
			else if (i == x - 1)
				Line = va_arg(args, int32);
		}
		for (int i = 0; i < Objects.Num(); ++i)
		{
			if(Objects[i] == nullptr || Objects[i]->IsPendingKill())
			{
				SCREENMSG4(i, FuncName, Line);
				return false;
			}
		}

		va_end(args);
		return true;
	}
};