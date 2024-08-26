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

	enum class AngleMode
	{
		Degrees, Radians
	};

	struct ArcInfo
	{
		Point2f center;
		float innerRad;
		float outerRad;
		float startAngle;
		float endAngle;
	};

	struct SpiralInfo
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

	struct SpiralRadInfo
	{
		float startInner;
		float startOuter;
		float endInner;
		float endOuter;
	};

	struct SpiralInfoAtAngle
	{
		float spiralRad;
		float centerRad;
		float innerRad;
		float outerRad;
		float width;
	};

	static void DrawLineOnSpiral(const SpiralInfo& info, float angle, float lineWidth = 1.f);
	static void DrawSpiralVertices(const Point2f& center, float startRad, float endRad, float startAngle, float endAngle);
	static void DrawSpiral(const Point2f& center, float startRad, float endRad, float startAngle, float endAngle, float lineWidth = 1.f);
	
	//Arcs
	static void DrawArcBoundary(const ArcInfo& arcInfo, float lineWidth = 1.0f);
	static void DrawFilledArc(const ArcInfo& arcInfo);
	static void DrawPartialArcBoundary(const ArcInfo& arcInfo, float startAngle, float endAngle, float lineWidth = 1.f);
	static void DrawPartiallyFilledArc(const ArcInfo& arcInfo, float startAngle, float endAngle);
	
	//Spirals
	static void DrawSpiralBoundary(const SpiralInfo& info, float lineWidth = 1.f);
	static void DrawFilledSpiral(const SpiralInfo& info);
	static void DrawPartialSpiralBoundary(const SpiralInfo& info, float startAngle, float endAngle, float lineWidth = 1.f);
	static void DrawPartiallyFilledSpiral(const SpiralInfo& info, float startAngle, float endAngle);
	
	//utility
	static void SetAngleMode(AngleMode angleMode);
	static SpiralInfo GetPartialSpiralInfo(const SpiralInfo& info, float startAngle, float endAngle);
	static SpiralInfoAtAngle GetSpiralInfoAtAngle(const SpiralInfo& info, float angle);
	static SpiralRadInfo GetSpiralRadInfo(float startRad, float endRad, float startWidth, float endWidth, DrawMode drawMode);
	static float Degrees(float radians);
	static float Radians(float degrees);
	static bool IsAngleBetween(float angle, float startAngle, float endAngle);
	static float ClampAngleBetween(float angle, float startAngle, float endAngle);
private:
	Spiral();

	static void DrawSpiralBoundary(const SpiralInfo& info, const SpiralRadInfo& radInfo, float lineWidth = 1.f);
	static void DrawFilledSpiral(const SpiralInfo& info, const SpiralRadInfo& radInfo);

	static AngleMode s_AngleMode;
};

