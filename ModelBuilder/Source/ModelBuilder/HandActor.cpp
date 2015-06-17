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
	InteractionSphere->SetSphereRadius(10);


	//Sphere to interact with objects
	ReleaseSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("ReleaseSphere"));
	ReleaseSphere->AttachTo(RootComponent);
	ReleaseSphere->SetSphereRadius(12);

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
	if (heldObjects.Num() > 0)
		return;

	//Get all overlapping acotrs and store them
	TArray<AActor*> CollectedActors;
	InteractionSphere->GetOverlappingActors(CollectedActors);

	TObjectIterator<AModelActor> Itr;
	
	//Cast Here to our type
	AModelActor* closestObj = NULL;
	float minDist = FLT_MAX;

	//New way of doing it, first get an interator for ModelActors,
	//Iterate through all modelActors finding the closest,
	//If its within range set it to glowing, Detach it from its parent and attach it to this
	//Add it to the list of held objects (Should only be 1)

	for (TObjectIterator<AModelActor> Itr; Itr; ++Itr)
	{
		FVector ObjectLoc = Itr->GetActorLocation();
		FVector ThisLoc = this->GetActorLocation();
		float distance = (ThisLoc - ObjectLoc).Size();
		Itr->ModelActorMesh->SetRenderCustomDepth(false);

		if (distance < minDist)
		{
			minDist = distance;
			closestObj = *Itr;
		}
	}

	if (minDist < 300)
	{
		currentHeldObject = closestObj;
		closestObj->DetachRootComponentFromParent();
		closestObj->AttachRootComponentToActor(this, NAME_None, EAttachLocation::KeepWorldPosition, true);
		closestObj->ModelActorMesh->SetRenderCustomDepth(true);
		heldObjects.Add(closestObj);
	}

	/***********************************
	Early return old code
	************************************/
	return;


	int x = 0;

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
			currentHeldObject = testActor;
			testActor->DetachRootComponentFromParent();
			testActor->AttachRootComponentToActor(this, NAME_None, EAttachLocation::KeepWorldPosition, true);
			testActor->ModelActorMesh->SetRenderCustomDepth(true);
			heldObjects.Add(testActor);
			break;
		}
	}

}

void AHandActor::releaseObjects()
{
	//Cast Here to our type
	AModelActor* closestObj = NULL;
	float minDist = FLT_MAX;

	for (TObjectIterator<AModelActor> Itr; Itr; ++Itr)
	{
		FVector ObjectLoc = Itr->GetActorLocation();
		FVector ThisLoc = this->GetActorLocation();
		float distance = (ThisLoc - ObjectLoc).Size();
		//Itr->ModelActorMesh->SetRenderCustomDepth(false);

		if (distance < minDist)
		{
			minDist = distance;
			closestObj = *Itr;
		}
	}

//	closestObj->ModelActorMesh->SetRenderCustomDepth(true);


	//Get all overlapping acotrs and store them
	TArray<AActor*> CollectedActors;

	for (int x = 0; x < heldObjects.Num(); x++)
	{
		heldObjects[x]->DetachRootComponentFromParent();
		heldObjects[x]->connectToParent();
		//heldObjects[x]->ModelActorMesh->SetRenderCustomDepth(false);
	}

	heldObjects.Empty();

	return;

	ReleaseSphere->GetOverlappingActors(CollectedActors);

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
			if (testActor->GetAttachParentActor() == this)
			{
				testActor->DetachRootComponentFromParent();
				testActor->connectToParent();
				testActor->ModelActorMesh->SetRenderCustomDepth(false);
			}
		}
	}

	return;


	//If already Holdinng an Object, move along
	if (!currentHeldObject)
		return;

	currentHeldObject->DetachRootComponentFromParent();
	currentHeldObject->connectToParent();
	currentHeldObject = NULL;
}

void AHandActor::disConnectAllObjects()
{

	TObjectIterator<AModelActor> Itr;
	for (TObjectIterator<AModelActor> Itr; Itr; ++Itr)
	{
		Itr->DetachRootComponentFromParent();
	}
}
