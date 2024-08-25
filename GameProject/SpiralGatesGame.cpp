#include "pch.h"
#include "SpiralGatesGame.h"

SpiralGatesGame::SpiralGatesGame(int difficulty, const DrawData& drawData)
	:MiniGame(MiniGame::Type::SpiralGates, difficulty, 2, drawData)
	, m_MaxNumGates{ 4 }
	, m_NumUsedGates{}
	, m_Gates{ std::vector<Gate>(m_MaxNumGates) }
	, m_StartAngle{}
	, m_EndAngle{}
	, m_Loops{}
	, m_SpiralDir{}
	, m_SelectorAngle{}
	, m_SelectorNextGateIdx{}
	, m_Config{}
{
	Init(m_Difficulty);
}

void SpiralGatesGame::Draw(Point2f pos) const
{
	//Draw main spiral
	float spiralWidth{ (m_DrawData.outerRad - m_DrawData.innerRad) / (m_Loops + 1) * 0.9f };
	utils::SetColor(Color3f{ 0.31f, 0.216f, 0.055f });
	Spiral::DrawInfo drawInfo{ pos, m_DrawData.innerRad, m_DrawData.outerRad, utils::Radians(m_StartAngle), utils::Radians(m_EndAngle), spiralWidth, spiralWidth, Spiral::DrawMode::extrema };
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

	//Draw start
	utils::SetColor(Color3f{ 0.f, 1.f, 0.f });
	Spiral::DrawPartiallyFilledSpiral(drawInfo, utils::Radians(m_StartAngle), utils::Radians(m_StartAngle + m_SpiralDir * 10.f));

	//Draw finish
	utils::SetColor(Color3f{ 0.7f, 0.f, 0.f });
	Spiral::DrawPartiallyFilledSpiral(drawInfo, utils::Radians(m_EndAngle - m_SpiralDir * 10.f), utils::Radians(m_EndAngle));

	//Draw the selector
	utils::SetColor(Color3f{ 1.f, 0.f, 0.f });
	Spiral::DrawLineOnSpiral(drawInfo, m_SelectorAngle, 3.f);
}

void SpiralGatesGame::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
	if (input.pressedSpace)
		Click(feedback);

	m_SelectorAngle += m_SpiralDir * m_Config.selectorRotSpeed * dt;

	if (m_SelectorNextGateIdx < m_NumUsedGates)
	{
		const Gate& nextGate{ m_Gates[m_SelectorNextGateIdx] };
		if (IsSelectorPastAngle(nextGate.endAngle) && !nextGate.isOpen)
			m_SelectorAngle = m_StartAngle;
	}
	else if (IsSelectorPastAngle(m_EndAngle)) //finished
	{
		m_Points += m_NumUsedGates;
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
		gate.isOpen = !gate.isOpen;
		++m_SelectorNextGateIdx;
	}
	else //before gate, miss click
	{
		feedback.failedMiniGame = true;
		m_State = State::Failed;
	}
}

void SpiralGatesGame::Init(int difficulty, bool activate)
{
	ConfigureDifficulty(difficulty);

	if (activate)
		Activate();

	m_NumUsedGates = rand() % (m_MaxNumGates - m_Config.minNumGates + 1) + m_Config.minNumGates;
	m_StartAngle = float(rand() % 360);
	m_SpiralDir = rand() % 2 == 0 ? 1 : -1;

	float distToNextGate{ m_Config.startEmptyArcLength };
	float angle{ m_StartAngle};
	for (int i{}; i < m_NumUsedGates; ++i)
	{
		angle += m_SpiralDir * distToNextGate;
		Gate& gate{ m_Gates[i] };
		gate.startAngle = angle;
		angle += m_SpiralDir * m_Config.gateArcLength;
		gate.endAngle = angle;
		gate.isOpen = false;
		distToNextGate = rand() % int(m_Config.maxAngleBetweenGates - m_Config.minAngleBetweenGates) + m_Config.minAngleBetweenGates;
	}
	angle += m_SpiralDir * m_Config.finishArcLength;
	m_EndAngle = angle;

	m_Loops = std::abs((m_EndAngle - m_StartAngle) / 360.f);

	m_SelectorAngle = m_StartAngle;
	m_SelectorNextGateIdx = 0;

	CalculateTimeToComplete();
}

void SpiralGatesGame::ConfigureDifficulty(int difficulty)
{
	if (difficulty == 0)
	{
		m_Config.minNumGates = 2;
		m_Config.selectorRotSpeed = 60.f;
		m_Config.startEmptyArcLength = 120.f;
		m_Config.finishArcLength = 30.f;
		m_Config.gateArcLength = 40.f;
		m_Config.minAngleBetweenGates = 60.f;
		m_Config.maxAngleBetweenGates = 120.f;
	}
	else if (difficulty == 1)
	{
		m_Config.minNumGates = 2;
		m_Config.selectorRotSpeed = 90.f;
		m_Config.startEmptyArcLength = 90.f;
		m_Config.finishArcLength = 30.f;
		m_Config.gateArcLength = 30.f;
		m_Config.minAngleBetweenGates = 40.f;
		m_Config.maxAngleBetweenGates = 120.f;
	}
	else if (difficulty == 2)
	{
		m_Config.minNumGates = 3;
		m_Config.selectorRotSpeed = 120.f;
		m_Config.startEmptyArcLength = 60.f;
		m_Config.finishArcLength = 30.f;
		m_Config.gateArcLength = 30.f;
		m_Config.minAngleBetweenGates = 20.f;
		m_Config.maxAngleBetweenGates = 120.f;
	}

	m_Config.minNumGates = std::min(m_Config.minNumGates, m_MaxNumGates);
}

void SpiralGatesGame::CalculateTimeToComplete()
{
	float spiralArcLength{ std::abs(m_EndAngle - m_StartAngle) };
	float timeToTravelSpiral{ spiralArcLength / m_Config.selectorRotSpeed };
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
