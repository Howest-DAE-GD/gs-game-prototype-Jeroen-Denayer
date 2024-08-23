#include "pch.h"
#include "SelectColorGame.h"
#include "BallManager.h"
#include <unordered_set>
#include "Spiral.h"

const std::vector<Color3f> SelectColorGame::s_Colors{
	Color3f{0.094f, 0.094f, 0.812f}, //dark blue
	Color3f{0.137f, 0.522f, 0.89f}, //light blue
	Color3f{0.f, 0.392f, 0.f}, //dark green
	Color3f{0.192f, 0.722f, 0.192f}, //light green
	Color3f{0.812f, 0.718f, 0.2f}, //bright yellow
	Color3f{0.859f, 0.408f, 0.141f},  //orange 
	Color3f{0.78f, 0.082f, 0.522f}, //pink
};

const std::vector<float> SelectColorGame::s_AngleDeviationPerDifficulty{ 30.f, 20.f, 10.f };
const std::vector<float> SelectColorGame::s_SelectorRotSpeedPerDifficulty{ 180.f, 180.f, 210.f };

SelectColorGame::SelectColorGame(int difficulty)
	:MiniGame(MiniGame::Type::SelectColor, difficulty, 2)
	, m_ValidColorIdx{}
	, m_MaxNumColorRegions{ 4 }
	, m_MinNumColorRegions{ 2 }
	, m_NumUsedColorRegions{}
	, m_ColorRegions{ std::vector<ColorRegion>(m_MaxNumColorRegions) }
	, m_SelectorAngle{ float(rand() % 360) }
	, m_SelectorRotSpeed{}
	, m_SelectorRotDir{ rand() % 2 == 0 ? 1 : -1 }
{
	Init();
}

void SelectColorGame::Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const
{
	//Draw color regions
	for (int i{}; i < m_NumUsedColorRegions; ++i)
	{
		const ColorRegion& colorRegion{ m_ColorRegions[i] };
		float fromAngle{ colorRegion.angle - colorRegion.angleDeviation };
		float tillAngle{ colorRegion.angle + colorRegion.angleDeviation };
		utils::SetColor(s_Colors[colorRegion.colorIdx]);
		Spiral::DrawFilledArc(pos, innerRad, outerRad, utils::Radians(fromAngle), utils::Radians(tillAngle));
	}

	//Draw circle with the correct color that needs to be matched
	utils::SetColor(s_Colors[m_ValidColorIdx]);
	utils::FillEllipse(pos, centerRadius, centerRadius);

	//Draw selector
	Point2f p0{ pos.x + std::cosf(utils::Radians(m_SelectorAngle)) * innerRad, pos.y + std::sinf(utils::Radians(m_SelectorAngle)) * innerRad };
	Point2f p1{ pos.x + std::cosf(utils::Radians(m_SelectorAngle)) * outerRad, pos.y + std::sinf(utils::Radians(m_SelectorAngle)) * outerRad };
	utils::SetColor(Color3f{ 1.f, 0.f, 0.f });
	utils::DrawLine(p0, p1, 3.f);
}

void SelectColorGame::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
	if (m_State == State::Idle)
		return;

	if (input.pressedSpace)
		Click(feedback);

	m_SelectorAngle += m_SelectorRotSpeed * dt;
	if (m_SelectorRotSpeed > 0.f && m_SelectorAngle > 360.f)
		m_SelectorAngle = 0.f;
	else if (m_SelectorAngle < 0.f)
		m_SelectorAngle = 360.f;
}

void SelectColorGame::Click(GameData::Feedback& feedback)
{
	bool hitRegion{ false };
	for (int i{}; i < m_NumUsedColorRegions; ++i)
	{
		ColorRegion& colorRegion{ m_ColorRegions[i] };
		if (utils::SmallestAngleBetween2Angles(colorRegion.angle, m_SelectorAngle, true) < colorRegion.angleDeviation &&
			colorRegion.colorIdx == m_ValidColorIdx)
		{
			colorRegion.activated = true;
			hitRegion = true;
			break;
		}
	}

	if (hitRegion)
	{
		++m_Points;
		feedback.totalPoints = m_Points;
		m_State = State::Completed;
		return;
	}
	else
	{
		feedback.lostLife = true;
		m_State = State::Failed;
	}
}

void SelectColorGame::Init(bool activate)
{
	std::unordered_set<int> colorIndicesUsed{};
	std::unordered_set<int> usedAngles{};

	m_SelectorRotSpeed = m_SelectorRotDir * s_SelectorRotSpeedPerDifficulty[m_Difficulty];

	m_NumUsedColorRegions = rand() % (m_MaxNumColorRegions - m_MinNumColorRegions + 1) + m_MinNumColorRegions;
	float angleOffset{ float(rand() % 360) };
	float angleStep{ 360.f / m_MaxNumColorRegions };
	for (int i{}; i < m_NumUsedColorRegions; ++i)
	{
		int colorIdx{};
		do
		{
			colorIdx = rand() % s_Colors.size();
		} while (colorIndicesUsed.find(colorIdx) != colorIndicesUsed.end());
		colorIndicesUsed.insert(colorIdx);

		int angleIdx{};
		do
		{
			angleIdx = rand() % m_MaxNumColorRegions;
		} while (usedAngles.find(angleIdx) != usedAngles.end());
		usedAngles.insert(angleIdx);

		m_ColorRegions[i].activated = false;
		m_ColorRegions[i].colorIdx = colorIdx;
		m_ColorRegions[i].angle = angleIdx * angleStep + angleOffset;
		m_ColorRegions[i].angleDeviation = s_AngleDeviationPerDifficulty[m_Difficulty];
	}

	m_ValidColorIdx = m_ColorRegions[rand() % m_NumUsedColorRegions].colorIdx;
}

void SelectColorGame::ConfigureDifficulty(int difficulty)
{
}

void SelectColorGame::CalculateTimeToComplete()
{
}
