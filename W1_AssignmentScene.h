#pragma once
#include "GameScene.h"
class W1_AssignmentScene : public GameScene
{
public:
	W1_AssignmentScene() :GameScene(L"W1_AssignmentScene") {}
	~W1_AssignmentScene() override = default;
	W1_AssignmentScene(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene(W1_AssignmentScene&& other) noexcept = delete;
	W1_AssignmentScene& operator=(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene& operator=(W1_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	GameObject* m_pGroundCube{ nullptr };
	PxRigidDynamic* m_pSphereActor{ nullptr };
	GameObject* m_pSphere{ nullptr };

	std::vector<GameObject*> m_pCubeVec{};
	const int m_Rows{ 10 };
	const int m_Cols{ 10 };

	const XMFLOAT3 m_CubeDimension{ 2.f,2.f,2.f };

	void Reset();
};

