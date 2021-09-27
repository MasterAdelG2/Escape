// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMass() const;
	void FindAudioComponent();
private:
	FRotator DefaultRotation;
	float ClosedYaw;
	UPROPERTY(EditAnywhere)
	float Angle;
	float DoorLastOpened;
	float DoorCloseDelay= 0.2f;
	UPROPERTY(EditAnywhere)
		float MassToOpenDoor = 70.f;
	UPROPERTY(EditAnywhere)
		float DoorOpenSpeed = 0.8f;
	UPROPERTY(EditAnywhere)
		float DoorCloseSpeed = 2.0f;
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate= nullptr;
	UPROPERTY(EditAnywhere)
		AActor* ActorThatOpen = nullptr;
	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
