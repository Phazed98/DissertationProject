// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelBuilder.h"
#include "HandActor.h"



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
	//If already Holdinng an Object, move along
	if (currentHeldObject)
		return;


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
		AModelActor* const testActor = Cast<AModelActor>(CollectedActors[x]);
		if (testActor)
		{
			currentHeldObject = testActor;
			testActor->AttachRootComponentToActor(this, NAME_None, EAttachLocation::KeepWorldPosition, true);
		}
	}

}

void AHandActor::releaseObjects()
{
	//If already Holdinng an Object, move along
	if (!currentHeldObject)
		return;

	currentHeldObject->DetachRootComponentFromParent();
	currentHeldObject = NULL;
}

