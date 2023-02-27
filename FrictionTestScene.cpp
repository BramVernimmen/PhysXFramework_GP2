#include "stdafx.h"
#include "FrictionTestScene.h"

#include "CubePosColorNorm.h"

void FrictionTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//GROUNDPLANE
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.0f, 0.0f, 0.0f);
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	//RAMPS
	const XMFLOAT3 rampActorDimension{ 6.f, 1.f, 3.f };
	m_pRamp1 = new CubePosColorNorm(rampActorDimension.x, rampActorDimension.y, rampActorDimension.z);
	m_pRamp2 = new CubePosColorNorm(rampActorDimension.x, rampActorDimension.y, rampActorDimension.z);
	m_pRamp3 = new CubePosColorNorm(rampActorDimension.x, rampActorDimension.y, rampActorDimension.z);
	AddGameObject(m_pRamp1);
	AddGameObject(m_pRamp2);
	AddGameObject(m_pRamp3);
	m_pRamp1->Translate(-10.f, 2.f, 0.f);
	m_pRamp2->Translate(0.f, 2.f, 0.f);
	m_pRamp3->Translate(10.f, 2.f, 0.f);
	m_pRamp1->RotateDegrees(0.f, 0.f, -25.f);
	m_pRamp2->RotateDegrees(0.f, 0.f, -25.f);
	m_pRamp3->RotateDegrees(0.f, 0.f, -25.f);


	//CUBES
	const XMFLOAT3 cubeActorDimension{ 1.f, 1.f, 1.f };
	m_pCube1 = new CubePosColorNorm(cubeActorDimension.x, cubeActorDimension.y, cubeActorDimension.z);
	m_pCube2 = new CubePosColorNorm(cubeActorDimension.x, cubeActorDimension.y, cubeActorDimension.z);
	m_pCube3 = new CubePosColorNorm(cubeActorDimension.x, cubeActorDimension.y, cubeActorDimension.z);
	AddGameObject(m_pCube1);
	AddGameObject(m_pCube2);
	AddGameObject(m_pCube3);
	m_pCube1->Translate(-11.f, 4.f, 0.f);
	m_pCube2->Translate(-1.f, 4.f, 0.f);
	m_pCube3->Translate(9.f, 4.f, 0.f);
	m_pCube1->RotateDegrees(0.f, 0.f, -25.f);
	m_pCube2->RotateDegrees(0.f, 0.f, -25.f);
	m_pCube3->RotateDegrees(0.f, 0.f, -25.f);

	//RAMP ACTORS
	PxRigidStatic* pRampActor1 = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	PxRigidStatic* pRampActor2 = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	PxRigidStatic* pRampActor3 = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	const PxBoxGeometry rampBoxGeometry = PxBoxGeometry{ rampActorDimension.x / 2.f, rampActorDimension.y / 2.f, rampActorDimension.z / 2.f };
	const PxMaterial* pRampMaterial = pPhysX->createMaterial(0.0f, 0.0f, 0.0f);

	PxRigidActorExt::createExclusiveShape(*pRampActor1, rampBoxGeometry, *pRampMaterial);
	PxRigidActorExt::createExclusiveShape(*pRampActor2, rampBoxGeometry, *pRampMaterial);
	PxRigidActorExt::createExclusiveShape(*pRampActor3, rampBoxGeometry, *pRampMaterial);
	m_pRamp1->AttachRigidActor(pRampActor1);
	m_pRamp2->AttachRigidActor(pRampActor2);
	m_pRamp3->AttachRigidActor(pRampActor3);


	//CUBE ACTORS
	PxRigidDynamic* pCubeActor1 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pCubeActor2 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pCubeActor3 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	const PxBoxGeometry cubeBoxGeometry = PxBoxGeometry{ cubeActorDimension.x / 2.f, cubeActorDimension.y / 2.f, cubeActorDimension.z / 2.f };

	const PxMaterial* pNoFrictionMaterial = pPhysX->createMaterial(0.f, 0.f, 0.f);
	const PxMaterial* pHalfFrictionMaterial = pPhysX->createMaterial(0.3f, 0.3f, 0.f);
	const PxMaterial* pFullFrictionMaterial = pPhysX->createMaterial(1.f, 1.f, 0.f);

	PxRigidActorExt::createExclusiveShape(*pCubeActor1, cubeBoxGeometry, *pNoFrictionMaterial);
	PxRigidActorExt::createExclusiveShape(*pCubeActor2, cubeBoxGeometry, *pHalfFrictionMaterial);
	PxRigidActorExt::createExclusiveShape(*pCubeActor3, cubeBoxGeometry, *pFullFrictionMaterial);

	m_pCube1->AttachRigidActor(pCubeActor1);
	m_pCube2->AttachRigidActor(pCubeActor2);
	m_pCube3->AttachRigidActor(pCubeActor3);

}

void FrictionTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pCube1->Translate(-11.f, 4.f, 0.f);
		m_pCube2->Translate(-1.f, 4.f, 0.f);
		m_pCube3->Translate(9.f, 4.f, 0.f);
	}
}

void FrictionTestScene::Draw() const
{
}

void FrictionTestScene::OnSceneActivated()
{
}

void FrictionTestScene::OnSceneDeactivated()
{
}
