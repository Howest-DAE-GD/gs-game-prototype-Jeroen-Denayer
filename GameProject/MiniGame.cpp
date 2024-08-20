#include "pch.h"
#include "MiniGame.h"

MiniGame::MiniGame(Type type, int numPlaythroughsToComplete)
	: m_Type{ type }
	, m_State{ State::Idle }
	, m_Difficulty{ 1 }
	, m_Points{ 0 }
	, m_NumPlaythroughs{ 0 }
	, m_NumPlaythroughsToComplete{ numPlaythroughsToComplete }
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

int MiniGame::GetPoints() const
{
	return m_Points;
}
