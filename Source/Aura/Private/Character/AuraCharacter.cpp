// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacter::AAuraCharacter()
{
	// ĳ������ �̵��������� ĳ���͸� ȳ��, �ʴ� 400���� ȸ���ӵ�
	// ĳ������ �������� ������� ����, ���۽� ĳ���Ͱ� ����� ��������� ��鿡�� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 800.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// ī�޶� ȸ���� ĳ���Ͱ� �������� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}
