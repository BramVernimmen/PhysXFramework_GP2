#include "stdafx.h"
#include "PhysXTestScene.h"

#include "CubePosColorNorm.h"

void PhysXTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	//CUBE
	const XMFLOAT3 actorDimension{ 2.f, 2.f, 2.f };
	m_pCube = new CubePosColorNorm(actorDimension.x, actorDimension.y, actorDimension.z);
	AddGameObject(m_pCube);
	m_pCube->Translate(0.f, 10.f, 0.f);
	m_pCube->RotateDegrees(30.f, 45.f, 25.f);

	//CUBE ACTOR
	PxRigidDynamic* pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeometry = PxBoxGeometry{ actorDimension.x / 2.f, actorDimension.y / 2.f, actorDimension.z / 2.f };

	// long methode
	//PxShape* pCubeShape = pPhysX->createShape(boxGeometry, *pDefaultMaterial, true);
	//pCubeActor->attachShape(*pCubeShape);
	//m_pPhysxScene->addActor(*pCubeActor);

	PxRigidActorExt::createExclusiveShape(*pCubeActor, boxGeometry, *pDefaultMaterial);

	//Link CUBE WITH CUBE_ACTOR
	m_pCube->AttachRigidActor(pCubeActor);

	//GROUNDPLANE
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);
}

void PhysXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pCube->Translate(0.f, 10.f, 0.f);
	}
}

void PhysXTestScene::Draw() const
{
}

void PhysXTestScene::OnSceneActivated()
{
}

void PhysXTestScene::OnSceneDeactivated()
{
}
