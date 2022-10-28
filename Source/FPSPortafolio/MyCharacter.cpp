// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "RunTime/Engine/Public/TimerManager.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	//Size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Jump Value
	JumpHeight = 450.f;

	//Sprint values
	WalkingSpeed = 600.f;
	RunningSpeed = 1200.f;

	//Character Movement
	GetCharacterMovement()->AirControl = 0.3f;

	//Create a Camera Boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	//Create a Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Dash Values
	dashDistance = 19200.f;
	dashCooldown = 0.25f;
	dashStop = 0.1f;
	canDash = true;

	//Crouching values
	IsCrouching = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyCharacter::MoveRight(float AxisVal){
	AddMovementInput(GetActorRightVector() * AxisVal);
}

void AMyCharacter::MoveForward(float AxisVal){
	AddMovementInput(GetActorForwardVector() * AxisVal);
}

void AMyCharacter::Landed(const FHitResult& Hit)
{
	DoubleJumpCounter = 0;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	//Player movement
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
	//Player camera movement
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::AddControllerPitchInput);
	//Player Jump
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AMyCharacter::DoubleJump);
	//Player Sprint
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMyCharacter::sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMyCharacter::walk);
	//Player Dash
	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AMyCharacter::Dash);
	//Player Crouch
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &AMyCharacter::StartCrouch);
	PlayerInputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &AMyCharacter::StopCrouch);
}

//DoubleJump Function
void AMyCharacter::DoubleJump(){
	if (DoubleJumpCounter <= 1) {
		AMyCharacter::LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
		DoubleJumpCounter++;
	}
}

void AMyCharacter::sprint()
{
	if (IsCrouching)
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed/2;
	}
	GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
}

void AMyCharacter::walk()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
}

void AMyCharacter::StartCrouch()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(48.0f);
	GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	AMyCharacter::Crouch();

	IsCrouching = true;
}
void AMyCharacter::StopCrouch()
{
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	AMyCharacter::UnCrouch();

	IsCrouching = false;
}

//Dash Functions
void AMyCharacter::Dash()
{
	if (canDash) {
		GetCharacterMovement()->BrakingFrictionFactor = 0.f;
		LaunchCharacter(FVector(FollowCamera->GetForwardVector().X, FollowCamera->GetForwardVector().Y,0) *dashDistance, true, true);
		canDash = false;
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMyCharacter::StopDash, dashStop, false);
	}
}

void AMyCharacter::StopDash()
{
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->BrakingFrictionFactor = 2.f;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMyCharacter::ResetDash, dashCooldown, false);

}

void AMyCharacter::ResetDash()
{
	canDash = true;
}

