// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRocket.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AMyRocket::AMyRocket()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyRocket::BeginPlay()
{
	Super::BeginPlay();

	// BP에서 지정하지 않았을 때만 기본 폭발 이펙트를 불러온다.
	if (!ExplosionTemplate)
	{
		ExplosionTemplate = LoadObject<UNiagaraSystem>(
			nullptr, TEXT("/Game/Effects/FX_Explosions/NS_Explosion.NS_Explosion"));
	}

	OnActorBeginOverlap.AddDynamic(this, &AMyRocket::ProcessActorBeginOverlap);
	OnActorHit.AddDynamic(this, &AMyRocket::ProcessActorHit);

	StartLocation = GetActorLocation();

	// 작은 로켓 메시가 카메라에서 잘 보이도록 시각 크기만 조절한다.
	TArray<UStaticMeshComponent*> MeshComponents;
	GetComponents(MeshComponents);
	for (UStaticMeshComponent* MeshComponent : MeshComponents)
	{
		MeshComponent->SetRelativeScale3D(
			MeshComponent->GetRelativeScale3D() * RocketVisualScale);
	}

	// BP_Rocket에 있는 ProjectileMovement를 찾아 소켓의 앞 방향으로 발사한다.
	if (UProjectileMovementComponent* ProjectileMovement =
		FindComponentByClass<UProjectileMovementComponent>())
	{
		ProjectileMovement->InitialSpeed = RocketSpeed;
		ProjectileMovement->MaxSpeed = RocketSpeed;
		ProjectileMovement->ProjectileGravityScale = 0.0f;
		ProjectileMovement->bRotationFollowsVelocity = true;
		ProjectileMovement->Velocity = GetActorForwardVector() * RocketSpeed;
	}

	// 발사한 기체와 충돌하지 않고, 카메라의 Spring Arm도 밀지 않게 한다.
	TArray<UPrimitiveComponent*> PrimitiveComponents;
	GetComponents(PrimitiveComponents);
	for (UPrimitiveComponent* Component : PrimitiveComponents)
	{
		Component->IgnoreActorWhenMoving(GetOwner(), true);
		Component->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	}
}

// Called every frame
void AMyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FVector::DistSquared(StartLocation, GetActorLocation()) >= FMath::Square(MaxDistance))
	{
		Destroy();
	}
}

void AMyRocket::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor == nullptr || OtherActor == GetOwner())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap %s"), *OtherActor->GetName());
	Explode(OtherActor, GetActorLocation());
}

void AMyRocket::ProcessActorHit(AActor* SelfActor, AActor* OtherActor,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr || OtherActor == GetOwner())
	{
		return;
	}

	// Hit 이벤트에서는 실제 충돌 지점을 사용할 수 있다.
	Explode(OtherActor, Hit.ImpactPoint);
}

void AMyRocket::Explode(AActor* OtherActor, const FVector& ExplosionLocation)
{
	if (bExploded)
	{
		return;
	}
	bExploded = true;

	if (ExplosionTemplate)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), ExplosionTemplate, ExplosionLocation);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(), ExplosionSound, ExplosionLocation);
	}

	UGameplayStatics::ApplyDamage(OtherActor,
		10,
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		this,
		nullptr
	);
	
	//Blueprint 함수 호출 하고 싶어요
	CallCPPExecuteBP();

	//Blueprint 함수 호출 하고 싶어요(니가 안하면 내가 기본으로 한다.
	CallCPPExecuteBPDefault();

	Destroy();
}

void AMyRocket::CallCPPExecuteBPDefault_Implementation(void)
{
	UE_LOG(LogTemp, Warning, TEXT("Execute CPP"));
}
