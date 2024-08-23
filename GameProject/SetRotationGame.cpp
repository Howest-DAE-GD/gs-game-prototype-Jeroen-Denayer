#include "pch.h"
#include "SetRotationGame.h"
#include "BallManager.h"

const std::vector<float> SetRotationGame::s_AngleDeviationPerDifficulty{ 30.f, 20.f, 10.f };

SetRotationGame::SetRotationGame(int difficulty)
	:MiniGame(MiniGame::Type::SetRotation, difficulty, 2, 3)
	, m_ValidAngle{}
	, m_ValidAngleDeviation{}
	, m_SelectorAngle{}
	, m_SelectorRotSpeed{ 180.f }
{
	Init();
}

void SetRotationGame::Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const
{
	float fromAngle{ m_ValidAngle - m_ValidAngleDeviation };
	float tillAngle{ m_ValidAngle + m_ValidAngleDeviation };

	//Draw the valid angle region
	utils::SetColor(Color3f{ 0.212f, 0.388f, 0.149f });
	utils::FillArcBand(pos, innerRad, outerRad, utils::Radians(fromAngle), utils::Radians(tillAngle));

	//Draw 2 lines to indicate the valid angle region
	float linePercOfRad{ 0.5f };
	Point2f p0{ pos.x + std::cosf(utils::Radians(fromAngle)) * innerRad	, pos.y + std::sinf(utils::Radians(fromAngle)) * innerRad };
	Point2f p1{ pos.x + std::cosf(utils::Radians(fromAngle)) * outerRad	, pos.y + std::sinf(utils::Radians(fromAngle)) * outerRad };
	Point2f p2{ pos.x + std::cosf(utils::Radians(tillAngle)) * innerRad	, pos.y + std::sinf(utils::Radians(tillAngle)) * innerRad };
	Point2f p3{ pos.x + std::cosf(utils::Radians(tillAngle)) * outerRad	, pos.y + std::sinf(utils::Radians(tillAngle)) * outerRad };
	utils::SetColor(Color3f{ 1.f, 1.f, 1.f });
	utils::DrawLine(p0, p1, 2.f);
	utils::DrawLine(p2, p3, 2.f);

	//Draw the player controlled line
	Point2f lineStart{ pos.x + std::cosf(utils::Radians(m_SelectorAngle)) * innerRad, pos.y + std::sinf(utils::Radians(m_SelectorAngle)) * innerRad };
	Point2f lineEnd{ pos.x + std::cosf(utils::Radians(m_SelectorAngle)) * outerRad, pos.y + std::sinf(utils::Radians(m_SelectorAngle)) * outerRad };
	utils::SetColor(Color3f{ 1.f, 0.f, 0.f });
	utils::DrawLine(lineStart, lineEnd, 2.f);
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
	if (utils::SmallestAngleBetween2Angles(m_SelectorAngle, m_ValidAngle) < m_ValidAngleDeviation)
	{
		++m_Points;
		if (m_NumPlaythroughs != m_NumPlaythroughsToComplete)
			Init();
		else
		{
			feedback.totalPoints = m_Points;
			m_State = State::Completed;
			return;
		}
	}
	else
	{
		feedback.lostLife = true;
		m_State = State::Failed;
	}
}

void SetRotationGame::Init(bool activate)
{
	if (m_NumPlaythroughs == 0)
	{
		m_ValidAngle = float(rand() % 360);
		//Set m_SelectorAngle so it never starts in the correct angle
		float minAngleDeviation{ 30.f };
		int sign{ rand() % 2 == 0 ? 1 : -1 };
		m_SelectorAngle = utils::NormalizeAngle(m_ValidAngle + sign * (rand() % int(180 - minAngleDeviation) + minAngleDeviation));
	}
	else
	{
		//Set the new angle to be min 60 degrees from the current selector pos.
		float minAngleDiff{ 60.f };
		int sign{ rand() % 2 == 0 ? 1 : -1 };
		m_ValidAngle = utils::NormalizeAngle(m_ValidAngle + sign * (rand() % int(180 - minAngleDiff) + minAngleDiff));
	}

	m_ValidAngleDeviation = s_AngleDeviationPerDifficulty[m_Difficulty];

	++m_NumPlaythroughs;
}
