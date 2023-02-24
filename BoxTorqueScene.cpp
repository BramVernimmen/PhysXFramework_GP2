#include "stdafx.h"
#include "BoxTorqueScene.h"

#include "CubePosColorNorm.h"

enum InputIds : int
{
	Up,
	Down,
	Left,
	Right
};


void BoxTorqueScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	m_pPhysxScene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, 1.f);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();

	//GROUNDPLANE
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	// CAMERA
	m_SceneContext.GetCamera()->SetPosition(XMFLOAT3{ 0.f, 30.f, -30.f });
	m_SceneContext.GetCamera()->SetForward(XMFLOAT3{ 0.f, -1.0f, 1.f });

	//CUBE
	const XMFLOAT3 actorDimension{ 2.f, 2.f, 2.f };
	m_pCube = new CubePosColorNorm(actorDimension.x, actorDimension.y, actorDimension.z);
	AddGameObject(m_pCube);

	//CUBE ACTOR
	m_pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeometry = PxBoxGeometry{ actorDimension.x / 2.f, actorDimension.y / 2.f, actorDimension.z / 2.f };
	PxRigidActorExt::createExclusiveShape(*m_pCubeActor, boxGeometry, *pDefaultMaterial);
	m_pCube->AttachRigidActor(m_pCubeActor);


	// INPUT
	m_SceneContext.GetInput()->AddInputAction(InputAction{
			InputIds::Down, InputTriggerState::down , VK_DOWN, -1, XINPUT_GAMEPAD_DPAD_DOWN });
	m_SceneContext.GetInput()->AddInputAction(InputAction{
			InputIds::Up, InputTriggerState::down , VK_UP, -1, XINPUT_GAMEPAD_DPAD_UP });
	m_SceneContext.GetInput()->AddInputAction(InputAction{
			InputIds::Right, InputTriggerState::down , VK_RIGHT, -1, XINPUT_GAMEPAD_DPAD_RIGHT });
	m_SceneContext.GetInput()->AddInputAction(InputAction{
			InputIds::Left, InputTriggerState::down , VK_LEFT, -1, XINPUT_GAMEPAD_DPAD_LEFT });
}

void BoxTorqueScene::Update()
{
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Left))
	{
		m_pCubeActor->addTorque(PxVec3{ 0.f, 0.f, 0.5f }, PxForceMode::eIMPULSE);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Right))
	{
		m_pCubeActor->addTorque(PxVec3{ 0.f, 0.f, -0.5f }, PxForceMode::eIMPULSE);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Up))
	{
		m_pCubeActor->addTorque(PxVec3{ 0.5f, 0.f, 0.f }, PxForceMode::eIMPULSE);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(InputIds::Down))
	{
		m_pCubeActor->addTorque(PxVec3{ -0.5f, 0.f, 0.f }, PxForceMode::eIMPULSE);
	}
}

void BoxTorqueScene::Draw() const
{
}

void BoxTorqueScene::OnSceneActivated()
{
}

void BoxTorqueScene::OnSceneDeactivated()
{
}