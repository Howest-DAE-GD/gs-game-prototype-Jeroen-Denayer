#pragma once

namespace GameData
{
	struct Input
	{
		bool pressedSpace;
		bool pressedLeft;
		bool pressedRight;
	};

	struct Feedback
	{
		bool finishedBall;
		bool perfectBall;
		int totalBallPoints;
		int newPoints;
		bool failedMiniGame;
	};
}

