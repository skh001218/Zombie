#include "stdafx.h"
#include "Item.h"
#include "SceneGame.h"
#include "Player.h"
#include "UiHud.h"

Item::Item(const std::string& name)
	: GameObject(name)
{
}

void Item::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Item::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Item::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Item::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Item::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect Item::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect Item::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

void Item::Init()
{
}

void Item::Release()
{
}

void Item::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(itemTexture));
	SetPosition({0.f, 0.f});
	SetRotation(0.f);
	SetOrigin(Origins::MC);
	scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
}

void Item::Update(float dt)
{
	debugBox.SetBounds(GetGlobalBounds());
}

void Item::FixedUpdate(float dt)
{
	if (scene == nullptr)
		return;

	debugBox.SetOutlineColor(sf::Color::Green);
	if (!player->IsActive())
		return;

	UiHud* uiHud  = dynamic_cast<UiHud*>(SCENE_MGR.GetCurrentScene()->FindGo("UiHud"));

	sf::FloatRect bounds = GetGlobalBounds();
	sf::FloatRect playerBounds = player->GetGlobalBounds();
	if (bounds.intersects(playerBounds))
	{
		debugBox.SetOutlineColor(sf::Color::Red);
		if (Utils::CheckCollision(body, player->GetBody()))
		{
			if (itemInfo == ItemInfo::Bullet)
			{
				int curBul = player->GetCurBulletCount();
				curBul += 10;
				int totalBul = player->GetBulletCount();
				player->SetCurBulletCount(Utils::Clamp(curBul, 0, totalBul));
				uiHud->SetAmmo(Utils::Clamp(curBul, 0, totalBul), totalBul);
			}
			if (itemInfo == ItemInfo::Hp)
			{
				int curHp = player->GetHp();
				curHp += 20;
				int totalHp = player->GetmaxHp();
				player->SetHp(Utils::Clamp(curHp, 0, totalHp));
				uiHud->SetHp(Utils::Clamp(curHp, 0, totalHp), totalHp);
			}
			this->SetActive(false);
		}
	}
}

void Item::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	debugBox.Draw(window);
}

void Item::Gen(ItemInfo item)
{
	switch (item)
	{
	case ItemInfo::Hp:
		itemInfo = item;
		itemTexture = "graphics/health_pickup.png";
		break;
	case ItemInfo::Bullet:
		itemInfo = item;
		itemTexture = "graphics/ammo_pickup.png";
		break;
	default:
		break;
	}
	body.setTexture(TEXTURE_MGR.Get(itemTexture));
	sf::Vector2f pos = Utils::RandomInUnitCircle() * 500.f;
	Player* player = dynamic_cast<Player*>(scene->FindGo("Player"));

	SetPosition(player->GetPosition() + pos);
	
}
