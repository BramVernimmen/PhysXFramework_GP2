#pragma once
#include "GameScene.h"
class PhysXMeshTestScene : public GameScene
{
public:
	PhysXMeshTestScene() :GameScene(L"PhysXMeshTestScene") {}
	~PhysXMeshTestScene() override = default;
	PhysXMeshTestScene(const PhysXMeshTestScene& other) = delete;
	PhysXMeshTestScene(PhysXMeshTestScene&& other) noexcept = delete;
	PhysXMeshTestScene& operator=(const PhysXMeshTestScene& other) = delete;
	PhysXMeshTestScene& operator=(PhysXMeshTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;

private:
	GameObject* m_pChairConvex{ nullptr };
	GameObject* m_pChairTriangle{ nullptr };

	PxVec3 m_KinematicPosition{};

};

