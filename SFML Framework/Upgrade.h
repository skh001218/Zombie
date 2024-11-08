#pragma once

class Upgrade : public GameObject
{
protected:
	sf::Sprite backgruond;
	std::string backTexture = "graphics/background.png";
	
	std::vector<sf::Text> upgradeList;
	sf::Font& font = FONT_MGR.Get("fonts/zombiecontrol.ttf");

public:
	Upgrade(const std::string& name = "");
	~Upgrade() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetUpgradeList();
};
