// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "MyCharacter.generated.h"


UCLASS()
class FPSPORTAFOLIO_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Default)
		UCameraComponent* FollowCamera;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Movement
	void MoveRight(float AxisVal);
	void MoveForward(float AxisVal);

	virtual void Landed(const FHitResult& Hit) override;

public:	
	//Default values for the character's properties
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//Double Jump
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void DoubleJump();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int DoubleJumpCounter;
		float JumpHeight;

	//Sprint Components
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void sprint();
		void walk();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float WalkingSpeed;
		float RunningSpeed;

	//Dash Components
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void Dash();
		void StopDash();
		void ResetDash();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float dashDistance;
		float dashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool canDash;
		float dashStop;
		FTimerHandle UnusedHandle;

	//Crouch Components
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
		void StartCrouch(); //Decreases player speed while crouching
		void StopCrouch(); //Increases player speed while standing

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsCrouching;
};
