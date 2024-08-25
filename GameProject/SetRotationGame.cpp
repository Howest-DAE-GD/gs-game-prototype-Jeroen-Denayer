#include "pch.h"
#include "SetRotationGame.h"
#include "BallManager.h"
#include "Spiral.h"

SetRotationGame::SetRotationGame(int difficulty, const DrawData& drawData)
	:MiniGame(MiniGame::Type::SetRotation, difficulty, 2, drawData)
	, m_ValidAngle{  }
	, m_SelectorAngle{ float(rand() % 360) }
	, m_SelectorRotSpeed{ 180.f }
	, m_Config{}
{
	Init(m_Difficulty);
}

void SetRotationGame::Draw(Point2f pos) const
{
	float startAngle{ m_ValidAngle - m_Config.validAngleDeviation };
	float endAngle{ m_ValidAngle + m_Config.validAngleDeviation };

	//Draw the valid angle region
	utils::SetColor(Color3f{ 0.212f, 0.388f, 0.149f });
	Spiral::DrawFilledArc(pos, m_DrawData.innerRad, m_DrawData.outerRad, utils::Radians(startAngle), utils::Radians(endAngle));

	//Draw 2 lines to indicate the valid angle region
	utils::SetColor(Color3f{ 0.067f, 0.18f, 0.027f });
	utils::DrawRadialLine(pos, m_DrawData.innerRad, m_DrawData.outerRad, startAngle);
	utils::DrawRadialLine(pos, m_DrawData.innerRad, m_DrawData.outerRad, endAngle);

	//Draw the player controlled line
	utils::SetColor(Color3f{ 1.f, 0.f, 0.f });
	utils::DrawRadialLine(pos, m_DrawData.innerRad, m_DrawData.outerRad, m_SelectorAngle, 3.f);
}

void SetRotationGame::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
	if (m_State == State::Idle)
		return;

	if (input.pressedSpace)
    	Click(feedback);
	if (input.pressedLeft)
	{
		m_SelectorAngle += m_SelectorRotSpeed * dt;
		if (m_SelectorAngle > 360)
			m_SelectorAngle = 0.f;
	}
	if (input.pressedRight)
	{
		m_SelectorAngle -= m_SelectorRotSpeed * dt;
		if (m_SelectorAngle < 0.f)
			m_SelectorAngle = 360.f;
	}
}

void SetRotationGame::Click(GameData::Feedback& feedback)
{
	if (utils::SmallestAngleBetween2Angles(m_SelectorAngle, m_ValidAngle) < m_Config.validAngleDeviation)
	{
		++m_Points;
		m_State = State::Completed;
		return;
	}
	else
	{
		feedback.failedMiniGame = true;
		m_State = State::Failed;
	}
}

void SetRotationGame::Init(int difficulty, bool activate)
{
	ConfigureDifficulty(difficulty);

	if (activate)
		Activate();

	//Set the new angle to be min 60 degrees from the current selector pos.
	float minAngleDiff{ 60.f };
	int sign{ rand() % 2 == 0 ? 1 : -1 };
	m_ValidAngle = utils::NormalizeAngle(m_SelectorAngle + sign * (rand() % int(180 - minAngleDiff) + minAngleDiff));

	CalculateTimeToComplete();
}

void SetRotationGame::ConfigureDifficulty(int difficulty)
{
	if (difficulty == 0)
	{
		m_Config.validAngleDeviation = 30.f;
	}
	else if (difficulty == 1)
	{
		m_Config.validAngleDeviation = 20.f;
	}
	else if (difficulty == 2)
	{
		m_Config.validAngleDeviation = 10.f;
	}
}

void SetRotationGame::CalculateTimeToComplete()
{
	float distToTravel{ std::abs(m_ValidAngle - m_SelectorAngle) };
	float timeToTravel{ distToTravel / m_SelectorRotSpeed };
	float multiplier{ m_MaxDifficulty - m_Difficulty + 1.25f };
	m_MaxTimeToComplete = multiplier * timeToTravel;
}
