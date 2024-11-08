#pragma once

class SceneGame;
class Player;


class Item : public GameObject
{
public:
	enum class ItemInfo
	{
		Hp,
		Bullet,
	};
protected:
	sf::Sprite body;
	std::string itemTexture = "graphics/health_pickup.png";

	ItemInfo itemInfo;

	SceneGame* scene;

	DebugBox debugBox;
	Player* player = nullptr;
public:
	Item(const std::string& name = "");
	~Item() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() const override;
	sf::FloatRect GetGlobalBounds() const override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void Gen(ItemInfo item);
};
