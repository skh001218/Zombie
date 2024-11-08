#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "TileMap.h"
#include "Zombie.h"
#include "Bullet.h"
#include "Item.h"
#include "UiHud.h"
#include "Upgrade.h"
#include "ZombieBlood.h"

SceneGame::SceneGame()
	: Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	map = AddGo(new TileMap("TileMap"));
	upgrade = AddGo(new Upgrade("Upgrade"));
	uiHud = AddGo(new UiHud("UiHud"));
	player = AddGo(new Player("Player"));

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	Utils::SetOrigin(cursor, Origins::MC);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	gameMsg = new TextGo("fonts/zombiecontrol.ttf");
	//gameMsg.setCharacterSize(100.f);
	gameMsg->SetString("Press Enter to Start!");
	gameMsg->SetOrigin(Origins::MC);
	gameMsg->SetPosition({ size.x * 0.5f, size.y * 0.5f });
	gameMsg->Set(100);
	gameMsg->Reset();

	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f);
	Scene::Enter();

	upgrade->SetActive(false);
	score = 0;
	status = Status::None;

	wave = curWave = 1;
	uiHud->SetWave(wave);

	genHpTime = 0.f;
	genBulTime = 0.f;

	zombieSpawnArea = map->GetMapBounds();
}

void SceneGame::Exit()
{
	FRAMEWORK.GetWindow().setMouseCursorVisible(true);

	for (auto zombie : zombies)
	{
		RemoveGo(zombie);
		zombiePool.Return(zombie);
	}
	zombies.clear();

	for (auto bullet : bullets)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	bullets.clear();

	for (auto item : items)
	{
		RemoveGo(item);
		itemPool.Return(item);
	}
	items.clear();

	for (auto blood : bloods)
	{
		RemoveGo(blood);
		bloodPool.Return(blood);
	}
	bloods.clear();

	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	cursor.setPosition(ScreenToUi(InputMgr::GetMousePosition()));

	if (InputMgr::GetKeyDown(sf::Keyboard::Escape))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}
	
	if (status == Status::GameOver)
	{
		gameMsg->SetString("Press Esc To Restart");
		gameMsg->SetActive(true);
		gameMsg->Reset();
		return;
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
	{
		gameMsg->SetActive(false);
		preStatus = status;
		status = Status::Game;
		SetWave((Wave)wave);
	}

	if (status == Status::None)
		return;

	if (status == Status::Game && preStatus == Status::Upgrade)
	{
		SetWave((Wave)wave);
		preStatus = Status::Game;
	}

	if (status == Status::Upgrade)
	{
		upgrade->SetActive(true);
		return;
	}

	Scene::Update(dt);

	

	if (player != nullptr)
	{
		worldView.setCenter(player->GetPosition());
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::F1))
	{
		upgrade->SetActive(true);
	}

	if (genHpTime > genHpDelay)
	{
		GenItem(0);
		genHpTime = 0.f;
	}
	genHpTime += dt;

	if (genBulTime > genBulDelay)
	{
		GenItem(1);
		genBulTime = 0.f;
	}
	genBulTime += dt;
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	const sf::View& saveView = window.getView();
	window.setView(uiView);
	gameMsg->Draw(window);
	window.draw(cursor);
	window.setView(saveView);
}

void SceneGame::SpawnZombies(int count, Zombie::Types t)
{
	zombieCount += count;
	uiHud->SetZombieCount(zombieCount);
	for (int i = 0; i < count; ++i)
	{
		Zombie* zombie = zombiePool.Take();
		zombies.push_back(zombie);
		Zombie::Types zombieType; 
		
		if ((int)t < 0)
			zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes - 1); 
		else
			zombieType = t;
		zombie->SetType(zombieType);
		sf::Vector2f pos = Utils::RandomPointInRect(zombieSpawnArea);
		zombie->SetPosition(pos);
		
		AddGo(zombie);
	}
	zombieClear = false;
}

