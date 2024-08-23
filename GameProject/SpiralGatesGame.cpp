#include "pch.h"
#include "SpiralGatesGame.h"

SpiralGatesGame::SpiralGatesGame(int difficulty)
	:MiniGame(MiniGame::Type::SpiralGates, difficulty, 0, 1)
	, m_MaxNumGates{ 4 }
	, m_MinNumGates{ 2 }
	, m_NumUsedGates{}
	, m_Gates{ std::vector<Gate>(m_MaxNumGates) }
	, m_StartAngle{}
	, m_EndAngle{}
	, m_Loops{}
	, m_SpiralDir{}
	, m_SelectorAngle{}
	, m_SelectorRotSpeed{ 150.f }
{
	Init();
}

void SpiralGatesGame::Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const
{
	float spiralWidth{ (outerRad - innerRad) / (m_Loops + 1) * 0.9f };
	utils::SetColor(Color3f{ 0.31f, 0.216f, 0.055f });
	utils::FillSpiralBand(pos, outerRad, innerRad, utils::Radians(m_StartAngle), utils::Radians(m_EndAngle), spiralWidth, spiralWidth, 2);

	//startInnerRad = maxRad - startWidth;
	//startOuterRad = maxRad;
	//endInnerRad = minRad;
	//endOuterRad = minRad + endWidth;

	utils::SetColor(Color3f{ 0.741f, 0.537f, 0.204f });
	for (int i{}; i < m_NumUsedGates; ++i)
	{
		const Gate& gate{ m_Gates[i] };
		float gateStartPercOfSpiral{ std::abs(gate.startAngle - m_StartAngle) / std::abs(m_EndAngle - m_StartAngle) };
		float gateOuterRad{};
		float gateInnerRad{};
		utils::FillSpiralBand(pos, gateOuterRad, gateInnerRad, utils::Radians(gate.startAngle), utils::Radians(gate.endAngle), spiralWidth, spiralWidth, 2);
	}
}

void SpiralGatesGame::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
}

void SpiralGatesGame::Click(GameData::Feedback& feedback)
{
}

void SpiralGatesGame::Init(bool activate)
{
	m_NumUsedGates = rand() % (m_MaxNumGates - m_MinNumGates) + m_MinNumGates;
	m_StartAngle = float(rand() % 360);
	m_SpiralDir = rand() % 2 == 0 ? 1 : -1;

	int minAngleBetweenGates{60};
	int maxAngleBetweenGates{180};

	float gateArcLength{ 20.f };

	float angle{ m_StartAngle };
	for (int i{}; i < m_NumUsedGates; ++i)
	{
		Gate& gate{ m_Gates[i] };
		float distToNextGate{ float(rand() % (maxAngleBetweenGates - minAngleBetweenGates) + minAngleBetweenGates) };
		angle += m_SpiralDir * distToNextGate;
		gate.startAngle = angle;
		angle += m_SpiralDir * gateArcLength;
		gate.endAngle = angle;
		gate.isOpen = false;
		gate.closeAfterOpen = false;
		gate.bounceBackWhenClosed = false;
	}
	angle += m_SpiralDir * 30.f;
	m_EndAngle = angle;

	m_Loops = std::abs(int((m_EndAngle - m_StartAngle) / 360.f));

	m_SelectorAngle = m_StartAngle;
}
