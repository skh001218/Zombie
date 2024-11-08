#include "stdafx.h"
#include "Player.h"
#include "SceneGame.h"
#include "Bullet.h"
#include "UiHud.h"

Player::Player(const std::string& name)
	: GameObject(name)
{

}

void Player::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(angle);
}

void Player::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(s);
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect Player::GetLocalBounds() const
{
	return body.getLocalBounds();
}

sf::FloatRect Player::GetGlobalBounds() const
{
	return body.getGlobalBounds();
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetOrigin(Origins::MC);
}

void Player::Release()
{
}

void Player::Reset()
{
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	uiHud = dynamic_cast<UiHud*>(SCENE_MGR.GetCurrentScene()->FindGo("UiHud"));

	body.setTexture(TEXTURE_MGR.Get(textureId), true);
	SetOrigin(originPreset);
	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	direction = { 1.f, 0.f };

	curBulletCount = bulletCount = 20;
	maxHp = hp = 100;
	shootTimer = shootDelay;
	uiHud->SetAmmo(curBulletCount, bulletCount);
}

void Player::Update(float dt)
{
	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	float mag = Utils::Magnitude(direction);
	if (mag > 1.f)
	{
		 Utils::Normailize(direction);
	}

	sf::Vector2i mousePos = InputMgr::GetMousePosition();
	sf::Vector2f mouseWorldPos = SCENE_MGR.GetCurrentScene()->ScreenToWorld(mousePos);
	look = Utils::GetNormal(mouseWorldPos - position);

	SetRotation(Utils::Angle(look));
	SetPosition(position + direction * speed * dt);

	movableArea = sceneGame->FindGo("TileMap")->GetGlobalBounds();
	movableArea = { movableArea.left + 50.f, movableArea.top + 50.f,
		movableArea.left + movableArea.width - 50.f, movableArea.top + movableArea.height - 50.f };
	if (!movableArea.contains(position))
	{
		if (position.x < movableArea.left)
		{
			position.x += movableArea.left - position.x;
			SetPosition(position);
		}
		if(position.y < movableArea.top)
		{
			position.y += movableArea.top - position.y;
			SetPosition(position);
		}
		if (position.x > movableArea.width)
		{
			position.x -= position.x - movableArea.width ;
			SetPosition(position);
		}
		if (position.y > movableArea.height)
		{
			position.y -= position.y - movableArea.height ;
			SetPosition(position);
		}
	}

	shootTimer += dt;
	if (shootTimer > shootDelay && curBulletCount > 0 && InputMgr::GetMouseButton(sf::Mouse::Left))
	{
		shootTimer = 0.f;
		Shoot();
	}


	debugBox.SetBounds(body.getGlobalBounds());
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	debugBox.Draw(window);
}

void Player::OnDamage(int damge)
{
	hp -= damge;
	uiHud->SetHp(Utils::Clamp(hp, 0, maxHp), maxHp);
	if (hp <= 0 && sceneGame != nullptr)
	{
		sceneGame->OnPlayerDie(this);
	}
}

void Player::Shoot()
{
	Bullet* bullet = sceneGame->TakeBullet();
	bullet->Fire(position, look, 1000.f, 10);
	curBulletCount--;
	uiHud->SetAmmo(curBulletCount, bulletCount);
}