Bullet* SceneGame::TakeBullet()
{
	Bullet* bullet = bulletPool.Take();
	bullets.push_back(bullet);
	AddGo(bullet);
	return bullet;
}

void SceneGame::ReturnBullet(Bullet* bullet)
{
	RemoveGo(bullet);
	bulletPool.Return(bullet);
	bullets.remove(bullet);
}

void SceneGame::ReturnBlood(ZombieBlood* blood)
{
	RemoveGo(blood);
	bloodPool.Return(blood);
	bloods.remove(blood);
}

void SceneGame::OnZombieDie(Zombie* zombie)
{
	score += zombie->GetScore();
	OnZombieBlood(zombie);
	RemoveGo(zombie);
	zombiePool.Return(zombie);
	zombies.remove(zombie);
	uiHud->SetScore(score);
	zombieCount--;
	uiHud->SetZombieCount(zombieCount);

	if (zombieCount == 0 && !zombieClear)
	{
		if (wave == (int)Wave::W2)
		{
			preStatus = status;
			status = Status::GameOver;
			return;
		}
		zombieClear = true;
		wave++;
		preStatus = status;
		status = Status::Upgrade;
	}
}

void SceneGame::OnZombieBlood(Zombie* zombie)
{
	ZombieBlood* blood = bloodPool.Take();
	bloods.push_back(blood);

	sf::Vector2f pos = zombie->GetPosition();
	blood->SetPosition(pos);

	AddGo(blood);
}

void SceneGame::OnPlayerDie(Player* zombie)
{
	preStatus = status;
	status = Status::GameOver;
	player->SetActive(false);
}

void SceneGame::SetWave(Wave w)
{
	switch (w)
	{
	case Wave::W1:
		wave = (int)w;
		uiHud->SetWave(wave);
		SpawnZombies(10, Zombie::Types::Crawler);
		break;
	case Wave::W2:
		wave = (int)w;
		uiHud->SetWave(wave);
		SpawnZombies(10, Zombie::Types::Crawler);
		SpawnZombies(5, Zombie::Types::Chaser);
		break;
	case Wave::W3:
		wave = (int)w;
		uiHud->SetWave(wave);
		SpawnZombies(10, Zombie::Types::Bloater);
		SpawnZombies(20, Zombie::Types::Chaser);
		SpawnZombies(5, Zombie::Types::Crawler);
		break;
	case Wave::W4:
		wave = (int)w;
		uiHud->SetWave(wave);
		SpawnZombies(20, Zombie::Types::Chaser);
		SpawnZombies(5, Zombie::Types::Bloater);
		SpawnZombies(30);
		break;
	case Wave::W5:
		wave = (int)w;
		uiHud->SetWave(wave);
		SpawnZombies(10, Zombie::Types::Bloater);
		SpawnZombies(10, Zombie::Types::Chaser);
		SpawnZombies(50);
		break;
	default:
		break;
	}
}

void SceneGame::GenItem(int itemInfo)
{
	Item* item = itemPool.Take();
	items.push_back(item);

	item->Gen((Item::ItemInfo)itemInfo);
	AddGo(item);

}

void SceneGame::UpgradeInfo(int i)
{
	switch (i)
	{
	case 0:
	{
		float total = player->GetShootDelay() - 0.05f;
		player->SetShootDelay(total);
		break;
	}
	case 1:
	{
		int total = player->GetBulletCount() + 5;
		player->SetBulletCount(total);
		uiHud->SetAmmo(player->GetCurBulletCount(), total);
		break;
	}
	case 2:
	{
		int total = player->GetmaxHp() + 20;
		player->SetmaxHp(total);
		uiHud->SetHp(player->GetHp(), total);
		break;
	}
	case 3:
		player->SetShootDelay(player->GetShootDelay() - 0.05f);
		break;
	case 4:
		genHpDelay -= 3.f;
		break;
	case 5:
		genBulDelay -= 1.f;
		break;
	default:
		break;
	}
	preStatus = status;
	status = Status::Game;
}
