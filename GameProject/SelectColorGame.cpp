#include "pch.h"
#include "SelectColorGame.h"
#include <unordered_set>

const std::vector<Color3f> SelectColorGame::s_Colors{
	Color3f{0.f, 0.392f, 0.f},
	Color3f{1.f, 0.361f, 0.f}, 
	Color3f{1.f, 0.843f, 0.f},
	Color3f{0.78f, 0.082f, 0.522f},
	Color3f{0.f, 1.f, 0.f},
	Color3f{0.f, 1.f, 1.f},
	Color3f{0.f, 0.f, 1.f}
};

SelectColorGame::SelectColorGame()
	:MiniGame(MiniGame::Type::SelectColor, 3)
	, m_ValidColorIdx{}
	, m_MaxNumColorRegions{ 4 }
	, m_MinNumColorRegions{ 2 }
	, m_NumUsedColorRegions{}
	, m_ColorRegions{ std::vector<ColorRegion>(m_MaxNumColorRegions) }
	, m_SelectorAngle{ float(rand() % 360) }
	, m_SelectorRotSpeed{ 180.f * (rand() % 2 == 0 ? 1 : -1) }
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
		utils::FillArcBand(pos, innerRad, outerRad, utils::Radians(fromAngle), utils::Radians(tillAngle));
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

void SelectColorGame::Update(float dt, bool pressedLeft, bool pressedRight)
{
	if (m_State == State::Idle)
		return;

	m_SelectorAngle += m_SelectorRotSpeed * dt;
}

void SelectColorGame::Click()
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
		Init();
	}
	else
		m_State = State::Failed;
}

void SelectColorGame::Init(bool activate)
{
	if (m_NumPlaythroughs == m_NumPlaythroughsToComplete)
	{
		m_State = State::Completed;
		return;
	}

	std::unordered_set<int> colorIndicesUsed{};
	std::unordered_set<int> usedAngles{};

	m_NumUsedColorRegions = rand() % (m_MaxNumColorRegions - m_MinNumColorRegions) + m_MinNumColorRegions;
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
		m_ColorRegions[i].angleDeviation = float(rand() % 21 + 10);
	}

	m_ValidColorIdx = m_ColorRegions[rand() % m_NumUsedColorRegions].colorIdx;

	++m_NumPlaythroughs;
}
