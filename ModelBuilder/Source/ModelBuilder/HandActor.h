// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModelActor.h"
#include "GameFramework/Actor.h"
#include "HandActor.generated.h"

UCLASS()
class MODELBUILDER_API AHandActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandActor(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Base Collison Volumn
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hands)
	USphereComponent* BaseCollisionCompoenent;

	//Base Static Mesh
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Hands)
	UStaticMeshComponent* HandActorMesh;

	//Interaction Sphere to determine objects that are in range to be interacted with the PSMove controller
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hands)
	USphereComponent* InteractionSphere;

	//Release Sphere is slightly larger in order to release objects more garunteed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hands)
		USphereComponent* ReleaseSphere;

	//Should be called when Trigger is pulled
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void checkObjects();

	//Should be called when Trigger is pulled
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void releaseObjects();

	//Should be called when Trigger is pulled
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void disConnectAllObjects();

	TArray<AModelActor*> heldObjects;


	AModelActor* currentHeldObject;

};
