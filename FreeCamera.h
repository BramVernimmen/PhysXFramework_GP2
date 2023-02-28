#include "BaseCamera.h"

#pragma once
class FreeCamera: public BaseCamera
{
public:
	FreeCamera();
	virtual ~FreeCamera() = default;

	virtual void Update(const SceneContext& sceneContext) override;
	 void SetForward(const DirectX::XMFLOAT3& newForward) override;

private:
	POINT m_PrevCursorPosition;
	float m_TotalYaw, m_TotalPitch;
	float m_Speed;
	DirectX::XMFLOAT3 m_Direction;
	bool m_MouseDownPreviousFrame;
	char m_UpKey, m_LeftKey;
};

