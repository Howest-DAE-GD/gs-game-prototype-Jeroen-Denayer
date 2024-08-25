#include "pch.h"
#include "WallDodgeGame.h"
#include "Spiral.h"

WallDodgeGame::WallDodgeGame(int difficulty, const DrawData& drawData)
	:MiniGame(MiniGame::Type::WallDodge, difficulty, 0, drawData)
	, m_Dir{}
	, m_StartOnInside{}
	, m_BallOnInside{}
	, m_BallAngle{}
	, m_BallFrontAngle{}
	, m_BallBackAngle{}
	, m_BallSizePercOfLane{ 0.9f }
	, m_AreaStartAngle{}
	, m_AreaEndAngle{}
	, m_StartAngle{}
	, m_FinishAngle{}
	, m_AreaGapArcLength{ 45.f }
	, m_MinNumWalls{ 3 }
	, m_MaxNumWalls{ 9 }
	, m_NumWallsPerPoint{ 3 }
	, m_NextWallIdx{}
	, m_Walls{}
	, m_Config{}
{
	Init(m_Difficulty);
}

void WallDodgeGame::Draw(Point2f pos) const
{
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0.f);
	Point2f centerPos{0.f, 0.f};

	float laneWidth{ (m_DrawData.outerRad - m_DrawData.innerRad) / 2.f };
	float laneHalfWidth{ laneWidth / 2.f };
	float wallHeight{ laneWidth * 0.8f };
	float outsideWallInnerRad{ m_DrawData.outerRad - wallHeight };
	float insideWallOuterRad{ m_DrawData.innerRad + wallHeight };

	//Draw area
	utils::SetColor(Color3f{ 0.227f, 0.341f, 0.459f });
	Spiral::DrawFilledArc(centerPos, m_DrawData.innerRad, m_DrawData.outerRad, utils::Radians(m_AreaStartAngle), utils::Radians(m_AreaEndAngle));

	//Draw walls
	utils::SetColor(Color3f{ 0.075f, 0.122f, 0.31f });
	for (int i{}; i < m_Walls.size(); ++i)
	{
		const Wall& wall{ m_Walls[i] };
		//check if the start or the end is in the area
		if (!utils::IsAngleBetween(wall.startAngle, m_AreaStartAngle, m_AreaEndAngle) &&
			!utils::IsAngleBetween(wall.endAngle, m_AreaStartAngle, m_AreaEndAngle))
			continue;

		//flip wall from inside to outside
		float wallInnerRad{ wall.onInside ? m_DrawData.innerRad : outsideWallInnerRad };
		float wallOuterRad{ wall.onInside ? insideWallOuterRad : m_DrawData.outerRad };
		float startAngle{ std::min(m_AreaEndAngle, std::max(wall.startAngle, m_AreaStartAngle)) };
		float endAngle{ std::min(m_AreaEndAngle, std::max(wall.endAngle, m_AreaStartAngle)) };
		Spiral::DrawFilledArc(centerPos, wallInnerRad, wallOuterRad, utils::Radians(startAngle), utils::Radians(endAngle));
	}

	//Draw ball
	float ballRad{ laneWidth / 2.f * m_BallSizePercOfLane };
	float ballCenterRad{ m_BallOnInside ? m_DrawData.innerRad + laneHalfWidth : m_DrawData.outerRad - laneHalfWidth };
	Point2f ballPos{ utils::GetPointOnCircle(Point2f{0.f, 0.f}, ballCenterRad, m_BallAngle) };
	utils::SetColor(Color3f{ 1.f, 1.f, 0.f });
	utils::FillEllipse(ballPos, ballRad, ballRad);

	glPopMatrix();
}

void WallDodgeGame::Update(float dt, const GameData::Input& input, GameData::Feedback& feedback)
{
	if (m_State == State::Idle)
		return;

	if (input.pressedSpace)
		Click(feedback);

	float ballDeltaAngle{ m_Config.ballRotSpeed * dt };
	UpdateBallAngle(ballDeltaAngle);
	if (m_AreaEndAngle < m_FinishAngle)
	{
		m_AreaEndAngle += ballDeltaAngle;
		if (m_AreaStartAngle - m_AreaEndAngle + 360.f < m_AreaGapArcLength)
			m_AreaStartAngle += ballDeltaAngle;
	}

	//Only check wall overlaps if the ball isn ot past the last wall already
	if (m_NextWallIdx < m_Walls.size())
	{
		const Wall& nextWall{ m_Walls[m_NextWallIdx] };
		if (IsBallOverlappingWall(m_NextWallIdx))
		{
			feedback.failedMiniGame = true;
			m_State = State::Failed;
		}

		if (m_BallBackAngle > nextWall.endAngle)
			++m_NextWallIdx;
	}
	else if (m_BallFrontAngle > m_FinishAngle)
	{
		m_Points += int(m_Walls.size() / m_NumWallsPerPoint) + 1;
		m_State = State::Completed;
	}
}

