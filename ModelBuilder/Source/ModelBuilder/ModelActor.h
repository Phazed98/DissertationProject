// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ModelActor.generated.h"

UCLASS()
class MODELBUILDER_API AModelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AModelActor(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Base Collison Volumn
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hands)
		USphereComponent* BaseCollisionCompoenent;
	
	//Base Static Mesh
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hands)
		UStaticMeshComponent* ModelActorMesh;

	//Interaction Sphere to determine objects that are in range to be interacted with the PSMove controller
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hands)
		USphereComponent* InteractionSphere;

	//Should be called when Trigger is pulled
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void connectToParent();

};
