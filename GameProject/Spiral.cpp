#include "pch.h"
#include "Spiral.h"

void Spiral::DrawLineOnSpiral(const DrawInfo& info, float angle, float lineWidth)
{
	SpiralAngleInfo angleInfo{ Spiral::GetSpiralAngleInfo(info, utils::Radians(angle)) };
	Point2f p0{ info.center.x + angleInfo.outerRad * std::cosf(utils::Radians(angle)), info.center.y + angleInfo.outerRad * std::sinf(utils::Radians(angle)) };
	Point2f p1{ info.center.x + angleInfo.innerRad * std::cosf(utils::Radians(angle)), info.center.y + angleInfo.innerRad * std::sinf(utils::Radians(angle)) };
	utils::DrawLine(p0, p1, lineWidth);
}

void Spiral::DrawSpiralVertices(const Point2f& center, float startRad, float endRad, float startAngle, float endAngle)
{
	bool clockWise{ endAngle > startAngle ? false : true };
	int loopDir{ clockWise ? -1 : 1 };

	auto isSmaller{ [](float a, float b) -> bool {
		return a < b;
	} };
	auto isLarger{ [](float a, float b) -> bool {
	return a > b;
	} };

	auto condition = [clockWise, isSmaller, isLarger](float a, float b) -> bool {
		return clockWise ? isLarger(a, b) : isSmaller(a, b);
		};

	float dAngle{ startRad == 0.f ? float(M_PI) / 2.f : float(M_PI) / startRad };
	float percOfSpiral{ 0.f };
	for (float angle{ startAngle }; condition(angle, endAngle); angle += dAngle * loopDir)
	{
		percOfSpiral = std::abs(angle - startAngle) / std::abs(endAngle - startAngle);
		float rad{ startRad + percOfSpiral * (endRad - startRad) };
		glVertex2f(center.x + rad * cos(angle), center.y + rad * sin(angle));
		dAngle = rad == 0.f ? float(M_PI) / 2.f : float(M_PI) / rad;
	}
	glVertex2f(center.x + endRad * cos(endAngle), center.y + endRad * sin(endAngle));
}

void Spiral::DrawSpiral(const Point2f& center, float startRad, float endRad, float startAngle, float endAngle, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	{
		DrawSpiralVertices(center, startRad, endRad, startAngle, endAngle);
	}
	glEnd();
}

void Spiral::DrawArcBoundary(const Point2f& center, float innerRad, float outerRad, float startAngle, float endAngle, float lineWidth)
{
	float rad{ innerRad + 0.5f * (outerRad - innerRad) };
	float width{ outerRad - innerRad };
	DrawInfo drawInfo{ center, rad, rad, startAngle, endAngle, width ,width, Spiral::DrawMode::centered };
	DrawSpiralBoundary(drawInfo, lineWidth);
}

void Spiral::DrawFilledArc(const Point2f& center, float innerRad, float outerRad, float startAngle, float endAngle)
{
	float rad{ innerRad + 0.5f * (outerRad - innerRad) };
	float width{ outerRad - innerRad };
	DrawInfo drawInfo{ center, rad, rad, startAngle, endAngle, width ,width, Spiral::DrawMode::centered };
	DrawFilledSpiral(drawInfo);
}

void Spiral::DrawSpiralBoundary(const DrawInfo& info, float lineWidth)
{
	DrawRadInfo radInfo{ GetDrawRadInfo(info.startRad, info.endRad, info.startWidth, info.endWidth, info.drawMode) };
	DrawSpiralBoundary(info, radInfo, lineWidth);
}

void Spiral::DrawSpiralBoundary(const DrawInfo& info, const DrawRadInfo& radInfo, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	{
		glVertex2f(info.center.x + radInfo.startInner * cos(info.startAngle), info.center.y + radInfo.startInner * sin(info.endAngle));
		DrawSpiralVertices(info.center, radInfo.startOuter, radInfo.endOuter, info.startAngle, info.endAngle);
		DrawSpiralVertices(info.center, radInfo.endInner, radInfo.startInner, info.endAngle, info.startAngle);
	}
	glEnd();
}

void Spiral::DrawFilledSpiral(const DrawInfo& info)
{
	//float startHalfWidth{ 0.5f * info.startWidth };
	//float endHalfWidth{ 0.5f * info.endWidth };
	DrawRadInfo radInfo{ GetDrawRadInfo(info.startRad, info.endRad, info.startWidth, info.endWidth, info.drawMode) };
	DrawFilledSpiral(info, radInfo);
}

void Spiral::DrawFilledSpiral(const DrawInfo& info, const DrawRadInfo& radInfo)
{
	bool clockWise{ info.endAngle > info.startAngle ? false : true };
	int loopDir{ clockWise ? -1 : 1 };

	auto isSmaller{ [](float a, float b) -> bool {
		return a < b;
	} };
	auto isLarger{ [](float a, float b) -> bool {
	return a > b;
	} };

	auto condition = [clockWise, isSmaller, isLarger](float a, float b) -> bool {
		return clockWise ? isLarger(a, b) : isSmaller(a, b);
		};

	glBegin(GL_TRIANGLE_STRIP);
	{
		float percOfSpiral{ 0.f };
		float dAngle{ float(M_PI) / info.startRad };
		for (float angle{ info.startAngle }; condition(angle, info.endAngle); angle += dAngle * loopDir)
		{
			percOfSpiral = std::abs(angle - info.startAngle) / std::abs(info.endAngle - info.startAngle);
			float innerRad{ radInfo.startInner + percOfSpiral * (radInfo.endInner - radInfo.startInner) };
			float outerRad{ radInfo.startOuter + percOfSpiral * (radInfo.endOuter - radInfo.startOuter) };
			glVertex2f(info.center.x + outerRad * cos(angle), info.center.y + outerRad * sin(angle));
			glVertex2f(info.center.x + innerRad * cos(angle), info.center.y + innerRad * sin(angle));
			dAngle = float(M_PI) / outerRad;
		}
		glVertex2f(info.center.x + radInfo.endOuter * cos(info.endAngle), info.center.y + radInfo.endOuter * sin(info.endAngle));
		glVertex2f(info.center.x + radInfo.endInner * cos(info.endAngle), info.center.y + radInfo.endInner * sin(info.endAngle));
	}
	glEnd();
}

