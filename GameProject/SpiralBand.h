#pragma once

class SpiralBand
{
public:
	enum class DrawMode
	{
		centered, inner, outer, extrema
	};

	struct DrawInfo
	{
		Point2f center;
		float startRad;
		float endRad;
		float startAngle;
		float endAngle;
		float startWidth;
		float endWidth;
		float lineWidth;
		DrawMode drawMode;
	};

	struct DrawRadInfo
	{
		float startInner;
		float startOuter;
		float endInner;
		float endOuter;
	};

	SpiralBand(float startAngle, float endAngle, float startRad, float endRad, float startWidth, float endWidth, DrawMode drawMode);

	static void DrawBoundary(const DrawInfo& info);
	static void DrawFilled(const DrawInfo& info);
	static DrawRadInfo GetDrawRadInfo(float startRad, float endRad, float startWidth, float endWidth, DrawMode drawMode);
	void DrawBoundary(const Point2f& center, float lineWidth = 1.f) const;
	void DrawFilled(const Point2f& center) const;
	/*
	Parameters:
	- mode:
		0: centers the spiral around startRad/endRad
		-1: places the spiral on the inside of startRad/endRad
		1: places the spiral on the outside of startRad/endRad
		2: the min/max of startRad/endRad are the min/max radii for the spiral
	*/
	void SetMode(DrawMode drawMode);

private:
	//Functions

	//Members
	float m_StartAngle;
	float m_EndAngle;
	float m_StartRad; //the start CENTER radius
	float m_EndRad; //the end CENTER radius
	float m_StartWidth;
	float m_EndWidth;
	DrawMode m_DrawMode;
};

