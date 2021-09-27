// Fill out your copyright notice in the Description page of Project Settings.
#include "CollisionQueryParams.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	PrimaryComponentTick.bCanEverTick = true;
}
// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}
// Called When Left Mouse Button Pressed
void UGrabber::Grab()
{
	FHitResult Hit= GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab= Hit.GetComponent();
	if (Hit.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation
		(
			ComponentToGrab,
			NAME_Name,
			GetPlayersReach()
		);
	}
}
// Called When Left Mouse Button Released
void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}
// Called To check for Physics Handle Component
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Warning,TEXT("No PhysicsHadnle Set on Actor"));
	}
}
// Called To Setup Input Component
void UGrabber::SetupInputComponent()
{
InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab",IE_Pressed,this,&UGrabber::Grab);
		InputComponent->BindAction("Grab",IE_Released,this,&UGrabber::Release);
	}
}
// Called To Get Hit on First Body
FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PVPLocation;
	FRotator PVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PVPLocation,OUT PVPRotation);
	FVector LineTraceEnd = PVPLocation + PVPRotation.Vector() * Reach;
	
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PVPLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}
// Gets the End Point of lineTraced
FVector UGrabber::GetPlayersReach() const
{
	FVector PVPLocation;
	FRotator PVPRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PVPLocation,
		OUT PVPRotation);
	return PVPLocation + PVPRotation.Vector() * Reach;
}