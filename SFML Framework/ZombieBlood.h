#pragma once

class SceneGame;

class ZombieBlood : public GameObject
{
protected:
	sf::Sprite body;
	std::string textureId = "graphics/blood.png";

	SceneGame* scene;

	float showTime = 0.f;
public:
	ZombieBlood(const std::string& name = "");
	~ZombieBlood() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};
