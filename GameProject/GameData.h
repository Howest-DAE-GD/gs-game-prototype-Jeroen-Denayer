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
		int totalPoints;
		int newPoints;
		bool lostLife;
	};
}

