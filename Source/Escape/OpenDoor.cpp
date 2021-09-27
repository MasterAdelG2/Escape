// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	ClosedYaw = GetOwner()->GetActorRotation().Yaw;
	Angle = 0.f;
	DefaultRotation = GetOwner()->GetActorRotation();
	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
	FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (TotalMass() >= MassToOpenDoor)
	{
		if (Angle < 1.f)
			OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
	{
		if (Angle > 0.f)
			CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	Angle = Angle < 1.f ? Angle + DoorOpenSpeed * DeltaTime : Angle;
	DefaultRotation.Yaw = FMath::Lerp(ClosedYaw, ClosedYaw - 90.f, Angle);
	GetOwner()->SetActorRotation(DefaultRotation);
	if (!AudioComponent->IsPlaying())
	{
		AudioComponent->Play();
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	Angle = Angle > 0.f ? Angle - DoorCloseSpeed * DeltaTime : Angle;
	DefaultRotation.Yaw = FMath::Lerp(ClosedYaw , ClosedYaw - 90.f, Angle);
	GetOwner()->SetActorRotation(DefaultRotation);
	if (!AudioComponent->IsPlaying())
	{
		AudioComponent->Play();
	}
}

float UOpenDoor::TotalMass() const 
{
	TArray<AActor*> AllActors;
	float Sum=0.f;
	PressurePlate->GetOverlappingActors(AllActors);
	for(AActor* actor : AllActors)
	{
		Sum += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning,TEXT("%s"), *actor->GetName());
	}
	return Sum;
}
void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Warning,TEXT("No Audio Component"));
	}
}