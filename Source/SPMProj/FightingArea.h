// Include necessary headers
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FightingArea.generated.h"

// Forward declare necessary classes
class AEnemy;
class UBoxComponent;

UCLASS()
class AFightingArea : public AActor
{
	GENERATED_BODY()

public:
	AFightingArea();
	
protected:
	//define in protected to allow childs of this class to access these values	
	
	// Array of enemies in the fighting area
	TArray<AEnemy*> Enemies;

	// Box component that defines the bounds of the fighting area
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Bounds;
	
	// Door that opens when all enemies are dead
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AActor* Door;
	
	// Override BeginPlay() to initialize the fighting area
	virtual void BeginPlay() override;

	// Check if all enemies are dead and open the door if so
	UFUNCTION()
	void CheckEnemiesDead();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void EnemiesLeft(int EnemiesAlive);

private:
	//timer 
	FTimerHandle SetTimer;
	//callback function
	void SetUpFightingArea();
};

