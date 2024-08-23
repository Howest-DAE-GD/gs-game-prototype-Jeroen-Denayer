#pragma once

namespace GameData
{
	struct Input
	{
		bool pressedSpace;
		bool pressedLeft;
		bool pressedRight;
		float deadLineHeight;
	};

	struct Feedback
	{
		int totalPoints;
		int newPoints;
		bool lostLife;
	};
}

