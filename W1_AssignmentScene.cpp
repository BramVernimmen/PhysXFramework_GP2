#include "stdafx.h"
#include "W1_AssignmentScene.h"

#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"

enum InputIds : int
{
	Forward,
	Backward,
	Left,
	Right,
	Up
};

void W1_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	m_pPhysxScene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 2.f);


	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	// CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ -20.f, m_SceneContext.GetCamera()->GetPosition().y, -40.f});
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ 0.5f, 0.f, 1.f });

	//GROUND CUBE
	const XMFLOAT3 groundCubeDimension{ 350.0f, 0.1f, 350.0f };
	m_pGroundCube = new CubePosColorNorm(groundCubeDimension.x, groundCubeDimension.y, groundCubeDimension.z, XMFLOAT4{0.5f, 0.5f, 0.5f, 1.f});
	AddGameObject(m_pGroundCube);
	m_pGroundCube->Translate(0.0f, -0.25f, 0.0f);

	//GROUND CUBE ACTOR
	PxRigidStatic* pGroundCubeActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	const PxBoxGeometry groundCubeBoxGeometry = PxBoxGeometry{ groundCubeDimension.x / 2.f, groundCubeDimension.y / 2.f, groundCubeDimension.z / 2.f };
	const PxMaterial* pGroundCubeMaterial = pPhysX->createMaterial(0.2f, 0.6f, 0.3f);
	PxRigidActorExt::createExclusiveShape(*pGroundCubeActor, groundCubeBoxGeometry, *pGroundCubeMaterial);
	m_pGroundCube->AttachRigidActor(pGroundCubeActor);


	//SPHERE
	const float radius{ 2.5f };
	const int slices{ 50 };
	const int stacks{ 50 };
	const XMFLOAT4 color{ 1.f, 1.f, 0.f, 1.f };
	m_pSphere = new SpherePosColorNorm(radius, slices, stacks, color);
	AddGameObject(m_pSphere);

	//SPHERE ACTOR
	m_pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxSphereGeometry sphereGeometry = PxSphereGeometry{ radius };
	const PxMaterial* pSphereMaterial = pPhysX->createMaterial(0.1f, 0.9f, 0.5f);
	
	PxRigidBodyExt::setMassAndUpdateInertia(*m_pSphereActor, 25.f);
	PxRigidActorExt::createExclusiveShape(*m_pSphereActor, sphereGeometry, *pSphereMaterial);

	m_pSphere->AttachRigidActor(m_pSphereActor);


	//CUBES
	
	const PxMaterial* pCubeMaterial = pPhysX->createMaterial(0.5f, 0.2f, 0.8f);
	const PxBoxGeometry cubeBoxGeometry = PxBoxGeometry{ m_CubeDimension.x / 2.f, m_CubeDimension.y / 2.f, m_CubeDimension.z / 2.f };

	for (int i{ 0 }; i < m_Rows * m_Cols; ++i)
	{
		m_pCubeVec.push_back(new CubePosColorNorm(m_CubeDimension.x, m_CubeDimension.y, m_CubeDimension.z));
		AddGameObject(m_pCubeVec[i]);

		PxRigidDynamic* pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
		PxRigidActorExt::createExclusiveShape(*pCubeActor, cubeBoxGeometry, *pCubeMaterial);
		m_pCubeVec[i]->AttachRigidActor(pCubeActor);
	}










	// INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction{
			InputIds::Backward, InputTriggerState::down , VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{
			InputIds::Forward, InputTriggerState::down , VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP });
	m_SceneContext.GetInput()->AddInputAction(InputAction{
			InputIds::Right, InputTriggerState::down , VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{
			InputIds::Left, InputTriggerState::down , VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{
			InputIds::Up, InputTriggerState::pressed , VK_SPACE, -1, XINPUT_GAMEPAD_DPAD_LEFT });

	Reset();
}

void W1_AssignmentScene::Update()
{
	XMFLOAT3 camForward = m_SceneContext.GetCamera()->GetForward();
	XMFLOAT3 camRight = m_SceneContext.GetCamera()->GetRight();
	const float scaler{ 5.0f };
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Left))
	{
		m_pSphereActor->addTorque(PxVec3{ scaler * camForward.x, scaler * camForward.y, scaler * camForward.z }, PxForceMode::eIMPULSE);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Right))
	{
		m_pSphereActor->addTorque(PxVec3{ -scaler * camForward.x, -scaler * camForward.y, -scaler * camForward.z }, PxForceMode::eIMPULSE);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Forward))
	{
		m_pSphereActor->addTorque(PxVec3{ scaler * camRight.x, scaler * camRight.y , scaler * camRight.z}, PxForceMode::eIMPULSE);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Backward))
	{
		m_pSphereActor->addTorque(PxVec3{ -scaler * camRight.x, -scaler * camRight.y, -scaler * camRight.z }, PxForceMode::eIMPULSE);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Up))
	{
		m_pSphereActor->addForce(PxVec3{ 0.f, 250.f, 0.f }, PxForceMode::eIMPULSE);
	}


	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		Reset();
	}
}

void W1_AssignmentScene::Draw() const
{
}

void W1_AssignmentScene::OnSceneActivated()
{
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}

void W1_AssignmentScene::Reset()
{
	if (m_SceneContext.GetGameTime()->IsRunning())
	{
		m_pSphereActor->putToSleep();
		m_pSphereActor->wakeUp();
	}

	m_pSphere->Translate(5.f, 0.5f, -20.f);


	PxVec3 translateLoc{ -(m_Rows * m_CubeDimension.x) / 2.f, 1.0f, 50.0f };

	for (int i{ 0 }; i < m_Rows; ++i)
	{
		for (int j{ 0 }; j < m_Cols; ++j)
		{
			const int index{ (i * m_Cols) + j };
			m_pCubeVec[index]->Translate(translateLoc.x, translateLoc.y, translateLoc.z);
			m_pCubeVec[index]->RotateDegrees(0.0f, static_cast<float>(rand() % 40) - 30.f, 0.0f);
			translateLoc.x += m_CubeDimension.x + 0.5f;
		}
		translateLoc.x = -(m_Rows * m_CubeDimension.x) / 2.f;
		translateLoc.y += m_CubeDimension.y + 0.15f;
	}

}
