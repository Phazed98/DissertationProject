// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelBuilder.h"
#include "ModelActor.h"
#include "EngineUtils.h"


// Sets default values
AModelActor::AModelActor(const FObjectInitializer& ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Basic Collision Model
	BaseCollisionCompoenent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSphereComponenet"));
	RootComponent = BaseCollisionCompoenent;

	//Visible Mesh
	ModelActorMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("HandActorMesh"));
	ModelActorMesh->SetMobility(EComponentMobility::Movable);
	ModelActorMesh->AttachTo(RootComponent);


	//Sphere to interact with objects
	InteractionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("InteractionSphere"));
	InteractionSphere->AttachTo(RootComponent);
	InteractionSphere->SetSphereRadius(20.0f);


}

// Called when the game starts or when spawned
void AModelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AModelActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}


void AModelActor::connectToParent()
{
	//Get all overlapping acotrs and store them
	TArray<AActor*> CollectedActors;

	InteractionSphere->GetOverlappingActors(CollectedActors);

	//For Each Actor
	for (int x = 0; x < CollectedActors.Num(); ++x)
	{
		if (x == 0)
		{
			continue;
		}

		//Cast Here to our type
		AModelActor* const testActor = Cast<AModelActor>(CollectedActors[x]);
		
		if (testActor)
		{
			//Obv Cant Attach to Self
			if (testActor == this)
			{
				continue;
			}

			//Dont Attach To it if its Parent is it
			if (testActor->GetAttachParentActor() == this)
			{
				continue;
			}

			FVector WorldLoc1;
			FRotator WorldRot1;

			FVector WorldLoc2;
			FRotator WorldRot2;

			testActor->ModelActorMesh->GetSocketWorldLocationAndRotation("AttachSocket1", WorldLoc1, WorldRot1);
			testActor->ModelActorMesh->GetSocketWorldLocationAndRotation("AttachSocket2", WorldLoc2, WorldRot2);

			FVector locationVector1 = this->GetActorLocation() - WorldLoc1;
			FVector locationVector2 = this->GetActorLocation() - WorldLoc2;

			float distance1 = locationVector1.Size();
			float distance2 = locationVector2.Size();

			if (distance1 < distance2)
			{
				//this->SetActorLocation(WorldLoc1);
				this->AttachRootComponentToActor(testActor, "AttachSocket1", EAttachLocation::KeepWorldPosition, true);
			}
			else
			{
				//this->SetActorLocation(WorldLoc2);
				this->AttachRootComponentToActor(testActor, "AttachSocket2", EAttachLocation::KeepWorldPosition, true);
			}


			
		}
	}

}

