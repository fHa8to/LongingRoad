#include "DxLib.h"
#include "Bee.h"
#include "Slime.h"
#include "Knight.h"
#include "EnemyManager.h"

namespace
{
	//ステージ1で出てくる敵の数
	constexpr int kStage1EnemyNum = 3;

	//ステージ2で出てくる敵の数
	constexpr int kStage2EnemyNum = 5;

	//ステージ3で出てくる敵の数
	constexpr int kStage3EnemyNum = 7;

	//ステージ4で出てくる敵の数
	constexpr int kStage4EnemyNum = 10;

	//3秒に1回敵を生成する
	constexpr int kTimer = 180;

	//0ならハチを生成
	constexpr int kBee = 0;

	//1ならスライムを生成
	constexpr int kSlime = 1;

	//プレイヤーの場所によってエネミーを生成する
	constexpr float kStage1playerPosX = -4500.0f;
	
	constexpr float kStage2playerPosX = 265.0f;
	
	constexpr float kStage3playerPosX = 6175.0f;
	
	constexpr float kStage4playerPosX = 12000.0f;

	//残り人数を示す位置
	constexpr int kRestEnemysX = 850;
	constexpr int kRestEnemysY = 50;

}

EnemyManager::EnemyManager():m_timer(0),m_enemyNum(0),m_isClear1(false),m_isClear2(false),
m_isClear3(false),m_isClear4(false)
{
	m_beeHandle = MV1LoadModel("data/model/Enemy/Bee.mv1");
	m_slimeHandle = MV1LoadModel("data/model/Enemy/Slime.mv1");
}

EnemyManager::~EnemyManager()
{
	MV1DeleteModel(m_beeHandle);
	MV1DeleteModel(m_slimeHandle);

	for (int i = 0; i < kEnemyNum; i++)
	{
		delete m_pBee[i];
		delete m_pSlime[i];

		m_pBee[i] = nullptr;
		m_pSlime[i] = nullptr;
	}
}

void EnemyManager::Init()
{
	m_enemyNumRetention1 = kStage1EnemyNum;

	m_enemyNumRetention2 = kStage2EnemyNum;

	m_enemyNumRetention3 = kStage3EnemyNum;

	m_enemyNumRetention4 = kStage4EnemyNum;

	for (int i = 0; i < kEnemyNum; i++)
	{
		m_pBee[i] = nullptr;
		m_pSlime[i] = nullptr;
	}
}

void EnemyManager::Update(Knight* knight,VECTOR playerPos, Rect playerCollision, Rect playerAttackCollision)
{

	CreateEnemy(playerPos);

	StageClear(playerPos);
	
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (m_pBee[i])
		{
			m_pBee[i]->Update(knight,playerPos);
			
			m_beeCollision[i] = m_pBee[i]->GetBeeCollision();

			m_pBee[i]->HitAttack(playerAttackCollision);

			m_pBee[i]->HitPlayer(knight,playerCollision);

			m_beeHp[i] = m_pBee[i]->GetBeeHp();

			if (m_beeHp[i] <= 0)
			{
				if (playerPos.x < kStage1playerPosX)
				{
					m_enemyNumRetention1--;
				}
				else if (playerPos.x < kStage2playerPosX)
				{
					m_enemyNumRetention2--;
				}
				else if (playerPos.x < kStage3playerPosX)
				{
					m_enemyNumRetention3--;
				}
				else if (playerPos.x < kStage4playerPosX)
				{
					m_enemyNumRetention3--;
				}
				delete m_pBee[i];
				m_pBee[i] = nullptr;
			}

			
		}
		if (m_pSlime[i])
		{
			m_pSlime[i]->Update(knight, playerPos);
	
			m_slimeCollision[i] = m_pSlime[i]->GetSlimeCollision();

			m_pSlime[i]->HitAttack(playerAttackCollision);

			m_pSlime[i]->HitPlayer(knight, playerCollision);

			m_slimeHp[i] = m_pSlime[i]->GetSlimeHp();

			if (m_slimeHp[i] <= 0)
			{
				if (playerPos.x < kStage1playerPosX)
				{
					m_enemyNumRetention1--;
				}
				else if (playerPos.x < kStage2playerPosX)
				{
					m_enemyNumRetention2--;
				}
				else if (playerPos.x < kStage3playerPosX)
				{
					m_enemyNumRetention3--;
				}
				else if (playerPos.x < kStage4playerPosX)
				{
					m_enemyNumRetention4--;
				}
				delete m_pSlime[i];
				m_pSlime[i] = nullptr;
			}
		}
	}
}

