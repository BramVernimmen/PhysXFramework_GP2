#include "stdafx.h"
#include "PhysXMeshTestScene.h"

#include "MeshObject.h"
#include "PhysxManager.h"
#include "ContentManager.h"

void PhysXMeshTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	auto pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);

	// Ground plane
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, {0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);

	// Convex Chair
	m_pChairConvex = new MeshObject(L"Resources/Meshes/Chair.ovm");
	AddGameObject(m_pChairConvex);

	const auto pConvexMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc");
	const auto pConvexActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pConvexActor, PxConvexMeshGeometry{pConvexMesh}, *pDefaultMaterial);

	PxVec3 massLocalPose{0.f, 4.f, 0.f};
	PxRigidBodyExt::updateMassAndInertia(*pConvexActor, 10.f, &massLocalPose);

	m_pChairConvex->AttachRigidActor(pConvexActor);

	m_pChairConvex->Translate(0.f, 12.f, 0.f);

	// Triangle Chair
	m_pChairTriangle = new MeshObject(L"Resources/Meshes/Chair.ovm");
	AddGameObject(m_pChairTriangle);

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Chair.ovpt");
	const auto pTriangleActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pDefaultMaterial);

	m_pChairTriangle->AttachRigidActor(pTriangleActor);

	m_pChairTriangle->Translate(10.f, 0.f, 0.f);
	m_KinematicPosition = { 10.f, 0.f, 0.f };
}

void PhysXMeshTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pChairConvex->Translate(0.f, 12.f, 0.f);
		m_pChairConvex->RotateDegrees(0.f, 0.f, 0.f);

		m_pChairTriangle->Translate(10.f, 0.f, 0.f);
		m_KinematicPosition = { 10.f, 0.f, 0.f };

	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_KinematicPosition.x -= 5.f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPosition });
		//m_pChairTriangle->GetRigidActor()->setGlobalPose(PxTransform{ m_KinematicPosition });
	}
}

void PhysXMeshTestScene::Draw() const
{
}
