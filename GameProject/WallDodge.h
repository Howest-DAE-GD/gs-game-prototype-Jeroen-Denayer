#pragma once
#include "MiniGame.h"

class WallDodge final
	:public MiniGame
{
public:
	WallDodge(int difficulty, const DrawData& drawData);

	virtual void Draw(Point2f pos) const override;
	virtual void Update(float dt, const GameData::Input& input, GameData::Feedback& feedback) override;
	virtual void Init(int difficulty, bool activate = false) override;
private:
	struct Wall
	{
		bool onInside;
		float startAngle;
		float endAngle;
	};

	struct Config
	{
		float ballRotSpeed;
		float startEmptyArcLength;
		float finishArcLength;
		float minAngleBetweenWalls;
		float maxAngleBetweenWalls;
		float minWallArcLength;
		float maxWallArcLength;
		float lookAheadArcLength;
	};

	virtual void Click(GameData::Feedback& feedback) override;
	virtual void ConfigureDifficulty(int difficulty) override;
	virtual void CalculateTimeToComplete() override;
	void UpdateBallAngle(float deltaAngle);
	bool IsBallOverlappingWall(int ballIdx);

	//Members
	int m_Dir;
	bool m_StartOnInside;
	bool m_BallOnInside;
	float m_BallAngle;
	float m_BallFrontAngle;
	float m_BallBackAngle;
	float m_BallSizePercOfLane;
	float m_AreaStartAngle;
	float m_AreaEndAngle;
	float m_FinishAngle;
	float m_AreaGapArcLength;
	int m_MaxNumWalls;
	int m_MinNumWalls;
	int m_NumWallsPerPoint;
	int m_NextWallIdx;
	std::vector<Wall> m_Walls;

	Config m_Config;
};

