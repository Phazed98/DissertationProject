// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelBuilder.h"
#include "HandActor.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))


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
	hasSelectedObject = false;

	resetPosition = true;

	triggerHeld = false;
}

// Called when the game starts or when spawned
void AHandActor::BeginPlay()
{
	Super::BeginPlay();
	resetPosition = true;
	
	TheMaterial_Dyn2 = UMaterialInstanceDynamic::Create(TheMaterial2, this);
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
	if (resetPosition)
	{
		//Initial Values From the PSMove in Order to set position relative to
		HandStartPosition = HandPosition;
		HandStartRotation = HandRotation;
		resetPosition = false;
	}
	//Update Relative Position
	FVector RelativePosition = HandStartPosition - HandPosition;
	RelativePosition *= FVector(-2.0f, -2.0f, -2.0f);
	RelativePosition.X += 100;
	SetActorRelativeLocation(RelativePosition);

	//Update HandActor Relative Rotation
	SetActorRelativeRotation(HandRotation);
}

void AHandActor::selectObject()
{
	//If already Holdinng an Object, move along
	if (hasSelectedObject)
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
	

	if (minDist < 600)
	{
		currentSelectedObject = closestObj;
		currentSelectedObject->ModelActorMesh->SetRenderCustomDepth(true);
		currentSelectedObject->ModelActorMesh->SetMobility(EComponentMobility::Movable);
		hasSelectedObject = true;
	}
}

void AHandActor::deselectObject()
{
	if (hasSelectedObject)
		currentSelectedObject->ModelActorMesh->SetRenderCustomDepth(false);

	currentSelectedObject = NULL;
	hasSelectedObject = false;
}

void AHandActor::eyDropper()
{
	selectObject();

	if (hasSelectedObject)
	{
		TheMaterial_Dyn2 = UMaterialInstanceDynamic::Create(currentSelectedObject->ModelActorMesh->GetMaterial(0), this);
		HandActorMesh->SetMaterial(0, TheMaterial_Dyn2);
	}
}

void AHandActor::paintObject()
{
	selectObject();

	if (hasSelectedObject)
	{
		//TheMaterial_Dyn = UMaterialInstanceDynamic::Create(TheMaterial, this);
		//currentSelectedObject->ModelActorMesh->SetMaterial(0, TheMaterial_Dyn);


		//TheMaterial_Dyn2 = UMaterialInstanceDynamic::Create(TheMaterial2, this);
		HandActorMesh->SetMaterial(0, TheMaterial_Dyn2);
		currentSelectedObject->ModelActorMesh->SetMaterial(0, TheMaterial_Dyn2);
	}

	deselectObject();
}

void AHandActor::stretchObjects()
{
	selectObject();

	if (!triggerHeld)
	{
		triggerStartPosition = GetActorLocation();
		lastPosition = GetActorLocation();
		triggerHeld = true;
	}

	if (hasSelectedObject)
	{
		FVector endPosition = (lastPosition - GetActorLocation()) / 10;
		FVector FinalScale = currentSelectedObject->GetActorScale() + endPosition;
		
		FinalScale = FVector(FinalScale.X, FinalScale.X, FinalScale.X);


		//Set Minimum and Maximum Values
		FinalScale.X = MAX(FinalScale.X, 0.25);
		FinalScale.Y = MAX(FinalScale.Y, 0.25);
		FinalScale.Z = MAX(FinalScale.Z, 0.25);

		FinalScale.X = MIN(FinalScale.X, 3);
		FinalScale.Y = MIN(FinalScale.Y, 3);
		FinalScale.Z = MIN(FinalScale.Z, 3);

		currentSelectedObject->SetActorRelativeScale3D(FinalScale);
		lastPosition = GetActorLocation();

	}
}

void AHandActor::stretchObjectsRelease()
{
	if (!hasSelectedObject)
		return;

	if (triggerHeld)
		triggerHeld = false;


	deselectObject();
}


void AHandActor::pickupObject()
{
	//If we dont have an object selected get one.
	selectObject();

	//If successfully found an Object Attach It to the handActor
	if (hasSelectedObject)
	{
		currentSelectedObject->AttachRootComponentToActor(this, NAME_None, EAttachLocation::KeepWorldPosition, true);
		currentSelectedObject->ModelActorMesh->SetRenderCustomDepth(true);
	}
}


void AHandActor::rotateObjects()
{
	//If we dont have an object selected get one.
	selectObject();

	if (!triggerHeld)
	{
		triggerStartRotation = GetActorRotation();
		lastRotation = GetActorRotation();
		triggerHeld = true;
	}

	FRotator endRotation = (lastRotation - GetActorRotation());
	FRotator finalRotation = currentSelectedObject->GetActorRotation() + endRotation;


	currentSelectedObject->SetActorRotation(finalRotation);
	lastRotation = GetActorRotation();

	return;


	currentSelectedObject->GetActorRotation() - this->GetActorRotation();

	//If successfully found an Object Attach It to the handActor
	if (hasSelectedObject)
	{
		currentSelectedObject->SetActorRotation(this->GetActorRotation());
	}

	deselectObject();
}

void AHandActor::releaseObjects()
{
	if (!hasSelectedObject)
		return;

	currentSelectedObject->DetachRootComponentFromParent();
	currentSelectedObject->connectToParent();

	deselectObject();
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
		pickupObject();
		break;
	case STRETCH_TOOL:
		stretchObjects();
		break;
	case SCALE_TOOL:
		rotateObjects();
		break;
	case EYEDROPPER_TOOL:
		eyDropper();
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
		stretchObjectsRelease();
		break;
	case EYEDROPPER_TOOL:
		break;
	case PAINT_TOOL:
		paintObject();
		break;
	default:
		break;
	}
	
}

void AHandActor::xPressed()
{

	return;

	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
		break;
	case PAINT_TOOL:
		break;
	default:
		break;
	}
}

void AHandActor::circlePressed()
{

	resetPosition = true;

	return;

	switch (currentTool)
	{
	case GLUE_TOOL:
		break;
	case STRETCH_TOOL:
		break;
	case SCALE_TOOL:
		break;
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
	case EYEDROPPER_TOOL:
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
		currentTool = EYEDROPPER_TOOL;
		break;
	case EYEDROPPER_TOOL:
		currentTool = PAINT_TOOL;
		break;
	case PAINT_TOOL:
		currentTool = GLUE_TOOL;
		break;
	default:
		break;
	}
}