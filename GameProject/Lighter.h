#pragma once
class Lighter
{
public:
	struct Data
	{
		Point2f pos;
		float innerRadius;
		float outerRadius;
	};

	Lighter(Point2f startPos, float hitRadius, float hitRadiusWidth);

	void Draw() const;
	Data GetData() const;
private:
	Point2f m_Pos;
	float m_InnerRadius;
	float m_OuterRadius;
};

