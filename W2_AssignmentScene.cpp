#include "stdafx.h"
#include "W2_AssignmentScene.h"
#include "MeshObject.h"
#include "ContentManager.h"
#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"
#include "SoundManager.h"

void W2_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	m_pPhysxScene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);

	auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	// Ground plane
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, {0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	// Triangle Chair
	m_pLevelTriangle = new MeshObject(L"Resources/Meshes/Level.ovm");
	AddGameObject(m_pLevelTriangle);

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");
	const auto pTriangleActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	m_pLevelTriangle->AttachRigidActor(pTriangleActor);


	// Spheres
	m_pSphere1 = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	m_pSphere2 = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	m_pSphere3 = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere1);
	AddGameObject(m_pSphere2);
	AddGameObject(m_pSphere3);
	const auto pSphereActor1 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	const auto pSphereActor2 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	const auto pSphereActor3 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor1, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphereActor2, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphereActor3, PxSphereGeometry{ 1.f }, *pDefaultMaterial);
	m_pSphere1->AttachRigidActor(pSphereActor1);
	m_pSphere2->AttachRigidActor(pSphereActor2);
	m_pSphere3->AttachRigidActor(pSphereActor3);
	m_pSphere1->Translate(0.f, 6.f, 0.f);
	m_pSphere2->Translate(-2.f, 25.f, 0.f);
	m_pSphere3->Translate(2.f, 25.f, 0.f);

	// Cubes
	float width{ 1.5f };
	float height{ 1.5f };
	float depth{ 1.5f };
	m_pCube1 = new CubePosColorNorm(width, height, depth, XMFLOAT4{Colors::Blue});
	m_pCube2 = new CubePosColorNorm(width, height, depth, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pCube1);
	AddGameObject(m_pCube2);
	const auto pCubeActor1 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	const auto pCubeActor2 = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pCubeActor1, PxBoxGeometry{ width / 2.f, height / 2.f, depth / 2.f }, *pDefaultMaterial);
	PxRigidActorExt::createExclusiveShape(*pCubeActor2, PxBoxGeometry{ width / 2.f, height / 2.f, depth / 2.f }, *pDefaultMaterial);
	pCubeActor1->setMass(0.1f);
	pCubeActor2->setMass(0.1f);
	m_pCube1->AttachRigidActor(pCubeActor1);
	m_pCube2->AttachRigidActor(pCubeActor2);
	m_pCube1->Translate(-4.f, 6.f, 0.f);
	m_pCube2->Translate(4.f, 6.f, 0.f);

	// TRIGGER BLUE
	m_pTriggerBlue = pPhysX->createRigidStatic(PxTransform{ {-7.25f, 2.5f, 0.f} });
	const auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerBlue, PxBoxGeometry{ width / 2.f, height / 4.f, depth * 2.f}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*m_pTriggerBlue);
	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pTriggerBlueCube = new CubePosColorNorm(width, height / 2.f, depth * 4.f, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pTriggerBlueCube);
	m_pTriggerBlueCube->Translate(-7.25f, 2.5f, 0.f);



	// TRIGGER RED
	m_pTriggerRed = pPhysX->createRigidStatic(PxTransform{ {7.25f, 2.5f, 0.f} });
	const auto pShapeSecond = PxRigidActorExt::createExclusiveShape(*m_pTriggerRed, PxBoxGeometry{ width / 2.f, height / 4.f, depth * 2.f }, *pDefaultMaterial);
	m_pPhysxScene->addActor(*m_pTriggerRed);
	pShapeSecond->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShapeSecond->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);


	m_pTriggerRedCube = new CubePosColorNorm(width, height / 2.f, depth * 4.f, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pTriggerRedCube);
	m_pTriggerRedCube->Translate(7.25f, 2.5f, 0.f);



	const auto pFMod = SoundManager::GetInstance()->GetSystem();
	// SOUND 2D
	FMOD_RESULT result = pFMod->createStream("Resources/Sounds/bell.mp3", FMOD_2D, nullptr, &m_pSound2D);
	SoundManager::GetInstance()->ErrorCheck(result);


	// revalue the width, height and depth for these
	height /= 4.f;
	depth *= 4.f;

	// Hinge Cube
	m_pHingeCubeBlue = new CubePosColorNorm(width, height, depth, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pHingeCubeBlue);
	m_pHingeCubeBlue->Translate(-8.5f, 16.75f, 0.f);
	const auto pHingeCubeBlueActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	pHingeCubeBlueActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pHingeCubeBlueActor, PxBoxGeometry{ width / 2.f, height / 2.f, depth / 2.f }, * pDefaultMaterial);
	m_pHingeCubeBlue->AttachRigidActor(pHingeCubeBlueActor);

	// Hinges
	m_pHingeJointBlue = PxRevoluteJointCreate(*pPhysX, 
		m_pHingeCubeBlue->GetRigidActor(), PxTransform{ +width / 2.f + 0.1f, -height / 2.f, 0.f , PxQuat(-PxPiDivTwo, {0,1,0}) * PxQuat(PxPiDivTwo, {1,0,0})},
		m_pLevelTriangle->GetRigidActor(), PxTransform{ -8.5f + width / 2.f + 0.1f, 16.75f - height / 2.f, 0.f  , PxQuat(-PxPiDivTwo, {0,1,0}) * PxQuat(PxPiDivTwo, {1,0,0}) });
	m_pHingeJointBlue->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);

	m_pHingeJointBlue->setDriveVelocity(-10.f);
	m_pHingeJointBlue->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, false);

	// Hinge Cube
	m_pHingeCubeRed = new CubePosColorNorm(width, height, depth, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pHingeCubeRed);
	m_pHingeCubeRed->Translate(8.5f, 16.75f, 0.f);
	const auto pHingeCubeRedActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	pHingeCubeRedActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pHingeCubeRedActor, PxBoxGeometry{ width / 2.f, height / 2.f, depth / 2.f }, * pDefaultMaterial);
	m_pHingeCubeRed->AttachRigidActor(pHingeCubeRedActor);

	// Hinges
	m_pHingeJointRed = PxRevoluteJointCreate(*pPhysX, 
		m_pHingeCubeRed->GetRigidActor(), PxTransform{ -width / 2.f - 0.1f, -height / 2.f, 0.f , PxQuat(PxPiDivTwo, {0,1,0}) * PxQuat(PxPiDivTwo, {1,0,0})},
		m_pLevelTriangle->GetRigidActor(), PxTransform{ 8.5f - width / 2.f - 0.1f, 16.75f - height / 2.f, 0.f  , PxQuat(PxPiDivTwo, {0,1,0}) * PxQuat(PxPiDivTwo, {1,0,0}) });
	m_pHingeJointRed->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);

	m_pHingeJointRed->setDriveVelocity(-10.f);
	m_pHingeJointRed->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, false);



}

