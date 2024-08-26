#include "pch.h"
#include "LineScannerGame.h"

LineScannerGame::LineScannerGame(int difficulty, const DrawData& drawData)
	:MiniGame(MiniGame::Type::LineScanner, difficulty, 0, drawData)
	, m_SelectorDir{ 1 }
	, m_MaxSelectorPos{ 0.95f }
	, m_SelectorPos{ 0.f }
{
	Init(m_Difficulty);
}

void LineScannerGame::Draw(const Point2f& pos) const
{
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	Point2f center{ 0.f, 0.f };

	//Draw Selector
	utils::SetColor(Color3f{ 1.f, 0.f, 0.f });
	utils::DrawVerticalLineInCircle(center, m_DrawData.outerRad, m_SelectorPos, 3.f);

	//Draw end lines
	utils::SetColor(Color3f{ 1.f, 1.f, 1.f });
	utils::DrawVerticalLineInCircle(center, m_DrawData.outerRad, m_MaxSelectorPos, 2.f);
	utils::DrawVerticalLineInCircle(center, m_DrawData.outerRad, -m_MaxSelectorPos, 2.f);

	glPopMatrix();
}

void LineScannerGame::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
	//if (m_State == State::Idle)
	//	return;

	if (input.pressedSpace)
		Click(feedback);

	m_SelectorPos += m_SelectorDir * (m_Config.selectorSpeed / m_DrawData.outerRad) * dt;
	if (std::abs(m_SelectorPos) > m_MaxSelectorPos)
	{
		m_SelectorPos = utils::ClampValueBetweenOthers(m_SelectorPos, -m_MaxSelectorPos, m_MaxSelectorPos);
		m_SelectorDir *= -1;
	}
}

void LineScannerGame::Init(int difficulty, bool activate)
{
	ConfigureDifficulty(difficulty);



	CalculateTimeToComplete();
}

void LineScannerGame::Click(GameData::Feedback& feedback)
{
}

void LineScannerGame::ConfigureDifficulty(int difficulty)
{
	if (difficulty == 0)
	{
		m_Config.selectorSpeed = 100.f;
	}
}

void LineScannerGame::CalculateTimeToComplete()
{
	m_MaxTimeToComplete = 20.f;
}
