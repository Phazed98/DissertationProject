// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelBuilder.h"
#include "HandActor.h"


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

	//Set Which tool we are currently holding
	currentTool = GLUE_TOOL;

	triggerHeld = false;
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
}


void AHandActor::setInitialValues(FVector HandPosition, FRotator HandRotation)
{
	//Initial Values From the PSMove in Order to set position relative to
	HandStartPosition = HandPosition;
	HandStartRotation = HandRotation;
}

void AHandActor::UpdateHands(FVector HandPosition, FRotator HandRotation)
{
	//Update Relative Position
	FVector RelativePosition = HandStartPosition - HandPosition;
	RelativePosition *= FVector(-2.0f, -2.0f, -2.0f);
	RelativePosition.X += 100;
	SetActorRelativeLocation(RelativePosition);

	//Update HandActor Relative Rotation
	SetActorRelativeRotation(HandRotation);
}

void AHandActor::stretchObjects()
{
	if (!triggerHeld)
	{
		triggerStartPosition = GetActorLocation();
		triggerHeld = true;
	}


	//If already Holdinng an Object, move along
	if (heldObjects.Num() > 0)
		return;

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

	if (minDist < 900)
	{
		currentHeldObject = closestObj;
		//closestObj->DetachRootComponentFromParent();
		//closestObj->AttachRootComponentToActor(this, NAME_None, EAttachLocation::KeepWorldPosition, true);
		closestObj->ModelActorMesh->SetRenderCustomDepth(true);
		heldObjects.Add(closestObj);
	}
}

void AHandActor::stretchObjectsRelease()
{
	if (triggerHeld)
	{
		triggerHeld = false;

		FVector endPosition = triggerStartPosition - GetActorLocation();
		endPosition /= 10;
		FVector FinalScale = heldObjects[0]->GetActorScale() + endPosition;

		if (FinalScale.X < 0)
		{
			FinalScale.X = 0.25;
		}
		if (FinalScale.Y < 0)
		{
			FinalScale.Y = 0.25;
		}
		if (FinalScale.Z < 0)
		{
			FinalScale.Z = 0.25;
		}

		heldObjects[0]->SetActorRelativeScale3D(FinalScale);
	}
}


void AHandActor::checkObjects()
{
	//If already Holdinng an Object, move along
	if (heldObjects.Num() > 0)
		return;

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

	if (minDist < 900)
	{
		currentHeldObject = closestObj;
		closestObj->DetachRootComponentFromParent();
		closestObj->AttachRootComponentToActor(this, NAME_None, EAttachLocation::KeepWorldPosition, true);
		closestObj->ModelActorMesh->SetRenderCustomDepth(true);
		heldObjects.Add(closestObj);
	}

	/***********************************
			Early return old code

	Causes Issue with Parent/Child 
	relationship as a child will return 
	the parent object rather than itself

	************************************/
	return;


	//Get all overlapping acotrs and store them
	TArray<AActor*> CollectedActors;
	InteractionSphere->GetOverlappingActors(CollectedActors);

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
	for (int x = 0; x < heldObjects.Num(); x++)
	{
		heldObjects[x]->DetachRootComponentFromParent();
		heldObjects[x]->connectToParent();
		heldObjects[x]->ModelActorMesh->SetRenderCustomDepth(false);
	}

	heldObjects.Empty();

	return;
}

void AHandActor::disconnectAllObjects()
{
	for (TObjectIterator<AModelActor> Itr; Itr; ++Itr)
	{
		Itr->DetachRootComponentFromParent();
	}
}


void AHandActor::triggerPressed(uint8 pressedAmount)
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		checkObjects();
		break;
	case STRETCH_TOOL:
		stretchObjects();
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}

}

void AHandActor::triggerReleased()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		releaseObjects();
		break;
	case STRETCH_TOOL:
		stretchObjectsRelease();
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
	
}

void AHandActor::xPressed()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::xReleased()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::circlePressed()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::circleReleased()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::trianglePressed()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::triangleReleased()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		disconnectAllObjects();
		break;
	case STRETCH_TOOL:
		disconnectAllObjects();
		break;
	case SCALE_TOOL:
		disconnectAllObjects();
		break;
	case PAINT_TOOL:
		disconnectAllObjects();
		break;
	default:
		break;
	}
}

void AHandActor::squarePressed()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::squareReleased()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::startPressed()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::startReleased()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::selectPressed()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::selectReleased()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::movePressed()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::moveReleased()
{
	switch (currentTool)
	{
	case GLUE_TOOL:
		currentTool = STRETCH_TOOL;
		break;
	case STRETCH_TOOL:
		currentTool = SCALE_TOOL;
		break;
	case SCALE_TOOL:
		currentTool = PAINT_TOOL;
		break;
	case PAINT_TOOL:
		currentTool = GLUE_TOOL;
		break;
	default:
		break;
	}
}