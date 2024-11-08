#pragma once
#include "Scene.h"
#include "Zombie.h"

class Player;
class Zombie;
class Bullet;
class Map;
class TileMap;
class Item;
class UiHud;
class Upgrade;
class ZombieBlood;

enum class Wave
{
	W1 = 1,
	W2,
	W3,
	W4,
	W5,
};

enum class Status
{
	None = -1,
	Game,
	GameOver,
	Pause,
	Upgrade,
};

class SceneGame : public Scene
{
protected:
	TileMap* map;
	Player* player;
	UiHud* uiHud;
	Upgrade* upgrade;

	std::list<Zombie*> zombies;
	ObjectPool<Zombie> zombiePool;
	sf::FloatRect zombieSpawnArea;

	std::list<ZombieBlood*> bloods;
	ObjectPool<ZombieBlood> bloodPool;

	std::list<Bullet*> bullets;
	ObjectPool<Bullet> bulletPool;

	std::list<Item*> items;
	ObjectPool<Item> itemPool;

	float genHpTime = 0.f;
	float genHpDelay = 30.f;
	float genBulTime = 0.f;
	float genBulDelay = 15.f;

	sf::Sprite cursor;
	TextGo* gameMsg;

	Status preStatus;
	Status status;

	int score = 0;

	int curWave = 0;
	int wave = 0;

	int zombieCount = 0;
	bool zombieClear = true;

public:
	SceneGame();
	virtual ~SceneGame() = default;

	void Init();
	void Release();
	void Enter();
	void Exit();
	void Update(float dt);
	void Draw(sf::RenderWindow& window) override;

	void SpawnZombies(int count, Zombie::Types t = Zombie::Types::None);
	Bullet* TakeBullet();
	void ReturnBullet(Bullet* bullet);

	void ReturnBlood(ZombieBlood* blood);

	const std::list<Zombie*>& GetZombieList() const { return zombies; }

	void OnZombieDie(Zombie* zombie);
	void OnZombieBlood(Zombie* zombie);

	void OnPlayerDie(Player* zombie);
	void SetWave(Wave w);

	void GenItem(int item);
	void UpgradeInfo(int i);
};