void WallDodgeGame::Init(int difficulty, bool activate)
{
	ConfigureDifficulty(difficulty);

	int m_Dir{ rand() % 2 == 0 };
	m_StartOnInside = rand() % 2 == 0;
	m_BallOnInside = m_StartOnInside;

	m_BallAngle = float(rand() % 360);
	UpdateBallAngle(0); //Sets m_BallFrontAngle and m_BallBackAngle;

	m_StartAngle = m_BallBackAngle;
	m_AreaStartAngle = m_BallBackAngle;
	float maxEndAngle{ m_AreaStartAngle + 360 - m_AreaGapArcLength };
	m_AreaEndAngle = std::min(m_AreaStartAngle + m_Config.lookAheadArcLength, maxEndAngle);

	//generate walls
	m_NextWallIdx = 0;
	int numWalls{ rand() % (m_MaxNumWalls - m_MinNumWalls) + m_MinNumWalls };
	m_Walls.resize(numWalls);

	float arcLengthToNextWall{ m_Config.startEmptyArcLength };
	float angle{ m_BallAngle};
	for (int i{}; i < m_Walls.capacity(); ++i)
	{
		bool onInside{ (int(m_StartOnInside) + i) % 2 == 0 };
		float startAngle{ angle + arcLengthToNextWall };
		float wallArcLength{ rand() % int(m_Config.maxWallArcLength - m_Config.minWallArcLength) + m_Config.minWallArcLength };
		float endAngle{ startAngle + wallArcLength };
		m_Walls[i] = Wall{ onInside, startAngle, endAngle };
		angle = endAngle;
		arcLengthToNextWall = rand() % int(m_Config.maxAngleBetweenWalls - m_Config.minAngleBetweenWalls) + m_Config.minAngleBetweenWalls;
	}
	angle += m_Config.finishArcLength;
	m_FinishAngle = angle;

	CalculateTimeToComplete();
}

void WallDodgeGame::Click(GameData::Feedback& feedback)
{
	//Check if the ball is past the last wall already
	if (m_NextWallIdx > m_Walls.size() - 1)
	{
		m_BallOnInside = !m_BallOnInside;
		return;
	}

	//Check if the ball angle overlaps with the next wall
	if (IsBallOverlappingWall(m_NextWallIdx))
	{
		feedback.failedMiniGame = true;
		m_State = State::Failed;
	}
	else
	{
		m_BallOnInside = !m_BallOnInside;
	}

	const Wall& nextWall{ m_Walls[m_NextWallIdx] };
	if (m_BallBackAngle > nextWall.endAngle)
		++m_NextWallIdx;
}

void WallDodgeGame::ConfigureDifficulty(int difficulty)
{
	if (difficulty == 0)
	{
		m_Config.ballRotSpeed = 60.f;
		m_Config.startEmptyArcLength = 60.f;
		m_Config.finishArcLength = 45.f;
		m_Config.minAngleBetweenWalls = 60.f;
		m_Config.maxAngleBetweenWalls = 90.f;
		m_Config.minWallArcLength = 15.f;
		m_Config.maxWallArcLength = 30.f;
		m_Config.lookAheadArcLength = 180.f;
	}

	m_Config.lookAheadArcLength = std::min(360.f - m_AreaGapArcLength, m_Config.lookAheadArcLength);
}

void WallDodgeGame::CalculateTimeToComplete()
{
	m_MaxTimeToComplete = (m_FinishAngle - m_StartAngle) / m_Config.ballRotSpeed;
}

void WallDodgeGame::UpdateBallAngle(float deltaAngle)
{
	m_BallAngle += deltaAngle;

	float laneWidth{ (m_DrawData.outerRad - m_DrawData.innerRad) / 2.f };
	float laneHalfWidth{ laneWidth / 2.f };
	float ballRad{ laneWidth / 2.f * m_BallSizePercOfLane };
	float ballCenterRad{ m_BallOnInside ? m_DrawData.innerRad + laneHalfWidth : m_DrawData.outerRad - laneHalfWidth };
	float angleDiff{ utils::Degrees(std::tanf(ballRad / ballCenterRad)) };
	m_BallFrontAngle = m_BallAngle + angleDiff;
	m_BallBackAngle = m_BallAngle - angleDiff;
}

bool WallDodgeGame::IsBallOverlappingWall(int ballIdx)
{
	const Wall& wall{ m_Walls[m_NextWallIdx] };
	return ballIdx >= 0 && ballIdx < m_Walls.size() && m_BallOnInside == wall.onInside &&
		(utils::IsAngleBetween(m_BallFrontAngle, wall.startAngle, wall.endAngle) ||
		 utils::IsAngleBetween(m_BallBackAngle, wall.startAngle, wall.endAngle));
}
