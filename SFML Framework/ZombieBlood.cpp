#include "stdafx.h"
#include "ZombieBlood.h"
#include "SceneGame.h"

ZombieBlood::ZombieBlood(const std::string& name)
	: GameObject(name)
{
}

void ZombieBlood::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void ZombieBlood::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void ZombieBlood::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void ZombieBlood::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void ZombieBlood::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

void ZombieBlood::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
}

void ZombieBlood::Release()
{
}

void ZombieBlood::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(textureId));
	scene = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());

	SetPosition({ 0.f, 0.f });
	SetRotation(0.f);
	SetOrigin(Origins::MC);
}

void ZombieBlood::Update(float dt)
{
	if (showTime > 3.f)
	{
		showTime = 0;
		scene->ReturnBlood(this);
	}
	showTime += dt;
}

void ZombieBlood::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}
