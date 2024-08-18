#include "pch.h"
#include "MiniGame.h"

MiniGame::MiniGame(Type type)
	: m_Type{ type }
	, m_State{ State::Idle }
	, m_Points{ 1 }
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
