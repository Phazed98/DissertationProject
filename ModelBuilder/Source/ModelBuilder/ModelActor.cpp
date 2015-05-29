// Fill out your copyright notice in the Description page of Project Settings.

#include "ModelBuilder.h"
#include "ModelActor.h"


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
	ModelActorMesh->AttachTo(RootComponent);

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

