#pragma once
#include "GameScene.h"
class W2_AssignmentScene : public GameScene
{
public:
	W2_AssignmentScene() :GameScene(L"W2_AssignmentScene") {}
	~W2_AssignmentScene() override = default;
	W2_AssignmentScene(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene(W2_AssignmentScene&& other) noexcept = delete;
	W2_AssignmentScene& operator=(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene& operator=(W2_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:
	GameObject* m_pLevelTriangle{ nullptr };

	GameObject* m_pSphere1{ nullptr };
	GameObject* m_pSphere2{ nullptr };
	GameObject* m_pSphere3{ nullptr };

	GameObject* m_pCube1{ nullptr };
	GameObject* m_pCube2{ nullptr };

	PxRigidStatic* m_pTriggerBlue{ nullptr };
	GameObject* m_pTriggerBlueCube{ nullptr };
	PxRigidStatic* m_pTriggerRed{ nullptr };
	GameObject* m_pTriggerRedCube{ nullptr };

	FMOD::Channel* m_pChannel2D{ nullptr };
	FMOD::Sound* m_pSound2D{ nullptr };

	GameObject* m_pHingeCubeBlue{ nullptr };
	PxRevoluteJoint* m_pHingeJointBlue{ nullptr };
	GameObject* m_pHingeCubeRed{ nullptr };
	PxRevoluteJoint* m_pHingeJointRed{ nullptr };

	bool m_ShouldUpdateBlue{ false };
	bool m_ShouldUpdateRed{ false };

	void Reset();
};

