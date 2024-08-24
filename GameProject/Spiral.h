#pragma once

class Spiral final
{
public:
	/*
	centered: centers the spiral around startRad/endRad
	inner: places the spiral on the inside of startRad/endRad
	outer: places the spiral on the outside of startRad/endRad
	extrema: the min/max of startRad/endRad are the min/max radii for the spiral
	*/
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
		DrawMode drawMode;
	};

	struct DrawRadInfo
	{
		float startInner;
		float startOuter;
		float endInner;
		float endOuter;
	};

	struct SpiralAngleInfo
	{
		float spiralRad;
		float centerRad;
		float innerRad;
		float outerRad;
		float width;
	};

	static void DrawLineOnSpiral(const DrawInfo& info, float angle, float lineWidth = 1.f);
	static void DrawSpiralVertices(const Point2f& center, float startRad, float endRad, float startAngle, float endAngle);
	static void DrawSpiral(const Point2f& center, float startRad, float endRad, float startAngle, float endAngle, float lineWidth = 1.f);
	static void DrawArcBoundary(const Point2f& center, float innerRad, float outerRad, float startAngle, float endAngle, float lineWidth = 1.0f);
	static void DrawFilledArc(const Point2f& center, float innerRad, float outerRad, float startAngle, float endAngle);
	static void DrawSpiralBoundary(const DrawInfo& info, float lineWidth = 1.f);
	static void DrawSpiralBoundary(const DrawInfo& info, const DrawRadInfo& radInfo, float lineWidth = 1.f);
	static void DrawFilledSpiral(const DrawInfo& info);
	static void DrawFilledSpiral(const DrawInfo& info, const DrawRadInfo& radInfo);
	static void DrawPartialSpiralBoundary(const DrawInfo& info, float startAngle, float endAngle, float lineWidth = 1.f);
	static void DrawPartiallyFilledSpiral(const DrawInfo& info, float startAngle, float endAngle);
	static DrawInfo GetPartialSpiralDrawInfo(const DrawInfo& info, float startAngle, float endAngle);
	static SpiralAngleInfo GetSpiralAngleInfo(const DrawInfo& info, float angle);
	static DrawRadInfo GetDrawRadInfo(float startRad, float endRad, float startWidth, float endWidth, DrawMode drawMode);
private:
	Spiral();
};

