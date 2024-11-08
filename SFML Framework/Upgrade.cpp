#include "stdafx.h"
#include "Upgrade.h"
#include "SceneGame.h"

Upgrade::Upgrade(const std::string& name)
	: GameObject(name)
{
}

void Upgrade::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void Upgrade::SetRotation(float angle)
{
	rotation = angle;
}

void Upgrade::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void Upgrade::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void Upgrade::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void Upgrade::Init()
{
	SetUpgradeList();
}

void Upgrade::Release()
{
}

void Upgrade::Reset()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 1;

	backgruond.setTexture(TEXTURE_MGR.Get(backTexture));
	Utils::SetOrigin(backgruond, Origins::TL);

	
}

void Upgrade::Update(float dt)
{
}

void Upgrade::FixedUpdate(float dt)
{
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	if (sceneGame != nullptr)
	{
		sf::Vector2f mousePos = sceneGame->ScreenToUi(InputMgr::GetMousePosition());
		for (int i = 0; i < upgradeList.size(); ++i)
		{
			if (upgradeList[i].getGlobalBounds().contains(mousePos))
			{
				upgradeList[i].setFillColor(sf::Color::Red);
				if (InputMgr::GetMouseButtonDown(sf::Mouse::Left))
				{
					sceneGame->UpgradeInfo(i);
					SetActive(false);
					return;
				}
			}
			else
			{
				upgradeList[i].setFillColor(sf::Color::White);
			}
		}
	}

}

void Upgrade::Draw(sf::RenderWindow& window)
{
	window.draw(backgruond);
	for (auto& menu : upgradeList)
	{
		window.draw(menu);
	}
}

void Upgrade::SetUpgradeList()
{
	upgradeList.clear();
	for (int i = 0; i < 6; i++)
	{
		sf::Text menu;
		upgradeList.push_back(menu);
	}
	upgradeList[0].setString("1- INCRESED RATE OF FIRE");
	upgradeList[1].setString("2- INCRESED CLIP SIZE [NEXT RELOAD]");
	upgradeList[2].setString("3- INCRESED MAX HEALTH");
	upgradeList[3].setString("4- INCRESED RUN SPEED");
	upgradeList[4].setString("5- MORE AND BETTER HEALTH PICKUPS");
	upgradeList[5].setString("6- MORE AND BETTER AMMO PICKUPS");

	
	float textSize = 100.f;
	for (auto& menu : upgradeList)
	{
		menu.setFont(font);
		menu.setCharacterSize(textSize);
		menu.setFillColor(sf::Color::White);
		Utils::SetOrigin(menu, Origins::ML);
	}

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();
	for (int i = 0; i < 6; i++)
	{
		sf::FloatRect texSize = upgradeList[i].getLocalBounds();
		upgradeList[i].setPosition({ 200.f, size.y * 0.5f - (3 * texSize.height) + i * (texSize.height + 20.f)});
	}
}
