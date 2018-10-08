#pragma once
#include "GameScene.h"

class PickScene : public GameScene
{
public:
	PickScene();
	virtual ~PickScene() = default;

	PickScene(const PickScene& other) = delete;
	PickScene(PickScene&& other) noexcept = delete;
	PickScene& operator=(const PickScene& other) = delete;
	PickScene& operator=(PickScene&& other) noexcept = delete;
protected:
	void Initialize(const GameContext& gameContext) override;
	void Update(const GameContext& gameContext) override;
	void Draw(const GameContext& gameContext) override;
};


