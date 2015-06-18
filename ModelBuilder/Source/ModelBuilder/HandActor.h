// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModelActor.h"
#include "GameFramework/Actor.h"
#include "HandActor.generated.h"


enum tools
{
	GLUE_TOOL,
	STRETCH_TOOL,
	SCALE_TOOL,
	PAINT_TOOL
};

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


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hands)
		FVector HandStartPosition;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hands)
		FRotator HandStartRotation;

	TArray<AModelActor*> heldObjects;


	AModelActor* currentHeldObject;

	//Should be called when Trigger is pulled
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void setInitialValues(FVector position, FRotator rotation);

	//Should be called when Trigger is pulled
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void checkObjects();

	//Should be called when Trigger is pulled
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void releaseObjects();

	//Should be called when Trigger is pulled
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void disconnectAllObjects();

	//Should be called when Trigger is pulled
	UFUNCTION(BlueprintCallable, Category = "Update")
		void UpdateHands(FVector HandPosition, FRotator HandRotation);

	void stretchObjects();
	void stretchObjectsRelease();

	tools currentTool;

	bool triggerHeld;
	FVector triggerStartPosition;

	/*
	 ____  _  _  ____  ____   __   __ _        ____  ____  ____  ____  ____  ____  ____ 
	(  _ \/ )( \(_  _)(_  _) /  \ (  ( \      (  _ \(  _ \(  __)/ ___)/ ___)(  __)/ ___)
	 ) _ () \/ (  )(    )(  (  O )/    /       ) __/ )   / ) _) \___ \\___ \ ) _) \___ \
	(____/\____/ (__)  (__)  \__/ \_)__)      (__)  (__\_)(____)(____/(____/(____)(____/
	
	*/

	//X Button Pressed and Released
	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void triggerPressed(uint8 pressedAmount);

	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void triggerReleased();

	//X Button Pressed and Released
	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void xPressed();

	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void xReleased();

	//Circle Button Pressed and Released
	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void circlePressed();

	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void circleReleased();

	//Triangle Button Pressed and Released
	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void trianglePressed();

	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void triangleReleased();

	//Square Button Pressed and Released
	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void squarePressed();

	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void squareReleased();

	//Start Button Pressed and Released
	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void startPressed();

	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void startReleased();

	//Select Button Pressed and Released
	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void selectPressed();

	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void selectReleased();

	//PSMove Button Pressed and Released
	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void movePressed();

	UFUNCTION(BlueprintCallable, Category = "ButtonPress")
		void moveReleased();

};
