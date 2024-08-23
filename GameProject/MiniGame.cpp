#include "pch.h"
#include "MiniGame.h"

MiniGame::MiniGame(Type type, int difficulty, int maxDifficulty)
	: m_Type{ type }
	, m_State{ State::Idle }
	, m_MaxDifficulty{ maxDifficulty }
	, m_Difficulty{ std::min(difficulty, m_MaxDifficulty) }
	, m_Points{ 0 }
	, m_MaxTimeToComplete{ 5.f }
{
}

void MiniGame::Activate()
{
	if (m_State == State::Idle)
		m_State = State::Active;
}

MiniGame::State MiniGame::GetState() const
{
	return m_State;
}

float MiniGame::GetTimeToComplete() const
{
	return m_MaxTimeToComplete;
}