void EnemyManager::Draw()
{
	DrawEnemys();

	for (int i = 0; i < kEnemyNum; i++)
	{
		if (m_pBee[i])
		{
			m_pBee[i]->Draw();
		}
	}

	for (int i = 0; i < kEnemyNum; i++)
	{
		if (m_pSlime[i])
		{
			m_pSlime[i]->Draw();
		}
	}
}

void EnemyManager::CreateEnemy(VECTOR playerPos)
{
	if (playerPos.x <= kStage1playerPosX)
	{
		if (m_enemyNum < kStage1EnemyNum)
		{
			m_timer++;
			if (m_timer == kTimer)
			{
				m_randomEnemy = GetRand(1);
				m_timer = 0;

				if (m_randomEnemy == kBee)
				{
					CreateBee(playerPos);
				}
				if (m_randomEnemy == kSlime)
				{
					CreateSlime(playerPos);
				}
			}
		}	
	}
	else if (playerPos.x <= kStage2playerPosX)
	{
		if (m_enemyNum < kStage2EnemyNum)
		{
			m_timer++;
			if (m_timer == kTimer)
			{
				m_randomEnemy = GetRand(1);
				m_timer = 0;

				if (m_randomEnemy == kBee)
				{
					CreateBee(playerPos);
				}
				if (m_randomEnemy == kSlime)
				{
					CreateSlime(playerPos);
				}
			}
		}
	}
	else if (playerPos.x <= kStage3playerPosX)
	{
		if (m_enemyNum < kStage3EnemyNum)
		{
			m_timer++;
			if (m_timer == kTimer)
			{
				m_randomEnemy = GetRand(1);
				m_timer = 0;

				if (m_randomEnemy == kBee)
				{
					CreateBee(playerPos);
				}
				if (m_randomEnemy == kSlime)
				{
					CreateSlime(playerPos);
				}
			}
		}
	}
	else if (playerPos.x <= kStage4playerPosX)
	{
		if (m_enemyNum < kStage4EnemyNum)
		{
			m_timer++;
			if (m_timer == kTimer)
			{
				m_randomEnemy = GetRand(1);
				m_timer = 0;

				if (m_randomEnemy == kBee)
				{
					CreateBee(playerPos);
				}
				if (m_randomEnemy == kSlime)
				{
					CreateSlime(playerPos);
				}
			}
		}
	}
}

void EnemyManager::CreateBee(VECTOR playerPos)
{
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (!m_pBee[i])
		{
			m_pBee[i] = new Bee(m_beeHandle, playerPos);
			m_pBee[i]->Init();
			m_enemyNum++;

			return;
		}

	}
}

void EnemyManager::CreateSlime(VECTOR playerPos)
{
	for (int i = 0; i < kEnemyNum; i++)
	{
		if (!m_pSlime[i])
		{
			m_pSlime[i] = new Slime(m_slimeHandle, playerPos);
			m_pSlime[i]->Init();
			m_enemyNum++;

			return;
		}
	}
}

void EnemyManager::DrawEnemys()
{
	// HP の値を文字列とバーで表示
	if (!m_isClear1)
	{
		DrawFormatString(kRestEnemysX, kRestEnemysY, GetColor(255, 255, 255), "残り敵数 : %d", m_enemyNumRetention1);
	}
	else if (!m_isClear2)
	{
		DrawFormatString(kRestEnemysX, kRestEnemysY, GetColor(255, 255, 255), "残り敵数 : %d", m_enemyNumRetention2);
	}
	else if (!m_isClear3)
	{
		DrawFormatString(kRestEnemysX, kRestEnemysY, GetColor(255, 255, 255), "残り敵数 : %d", m_enemyNumRetention3);
	}
	else if (!m_isClear4)
	{
		DrawFormatString(kRestEnemysX, kRestEnemysY, GetColor(255, 255, 255), "残り敵数 : %d", m_enemyNumRetention4);
	}
	
}

void EnemyManager::StageClear(VECTOR playerPos)
{
	if (playerPos.x < kStage1playerPosX)
	{
		if (m_enemyNumRetention1 == 0)
		{
			m_isClear1 = true;

			m_isStageClear = m_isClear1;
		}
		else
		{
			m_isStageClear =false;
		}
	}
	else if (playerPos.x < kStage2playerPosX)
	{
		if (m_enemyNumRetention2 == 0)
		{
			m_isClear2 = true;

			m_isStageClear = m_isClear2;
		}
		else
		{
			m_isStageClear = false;
		}
	}
	else if (playerPos.x < kStage3playerPosX)
	{
		if (m_enemyNumRetention3 == 0)
		{
			m_isClear3 = true;
		}
		else
		{
			m_isStageClear = false;
		}
	}
	else if (playerPos.x < kStage4playerPosX)
	{
		if (m_enemyNumRetention4 == 0)
		{
			m_isStageClear = true;
		}
		else
		{
			m_isStageClear = false;
		}
	}
	
	
}



