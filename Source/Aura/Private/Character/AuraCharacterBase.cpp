#include "Character/AuraCharacterBase.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon"); // Weapon ������Ʈ ����
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket")); // �������� ����
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); // �浹 ����
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();

}