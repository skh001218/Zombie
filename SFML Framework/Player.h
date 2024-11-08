#pragma once

class SceneGame;
class UiHud;

class Player : public GameObject
{
protected:
	sf::Sprite body;
	std::string textureId = "graphics/player.png";

	sf::Vector2f direction;
	sf::Vector2f look;

	sf::FloatRect movableArea;

	float speed = 500.f;

	SceneGame* sceneGame;
	UiHud* uiHud;

	float shootDelay = 0.5f;
	float shootTimer = 0.f;

	int bulletCount;
	int curBulletCount;

	int maxHp = 100;
	int hp = 0;

	DebugBox debugBox;

public:
	Player(const std::string& name = "");
	~Player() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::Sprite GetBody() const { return body; }

	sf::FloatRect GetLocalBounds() const override;
	sf::FloatRect GetGlobalBounds() const override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void OnDamage(int damge);

	int GetBulletCount() const { return bulletCount; }
	void SetBulletCount(int count) { bulletCount = count; }

	int GetCurBulletCount() const { return curBulletCount; }
	void SetCurBulletCount(int count) { curBulletCount = count; }

	int GetmaxHp() const { return maxHp; }
	void SetmaxHp(int count) { maxHp = count; }

	int GetHp() const { return hp; }
	void SetHp(int count) { hp = count; }

	float GetShootDelay() const { return shootDelay; }
	void SetShootDelay(float count) { shootDelay = count; }

	float GetSpeed() const { return speed; }
	void SetSpeed(float count) { speed = count; }

	void Shoot();
};