void W2_AssignmentScene::Update()
{
	constexpr float force{ 5.f };
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere1->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0.f, 0.f, force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere1->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0.f, 0.f, -force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, VK_SPACE))
	{
		m_pSphere1->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0.f, force * 2.f, 0.f }, PxForceMode::eIMPULSE);
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pSphere1->Translate(0.f, 6.f, 0.f);
		m_pSphere1->GetRigidActor()->is<PxRigidDynamic>()->putToSleep();
		m_pSphere1->GetRigidActor()->is<PxRigidDynamic>()->wakeUp();
		m_pSphere2->Translate(-2.f, 25.f, 0.f);
		m_pSphere2->GetRigidActor()->is<PxRigidDynamic>()->putToSleep();
		m_pSphere2->GetRigidActor()->is<PxRigidDynamic>()->wakeUp();
		m_pSphere3->Translate(2.f, 25.f, 0.f);
		m_pSphere3->GetRigidActor()->is<PxRigidDynamic>()->putToSleep();
		m_pSphere3->GetRigidActor()->is<PxRigidDynamic>()->wakeUp();

		m_pCube1->Translate(-4.f, 6.f, 0.f);
		m_pCube1->RotateDegrees(0.f, 0.f, 0.f);
		m_pCube1->GetRigidActor()->is<PxRigidDynamic>()->putToSleep();
		m_pCube1->GetRigidActor()->is<PxRigidDynamic>()->wakeUp();
		m_pCube2->Translate(4.f, 6.f, 0.f);
		m_pCube2->RotateDegrees(0.f, 0.f, 0.f);
		m_pCube2->GetRigidActor()->is<PxRigidDynamic>()->putToSleep();
		m_pCube2->GetRigidActor()->is<PxRigidDynamic>()->wakeUp();

		m_pHingeCubeBlue->RotateDegrees(0.f, 0.f, 0.f);
		m_pHingeCubeBlue->Translate(-8.5f, 16.75f, 0.f);
		m_pHingeCubeRed->RotateDegrees(0.f, 0.f, 0.f);
		m_pHingeCubeRed->Translate(8.5f, 16.75f, 0.f);

	}

	if (m_ShouldUpdateBlue)
	{
		
		m_pHingeCubeBlue->GetRigidActor()->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
		m_pHingeCubeBlue->GetRigidActor()->is<PxRigidDynamic>()->wakeUp();

		m_pHingeJointBlue->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
		
		Logger::GetInstance()->LogDebug(std::to_wstring(m_pHingeJointBlue->getAngle()));

		// check if the hinge has rotated for 90 degrees, if so we want it to stop
		if (m_pHingeJointBlue->getAngle() <= -PxPiDivTwo)
		{
			m_pHingeJointBlue->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, false);

			m_pHingeCubeBlue->GetRigidActor()->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			m_ShouldUpdateBlue = false;
		}
	}
	if (m_ShouldUpdateRed)
	{
		
		m_pHingeCubeRed->GetRigidActor()->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, false);
		m_pHingeCubeRed->GetRigidActor()->is<PxRigidDynamic>()->wakeUp();

		m_pHingeJointRed->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
		
		Logger::GetInstance()->LogDebug(std::to_wstring(m_pHingeJointRed->getAngle()));

		// check if the hinge has rotated for 90 degrees, if so we want it to stop
		if (m_pHingeJointRed->getAngle() <= -PxPiDivTwo)
		{
			m_pHingeJointRed->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, false);

			m_pHingeCubeRed->GetRigidActor()->is<PxRigidDynamic>()->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
			m_ShouldUpdateRed = false;
		}
	}
}

void W2_AssignmentScene::Draw() const
{
}

void W2_AssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (auto i{ 0 }; i < count; ++i)
	{
		//ignore delted shapes
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER | PxTriggerPairFlag::eREMOVED_SHAPE_OTHER))
			continue;

		const PxTriggerPair& pair = pairs[i];
		if (pair.triggerActor == m_pTriggerBlue && pair.otherActor == m_pCube1->GetRigidActor()) // LEFT TRIGGER
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) // ENTERED
			{
				SoundManager::GetInstance()->GetSystem()->playSound(m_pSound2D, nullptr, false, &m_pChannel2D);
				m_ShouldUpdateBlue = true;
			}
		}
		else if (pair.triggerActor == m_pTriggerRed && pair.otherActor == m_pCube2->GetRigidActor()) // RIGHT TRIGGER
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND) // ENTERED
			{
				SoundManager::GetInstance()->GetSystem()->playSound(m_pSound2D, nullptr, false, &m_pChannel2D);
				m_ShouldUpdateRed = true;
			}
		}
	}
}
