// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelBuilder.h"
#include "HandActor.h"
#include "ModelActor.h"


// Sets default values
AHandActor::AHandActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Basic Collision Model
	BaseCollisionCompoenent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("BaseSphereComponenet"));
	RootComponent = BaseCollisionCompoenent;

	//Visible Mesh
	HandActorMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("HandActorMesh"));
	HandActorMesh->AttachTo(RootComponent);

	//Sphere to interact with objects
	InteractionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("InteractionSphere"));
	InteractionSphere->AttachTo(RootComponent);
	InteractionSphere->SetSphereRadius(5.0f);

	//InputComponent->BindAction("Attach", IE_Pressed, this, &AHandActor::checkObjects);
}

// Called when the game starts or when spawned
void AHandActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHandActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//checkObjects();

}

void AHandActor::checkObjects()
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
		//TODO Create type with Mesh and Cast to it
		AActor* const testActor = Cast<AModelActor>(CollectedActors[x]);
		if (testActor)
		{

			testActor->AttachRootComponentToActor(this, NAME_None, EAttachLocation::KeepRelativeOffset);
			//testActor->AttachRootComponentToActor(this);
			//testActor->AttachRootComponentTo(RootComponent);
			//testActor->DetachRootComponentFromParent();
		}
	}

}

