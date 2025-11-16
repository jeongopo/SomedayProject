

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPActor.generated.h"

UCLASS()
class SOMEDAYPROJECT_API ASPActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void ResetToDefaultData();
	
};