void Spiral::DrawPartialSpiralBoundary(const DrawInfo& info, float startAngle, float endAngle, float lineWidth)
{
	DrawInfo partialInfo{ GetPartialSpiralDrawInfo(info, startAngle, endAngle) };
	DrawRadInfo radInfo{ GetDrawRadInfo(partialInfo.startRad, partialInfo.endRad, partialInfo.startWidth, partialInfo.endWidth, DrawMode::centered) };
	DrawSpiralBoundary(partialInfo, radInfo, lineWidth);
}

void Spiral::DrawPartiallyFilledSpiral(const DrawInfo& info, float startAngle, float endAngle)
{
	DrawInfo partialInfo{ GetPartialSpiralDrawInfo(info, startAngle, endAngle) };
	DrawRadInfo radInfo{ GetDrawRadInfo(partialInfo.startRad, partialInfo.endRad, partialInfo.startWidth, partialInfo.endWidth, DrawMode::centered) };
	DrawFilledSpiral(partialInfo, radInfo);
}

Spiral::DrawInfo Spiral::GetPartialSpiralDrawInfo(const DrawInfo& info, float startAngle, float endAngle)
{
	SpiralAngleInfo startAngleInfo{ GetSpiralAngleInfo(info, startAngle) };
	SpiralAngleInfo endAngleInfo{ GetSpiralAngleInfo(info, endAngle) };
	return DrawInfo{ info.center, startAngleInfo.centerRad, endAngleInfo.centerRad, startAngle, endAngle, startAngleInfo.width, endAngleInfo.width, DrawMode::centered};
}

Spiral::SpiralAngleInfo Spiral::GetSpiralAngleInfo(const DrawInfo& info, float angle)
{
	//clamp startAngle/endAngle to the spiral
	float minAngle{ std::min(info.startAngle, info.endAngle) };
	float maxAngle{ std::max(info.startAngle, info.endAngle) };
	if (angle < minAngle)
		angle = minAngle;
	else if (angle > maxAngle)
		angle = maxAngle;

	//calculate how far the angle is in the spiral
	float percOfSpiral{ std::abs(angle - info.startAngle) / std::abs(info.endAngle - info.startAngle) };
	float spiralRad{ info.startRad + percOfSpiral * (info.endRad - info.startRad) };
	//calculate the center radius for the spiral
	DrawRadInfo radInfo{ GetDrawRadInfo(info.startRad, info.endRad, info.startWidth, info.endWidth, info.drawMode) };
	float startCenterRad{ radInfo.startInner + 0.5f * (radInfo.startOuter - radInfo.startInner) };
	float endCenterRad{ radInfo.endInner + 0.5f * (radInfo.endOuter - radInfo.endInner) };
	float centerRad{ startCenterRad + percOfSpiral * (endCenterRad - startCenterRad) };
	float innerRad{ radInfo.startInner + percOfSpiral * (radInfo.endInner - radInfo.startInner) };
	float outerRad{ radInfo.startOuter + percOfSpiral * (radInfo.endOuter - radInfo.startOuter) };
	float width{ info.startWidth + percOfSpiral * (info.endWidth - info.startWidth) };

	return SpiralAngleInfo{ spiralRad, centerRad, innerRad, outerRad, width };
}

Spiral::DrawRadInfo Spiral::GetDrawRadInfo(float startRad, float endRad, float startWidth, float endWidth, DrawMode drawMode)
{
	float minRad{ std::min(startRad, endRad) };
	float maxRad{ std::max(startRad, endRad) };

	float startHalfWidth{ 0.5f * startWidth };
	float endHalfWidth{ 0.5f * endWidth };

	float startInnerRad{}, startOuterRad{}, endInnerRad{}, endOuterRad{};
	switch (drawMode)
	{
	case DrawMode::centered:
		startInnerRad = std::max(maxRad - startHalfWidth, 0.f);
		startOuterRad = maxRad + startHalfWidth;
		endInnerRad = std::max(minRad - endHalfWidth, 0.f);
		endOuterRad = minRad + endHalfWidth;
		break;
	case DrawMode::inner:
		startInnerRad = std::max(maxRad - startWidth, 0.f);
		startOuterRad = maxRad;
		endInnerRad = std::max(minRad - endWidth, 0.f);
		endOuterRad = minRad;
		break;
	case DrawMode::outer:
		startInnerRad = maxRad;
		startOuterRad = maxRad + startWidth;
		endInnerRad = minRad;
		endOuterRad = minRad + endWidth;
		break;
	case DrawMode::extrema:
		startInnerRad = maxRad - startWidth;
		startOuterRad = maxRad;
		endInnerRad = minRad;
		endOuterRad = minRad + endWidth;
		break;
	}

	return DrawRadInfo{ startInnerRad, startOuterRad, endInnerRad, endOuterRad };
}