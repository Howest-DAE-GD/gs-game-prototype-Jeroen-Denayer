#include "pch.h"
#include "SpiralGatesGame.h"

float SpiralGatesGame::s_FinishArcLength{ 30.f };
std::vector<float> SpiralGatesGame::s_GateArcLength{ 40.f, 30.f, 20.f };
std::vector<int> SpiralGatesGame::s_StartEmptyArcLength{ 120, 90, 60 };
std::vector<int> SpiralGatesGame::s_MinAngleBetweenGates{ 60, 40, 20 };
std::vector<int> SpiralGatesGame::s_MaxAngleBetweenGates{ 120, 120, 120 };

SpiralGatesGame::SpiralGatesGame(int difficulty)
	:MiniGame(MiniGame::Type::SpiralGates, difficulty, 2)
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
	, m_SelectorNextGateIdx{}
	//, m_StartEmptyArcLength{}
	//, m_FinishArcLength{}
	//, m_GateArcLength{}
	//, m_MinAngleBetweenGates{}
	//, m_MaxAngleBetweenGates{}
{
	Init();
}

void SpiralGatesGame::Draw(Point2f pos, float innerRad, float outerRad, float centerRadius) const
{
	//Draw main spiral
	float spiralWidth{ (outerRad - innerRad) / (m_Loops + 1) * 0.9f };
	utils::SetColor(Color3f{ 0.31f, 0.216f, 0.055f });
	Spiral::DrawInfo drawInfo{ pos, innerRad, outerRad, utils::Radians(m_StartAngle), utils::Radians(m_EndAngle), spiralWidth, spiralWidth, Spiral::DrawMode::extrema };
	Spiral::DrawFilledSpiral(drawInfo);

	//Draw gates
	for (int i{}; i < m_NumUsedGates; ++i)
	{
		utils::SetColor(Color3f{ 0.741f, 0.537f, 0.204f });
		const Gate& gate{ m_Gates[i] };
		Spiral::DrawPartiallyFilledSpiral(drawInfo, utils::Radians(gate.startAngle), utils::Radians(gate.endAngle));
		Spiral::SpiralAngleInfo angleInfo{ Spiral::GetSpiralAngleInfo(drawInfo, utils::Radians(gate.endAngle)) };
		if (!gate.isOpen)
		{
			utils::SetColor(Color3f{ 1.f, 1.f, 1.f });
			Spiral::DrawLineOnSpiral(drawInfo, gate.endAngle, 2.f);
		}
	}

	//Draw finish
	utils::SetColor(Color3f{ 0.f, 1.f, 0.f });
	Spiral::DrawLineOnSpiral(drawInfo, m_EndAngle, 5.f);

	//Draw the selector
	utils::SetColor(Color3f{ 1.f, 0.f, 0.f });
	Spiral::DrawLineOnSpiral(drawInfo, m_SelectorAngle, 3.f);
}

void SpiralGatesGame::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
	if (input.pressedSpace)
		Click(feedback);

	m_SelectorAngle += m_SpiralDir * m_SelectorRotSpeed * dt;

	if (m_SelectorNextGateIdx < m_NumUsedGates)
	{
		const Gate& nextGate{ m_Gates[m_SelectorNextGateIdx] };
		if (IsSelectorPastAngle(nextGate.endAngle) && !nextGate.isOpen)
			m_SelectorAngle = m_StartAngle;
	}
	else if (IsSelectorPastAngle(m_EndAngle)) //finished
	{
		m_Points += m_NumUsedGates;
		feedback.totalPoints = m_Points;
		m_State = State::Completed;
		return;
	}
}

void SpiralGatesGame::Click(GameData::Feedback& feedback)
{
	//check if the selector overlaps with the next gate
	Gate& gate{ m_Gates[m_SelectorNextGateIdx] };
	if (IsSelectorPastAngle(gate.endAngle)) //passed gate
	{
		m_SelectorAngle = m_StartAngle;
	}
	else if (IsSelectorPastAngle(gate.startAngle)) //inside gate
	{
		gate.isOpen = true;
		++m_SelectorNextGateIdx;
	}
	else //before gate, miss click
	{
		m_SelectorAngle = m_StartAngle;
		feedback.lostLife = true;
	}
}

void SpiralGatesGame::Init(bool activate)
{
	m_NumUsedGates = rand() % (m_MaxNumGates - m_MinNumGates + 1) + m_MinNumGates;
	m_StartAngle = float(rand() % 360);
	m_SpiralDir = rand() % 2 == 0 ? 1 : -1;

	int minAngleBetweenGates{s_MinAngleBetweenGates[m_Difficulty]};
	int maxAngleBetweenGates{ s_MaxAngleBetweenGates[m_Difficulty] };

	float distToNextGate{ float(s_StartEmptyArcLength[m_Difficulty]) };
	float angle{ m_StartAngle};
	for (int i{}; i < m_NumUsedGates; ++i)
	{
		angle += m_SpiralDir * distToNextGate;
		Gate& gate{ m_Gates[i] };
		gate.startAngle = angle;
		angle += m_SpiralDir * s_GateArcLength[m_Difficulty];
		gate.endAngle = angle;
		gate.isOpen = false;
		distToNextGate = float(rand() % (maxAngleBetweenGates - minAngleBetweenGates) + minAngleBetweenGates);
	}
	angle += m_SpiralDir * s_FinishArcLength;
	m_EndAngle = angle;

	m_Loops = std::abs((m_EndAngle - m_StartAngle) / 360.f);

	m_SelectorAngle = m_StartAngle;
	m_SelectorNextGateIdx = 0;

	CalculateTimeToComplete();
}

void SpiralGatesGame::ConfigureDifficulty(int difficulty)
{
}

void SpiralGatesGame::CalculateTimeToComplete()
{
	float spiralArcLength{ std::abs(m_EndAngle - m_StartAngle) };
	float timeToTravelSpiral{ spiralArcLength / m_SelectorRotSpeed };

	float multiplier{ m_MaxDifficulty - m_Difficulty + 1.f };

	m_MaxTimeToComplete = multiplier * timeToTravelSpiral;
}

bool SpiralGatesGame::IsSelectorPastAngle(float angle) const
{
	return m_SpiralDir == 1 && m_SelectorAngle > angle || m_SpiralDir == -1 && m_SelectorAngle < angle;
}

bool SpiralGatesGame::SelectorOverlapsGate(const Gate& gate) const
{
	return IsSelectorPastAngle(gate.startAngle) && !IsSelectorPastAngle(gate.endAngle);
}
