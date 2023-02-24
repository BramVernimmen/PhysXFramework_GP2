#include "stdafx.h"
#include "BouncingBallScene.h"

#include "SpherePosColorNorm.h"

void BouncingBallScene::Initialize()
{
	EnablePhysxDebugRendering(true);

	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pBounceMaterial = pPhysX->createMaterial(0.5f, 0.5f, 1.f);
	const PxMaterial* pHalfBounceMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.5f);
	const PxMaterial* pNotBounceMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.0f);
	
	//SPHERE
	const float radius{ 1.f };
	const int slices{ 50 };
	const int stacks{ 50 };
	const XMFLOAT4 color{ 0.2f, 1.f, 0.2f, 1.f };
	m_pSphere1 = new SpherePosColorNorm(radius, slices, stacks, color);
	m_pSphere2 = new SpherePosColorNorm(radius, slices, stacks, color);
	m_pSphere3= new SpherePosColorNorm(radius, slices, stacks, color);
	AddGameObject(m_pSphere1);
	AddGameObject(m_pSphere2);
	AddGameObject(m_pSphere3);
	m_pSphere1->Translate(-5.f, 10.f, 0.f);
	m_pSphere2->Translate(0.f, 10.f, 0.f);
	m_pSphere3->Translate(5.f, 10.f, 0.f);

	//SPHERE ACTOR
	PxRigidDynamic* pSphereActorBounce = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphereActorHalfBounce = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidDynamic* pSphereActorNotBounce = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxSphereGeometry sphereGeometry = PxSphereGeometry{ radius / 2.f };

	PxRigidActorExt::createExclusiveShape(*pSphereActorBounce, sphereGeometry, *pBounceMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphereActorHalfBounce, sphereGeometry, *pHalfBounceMaterial);
	PxRigidActorExt::createExclusiveShape(*pSphereActorNotBounce, sphereGeometry, *pNotBounceMaterial);

	m_pSphere1->AttachRigidActor(pSphereActorBounce);
	m_pSphere2->AttachRigidActor(pSphereActorHalfBounce);
	m_pSphere3->AttachRigidActor(pSphereActorNotBounce);

	//GROUNDPLANE
	const PxMaterial* pDefaultMaterial = pPhysX->createMaterial(0.5f, 0.5f, 0.2f);
	const auto pGroundActor = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGroundActor, PxPlaneGeometry{}, *pDefaultMaterial);
	m_pPhysxScene->addActor(*pGroundActor);
}

void BouncingBallScene::Update()
{
}

void BouncingBallScene::Draw() const
{
}

void BouncingBallScene::OnSceneActivated()
{
}

void BouncingBallScene::OnSceneDeactivated()
{
}
