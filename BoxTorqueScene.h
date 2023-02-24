#pragma once
#include "GameScene.h"
class BoxTorqueScene : public GameScene
{
public:
	BoxTorqueScene() :GameScene(L"BoxTorqueScene") {}
	~BoxTorqueScene() override = default;
	BoxTorqueScene(const BoxTorqueScene& other) = delete;
	BoxTorqueScene(BoxTorqueScene&& other) noexcept = delete;
	BoxTorqueScene& operator=(const BoxTorqueScene& other) = delete;
	BoxTorqueScene& operator=(BoxTorqueScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:
	PxRigidDynamic* m_pCubeActor{ nullptr };
	GameObject* m_pCube{ nullptr };
};

