#pragma once

class Lighter final
{
public:
	struct Data
	{
		Point2f pos;
		float size;
		float innerRadius;
		float outerRadius;
		float angle;
	};

	Lighter(Point2f startPos, const std::vector<float>& sizes);

	void Draw() const;
	void IncreaseSize();
	void DecreaseSize();
	void IncreaseAngle(float dt);
	void DecreaseAngle(float dt);
	Data GetData() const;
private:
	Point2f m_Pos;
	std::vector<float> m_LighterSizes;
	float m_HitRadiusPerctageOfSize;
	int m_CurSizeIdx;
	float m_Angle;
	float m_RotSpeed;
};

