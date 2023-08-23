// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ILogable.generated.h"


UINTERFACE(MinimalAPI)
class UILogable : public UInterface
{
	GENERATED_BODY()
};


class ONLINESHOOTER_API IILogable
{
	GENERATED_BODY()

public:
	virtual void LogData()=0;
	virtual void LogState()=0;
};
