// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyRocket.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetCollisionProfileName(TEXT("Pawn"));
	Box->SetNotifyRigidBodyCollision(true);
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	Left->SetRelativeLocation(FVector(38.0f, -20.0f, 0.0f));
	Right->SetRelativeLocation(FVector(38.0f, 20.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 8.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->SetUpdatedComponent(Box);
	Movement->MaxSpeed = NormalSpeed;

	static ConstructorHelpers::FClassFinder<AMyRocket> ClassPath(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_Rocket.BP_Rocket_C'"));
	if (ClassPath.Succeeded())
	{
		RocketTemplate = ClassPath.Class;
	}

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Left->AddLocalRotation(FRotator(0, 0, 1440.0f * DeltaTime));
	Right->AddLocalRotation(FRotator(0, 0, 1440.0f * DeltaTime));

	// Box로 충돌을 계산하면서 기체는 계속 전진한다.
	AddMovementInput(GetActorForwardVector());
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyPawn::Fire);
	PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Pressed, this, &AMyPawn::BoostOn);
	PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Released, this, &AMyPawn::BoostOff);

	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyPawn::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
}

void AMyPawn::CallBlueprint(int Money, FString Name)
{
	UE_LOG(LogTemp, Warning, TEXT("Execute CPP %d %s"), Money, *Name);
}

void AMyPawn::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire"));

	if (RocketTemplate)
	{
		const FName SocketName = TEXT("RocketSpawnPoint");
		if (!Body->DoesSocketExist(SocketName))
		{
			UE_LOG(LogTemp, Warning, TEXT("RocketSpawnPoint 소켓이 없습니다."));
			return;
		}

		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;
		SpawnParameters.Instigator = this;
		SpawnParameters.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AMyRocket>(
			RocketTemplate,
			Body->GetSocketTransform(SocketName),
			SpawnParameters);
	}
}

void AMyPawn::Pitch(float Value)
{
	AddActorLocalRotation(
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * FRotator(Value, 0, 0) * 60);
}

void AMyPawn::Yaw(float Value)
{
	// A/D로 기체의 좌우 방향을 바꾼다.
	AddActorLocalRotation(
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * FRotator(0, Value, 0) * 60);
}

void AMyPawn::Roll(float Value)
{
	AddActorLocalRotation(
		UGameplayStatics::GetWorldDeltaSeconds(GetWorld()) * FRotator(0, 0, Value) * 60);
}

void AMyPawn::BoostOn()
{
	Movement->MaxSpeed = BoostSpeed;
}

void AMyPawn::BoostOff()
{
	Movement->MaxSpeed = NormalSpeed;
}
