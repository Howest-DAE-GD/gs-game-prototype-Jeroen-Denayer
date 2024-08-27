#include "pch.h"
#include "Spiral.h"

Spiral::AngleMode Spiral::s_AngleMode{ Spiral::AngleMode::Degrees };

void Spiral::DrawLineOnSpiral(const SpiralInfo& info, float angle, float lineWidth)
{
	SpiralInfoAtAngle angleInfo{ Spiral::GetSpiralInfoAtAngle(info, angle) };
	utils::DrawRadialLine(info.center, angleInfo.innerRad, angleInfo.outerRad, angle, lineWidth);
}

void Spiral::DrawSpiralVertices(const Point2f& center, float startRad, float endRad, float startAngle, float endAngle)
{
	if (s_AngleMode == AngleMode::Degrees)
	{
		startAngle = Radians(startAngle);
		endAngle = Radians(endAngle);
	}

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

void Spiral::DrawArcBoundary(const ArcInfo& arcInfo, float lineWidth)
{
	float rad{ arcInfo.innerRad + 0.5f * (arcInfo.outerRad - arcInfo.innerRad) };
	float width{ arcInfo.outerRad - arcInfo.innerRad };
	SpiralInfo drawInfo{ arcInfo.center, rad, rad, arcInfo.startAngle, arcInfo.endAngle, width ,width, Spiral::SpiralDrawMode::centered };
	DrawSpiralBoundary(drawInfo, lineWidth);
}

void Spiral::DrawFilledArc(const ArcInfo& arcInfo)
{
	float rad{ arcInfo.innerRad + 0.5f * (arcInfo.outerRad - arcInfo.innerRad) };
	float width{ arcInfo.outerRad - arcInfo.innerRad };
	SpiralInfo drawInfo{ arcInfo.center, rad, rad, arcInfo.startAngle, arcInfo.endAngle, width ,width, Spiral::SpiralDrawMode::centered };
	DrawFilledSpiral(drawInfo);
}

void Spiral::DrawPartialArcBoundary(const ArcInfo& arcInfo, float startAngle, float endAngle, float lineWidth)
{
	if (!Spiral::IsAngleBetween(startAngle, arcInfo.startAngle, arcInfo.endAngle) &&
		!Spiral::IsAngleBetween(endAngle, arcInfo.startAngle, arcInfo.endAngle))
		return;

	float rad{ arcInfo.innerRad + 0.5f * (arcInfo.outerRad - arcInfo.innerRad) };
	float width{ arcInfo.outerRad - arcInfo.innerRad };
	SpiralInfo drawInfo{ arcInfo.center, rad, rad, arcInfo.startAngle, arcInfo.endAngle, width ,width, Spiral::SpiralDrawMode::centered };
	SpiralInfo partialInfo{ GetPartialSpiralInfo(drawInfo, startAngle, endAngle) };
	SpiralRadInfo radInfo{ GetSpiralRadInfo(partialInfo.startRad, partialInfo.endRad, partialInfo.startWidth, partialInfo.endWidth, SpiralDrawMode::centered) };
	DrawSpiralBoundary(partialInfo, radInfo, lineWidth);
}

void Spiral::DrawPartiallyFilledArc(const ArcInfo& arcInfo, float startAngle, float endAngle)
{
	if (!Spiral::IsAngleBetween(startAngle, arcInfo.startAngle, arcInfo.endAngle) && 
		!Spiral::IsAngleBetween(endAngle, arcInfo.startAngle, arcInfo.endAngle) )
		return;

	float rad{ arcInfo.innerRad + 0.5f * (arcInfo.outerRad - arcInfo.innerRad) };
	float width{ arcInfo.outerRad - arcInfo.innerRad };
	SpiralInfo drawInfo{ arcInfo.center, rad, rad, arcInfo.startAngle, arcInfo.endAngle, width ,width, Spiral::SpiralDrawMode::centered };
	SpiralInfo partialInfo{ GetPartialSpiralInfo(drawInfo, startAngle, endAngle) };
	SpiralRadInfo radInfo{ GetSpiralRadInfo(partialInfo.startRad, partialInfo.endRad, partialInfo.startWidth, partialInfo.endWidth, SpiralDrawMode::centered) };
	DrawFilledSpiral(partialInfo, radInfo);
}

void Spiral::DrawSpiralBoundary(const SpiralInfo& info, float lineWidth)
{
	SpiralRadInfo radInfo{ GetSpiralRadInfo(info.startRad, info.endRad, info.startWidth, info.endWidth, info.drawMode) };
	DrawSpiralBoundary(info, radInfo, lineWidth);
}

void Spiral::DrawSpiralBoundary(const SpiralInfo& info, const SpiralRadInfo& radInfo, float lineWidth)
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	{
		glVertex2f(info.center.x + radInfo.startInner * std::cosf(ConvertAngle(info.startAngle)), info.center.y + radInfo.startInner * std::sinf(ConvertAngle(info.startAngle)));
		DrawSpiralVertices(info.center, radInfo.startOuter, radInfo.endOuter, info.startAngle, info.endAngle);
		DrawSpiralVertices(info.center, radInfo.endInner, radInfo.startInner, info.endAngle, info.startAngle);
	}
	glEnd();
}

void Spiral::DrawFilledSpiral(const SpiralInfo& info)
{
	SpiralRadInfo radInfo{ GetSpiralRadInfo(info.startRad, info.endRad, info.startWidth, info.endWidth, info.drawMode) };
	DrawFilledSpiral(info, radInfo);
}

void Spiral::DrawFilledSpiral(const SpiralInfo& info, const SpiralRadInfo& radInfo)
{
	float startAngle{ s_AngleMode == AngleMode::Degrees ? Radians(info.startAngle) : info.startAngle };
	float endAngle{ s_AngleMode == AngleMode::Degrees ? Radians(info.endAngle) : info.endAngle };

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

	glBegin(GL_TRIANGLE_STRIP);
	{
		float percOfSpiral{ 0.f };
		float dAngle{ float(M_PI) / info.startRad };
		for (float angle{ startAngle }; condition(angle, endAngle); angle += dAngle * loopDir)
		{
			percOfSpiral = std::abs(angle - startAngle) / std::abs(endAngle - startAngle);
			float innerRad{ radInfo.startInner + percOfSpiral * (radInfo.endInner - radInfo.startInner) };
			float outerRad{ radInfo.startOuter + percOfSpiral * (radInfo.endOuter - radInfo.startOuter) };
			glVertex2f(info.center.x + outerRad * cos(angle), info.center.y + outerRad * sin(angle));
			glVertex2f(info.center.x + innerRad * cos(angle), info.center.y + innerRad * sin(angle));
			dAngle = float(M_PI) / outerRad;
		}
		glVertex2f(info.center.x + radInfo.endOuter * cos(endAngle), info.center.y + radInfo.endOuter * sin(endAngle));
		glVertex2f(info.center.x + radInfo.endInner * cos(endAngle), info.center.y + radInfo.endInner * sin(endAngle));
	}
	glEnd();
}

float Spiral::Degrees(float radians)
{
	return radians * 180.f / float(M_PI);
}

float Spiral::Radians(float degrees)
{
	return degrees * float(M_PI) / 180.f;
}

bool Spiral::IsAngleBetween(float angle, float startAngle, float endAngle)
{
	if (startAngle > endAngle)
		std::swap(startAngle, endAngle);
	return angle >= startAngle && angle <= endAngle;
}

float Spiral::ClampAngleBetween(float angle, float startAngle, float endAngle)
{
	float minAngle{ std::min(startAngle, endAngle) };
	float maxAngle{ std::max(startAngle, endAngle) };
	if (angle < minAngle)
		angle = minAngle;
	else if (angle > maxAngle)
		angle = maxAngle;
	return angle;
}

float Spiral::ConvertAngle(float angle)
{
	return s_AngleMode == AngleMode::Degrees ? Radians(angle) : angle;
}

void Spiral::DrawPartialSpiralBoundary(const SpiralInfo& info, float startAngle, float endAngle, float lineWidth)
{
	SpiralInfo partialInfo{ GetPartialSpiralInfo(info, startAngle, endAngle) };
	SpiralRadInfo radInfo{ GetSpiralRadInfo(partialInfo.startRad, partialInfo.endRad, partialInfo.startWidth, partialInfo.endWidth, SpiralDrawMode::centered) };
	DrawSpiralBoundary(partialInfo, radInfo, lineWidth);
}

void Spiral::DrawPartiallyFilledSpiral(const SpiralInfo& info, float startAngle, float endAngle)
{
	SpiralInfo partialInfo{ GetPartialSpiralInfo(info, startAngle, endAngle) };
	SpiralRadInfo radInfo{ GetSpiralRadInfo(partialInfo.startRad, partialInfo.endRad, partialInfo.startWidth, partialInfo.endWidth, SpiralDrawMode::centered) };
	DrawFilledSpiral(partialInfo, radInfo);
}

void Spiral::DrawFilledHalfArrow(const Point2f& pos, const HalfArrowInfo& info, bool flip)
{
	if (info.endArcLength == 0)
		return;

	float startAngle{}, endAngle{};
	float startRad{}, endRad{};
	Vector2f startPoint{}, endPoint{};
	Vector2f offset{};
	if (info.outwardsCurve)
	{
		startAngle = float(M_PI) - ConvertAngle(std::abs(info.endArcLength));
		endAngle = float(M_PI);
		startRad = info.endRad;
		endRad = info.baseRad;
		startPoint = Vector2f{ (info.endRad - info.endHeight) * std::cosf(startAngle),  (info.endRad - info.endHeight) * std::sinf(startAngle) };
		endPoint = Vector2f{ -info.baseRad + info.baseHeight, 0.f };
		offset = Vector2f{ info.baseRad, 0.f};
	}
	else
	{
		startAngle = 0.f;
		endAngle = ConvertAngle(std::abs(info.endArcLength));
		startRad = info.baseRad;
		endRad = info.endRad;
		startPoint = Vector2f{ (info.baseRad + info.baseHeight) * std::cosf(startAngle), (info.baseRad + info.baseHeight) * std::sinf(startAngle) };
		endPoint = Vector2f{ (info.endRad + info.endHeight) * std::cosf(endAngle), (info.endRad + info.endHeight) * std::sinf(endAngle) };
		offset = Vector2f{ -info.baseRad, 0.f };
	}

	switch (info.drawMode)
	{
	case ArrowDrawMode::base:
		//Default draw mode
		break;
	case ArrowDrawMode::centered:
		offset.x += info.outwardsCurve ? -info.baseHeight / 2.f : -info.baseHeight / 2.f;
		break;
	case ArrowDrawMode::inner:
		offset.x += info.outwardsCurve ? 0.f : -info.baseHeight;
		break;
	case ArrowDrawMode::outer:
		offset.x += info.outwardsCurve ? -info.baseHeight : 0.f;
		break;
	}

	glPushMatrix();
	int yFlip{ info.endArcLength < 0 ? -1 : 1 };
	int xFlip{ flip ? -1 : 1 };
	glTranslatef(pos.x, pos.y, 0);
	glRotatef(info.baseAngle, 0, 0, 1);
	glScalef(xFlip, yFlip, 1);

	glBegin(GL_TRIANGLE_STRIP);
	{
		float percOfArc{ 0.f };
		float dAngle{ float(M_PI) / startRad };
		for (float angle{ startAngle }; angle < endAngle; angle += dAngle)
		{
			percOfArc = std::abs(angle - startAngle) / std::abs(endAngle - startAngle);
			float rad{ startRad + percOfArc * (endRad - startRad) };
			Vector2f linePoint{ startPoint + percOfArc * (endPoint - startPoint) };
			glVertex2f(rad * std::cosf(angle) + offset.x, rad * std::sinf(angle) + offset.y);
			glVertex2f(linePoint.x + offset.x, linePoint.y + offset.y);
			dAngle = float(M_PI) / rad * 2.f;
		}
		glVertex2f(endRad * std::cosf(endAngle) + offset.x, endRad * std::sinf(endAngle) + offset.y);
		glVertex2f(endPoint.x + offset.x, endPoint.y + offset.y);
	}
	glEnd();

	glPopMatrix();
}

void Spiral::DrawFilledArrow(const Point2f& pos, const ArrowInfo& info, bool flip)
{
	if (info.leftArcLength != 0.f)
	{
		HalfArrowInfo leftHalf{ info.baseAngle, info.baseRad, info.baseHeight, info.leftArcLength, info.leftRad, info.leftHeight, info.outwardsCurve, info.drawMode };
		DrawFilledHalfArrow(pos, leftHalf, flip);
	}
	if (info.rightArcLength != 0.f)
	{
		HalfArrowInfo rightHalf{ info.baseAngle, info.baseRad, info.baseHeight, info.rightArcLength, info.rightRad, info.rightHeight, info.outwardsCurve, info.drawMode };
		DrawFilledHalfArrow(pos, rightHalf, flip);
	}
}

void Spiral::DrawFilledArrowOnArc(const ArcInfo& arcInfo, float baseAngle, float baseRad, const ArrowShapeDef& arrowShapeDef, bool flip)
{
	float xPos{ baseRad * std::cosf(ConvertAngle(baseAngle)) };
	float yPos{ baseRad * std::sinf(ConvertAngle(baseAngle)) };
	ArrowInfo arrowInfo{ baseAngle, baseRad, arrowShapeDef.baseHeight, arrowShapeDef.endArcLength, -arrowShapeDef.endArcLength, baseRad, baseRad, arrowShapeDef.endHeight, arrowShapeDef.endHeight, arrowShapeDef.outwardsCurve, arrowShapeDef.drawMode };
	DrawFilledArrow(Point2f{ xPos, yPos }, arrowInfo, flip);
}

void Spiral::SetAngleMode(AngleMode angleMode)
{
	s_AngleMode = angleMode;
}

Spiral::SpiralInfo Spiral::GetPartialSpiralInfo(const SpiralInfo& info, float startAngle, float endAngle)
{
	startAngle = ClampAngleBetween(startAngle, info.startAngle, info.endAngle);
	endAngle = ClampAngleBetween(endAngle, info.startAngle, info.endAngle);
	SpiralInfoAtAngle startAngleInfo{ GetSpiralInfoAtAngle(info, startAngle) };
	SpiralInfoAtAngle endAngleInfo{ GetSpiralInfoAtAngle(info, endAngle) };
	return SpiralInfo{ info.center, startAngleInfo.centerRad, endAngleInfo.centerRad, startAngle, endAngle, startAngleInfo.width, endAngleInfo.width, SpiralDrawMode::centered};
}

Spiral::SpiralInfoAtAngle Spiral::GetSpiralInfoAtAngle(const SpiralInfo& info, float angle)
{
	//clamp startAngle/endAngle to the spiral
	angle = ClampAngleBetween(angle, info.startAngle, info.endAngle);

	//calculate how far the angle is in the spiral
	float percOfSpiral{ std::abs(angle - info.startAngle) / std::abs(info.endAngle - info.startAngle) };
	float spiralRad{ info.startRad + percOfSpiral * (info.endRad - info.startRad) };
	//calculate the center radius for the spiral
	SpiralRadInfo radInfo{ GetSpiralRadInfo(info.startRad, info.endRad, info.startWidth, info.endWidth, info.drawMode) };
	float startCenterRad{ radInfo.startInner + 0.5f * (radInfo.startOuter - radInfo.startInner) };
	float endCenterRad{ radInfo.endInner + 0.5f * (radInfo.endOuter - radInfo.endInner) };
	float centerRad{ startCenterRad + percOfSpiral * (endCenterRad - startCenterRad) };
	float innerRad{ radInfo.startInner + percOfSpiral * (radInfo.endInner - radInfo.startInner) };
	float outerRad{ radInfo.startOuter + percOfSpiral * (radInfo.endOuter - radInfo.startOuter) };
	float width{ info.startWidth + percOfSpiral * (info.endWidth - info.startWidth) };

	return SpiralInfoAtAngle{ spiralRad, centerRad, innerRad, outerRad, width };
}

Spiral::SpiralRadInfo Spiral::GetSpiralRadInfo(float startRad, float endRad, float startWidth, float endWidth, SpiralDrawMode drawMode)
{
	float minRad{ std::min(startRad, endRad) };
	float maxRad{ std::max(startRad, endRad) };

	float startHalfWidth{ 0.5f * startWidth };
	float endHalfWidth{ 0.5f * endWidth };

	float startInnerRad{}, startOuterRad{}, endInnerRad{}, endOuterRad{};
	switch (drawMode)
	{
	case SpiralDrawMode::centered:
		startInnerRad = std::max(maxRad - startHalfWidth, 0.f);
		startOuterRad = maxRad + startHalfWidth;
		endInnerRad = std::max(minRad - endHalfWidth, 0.f);
		endOuterRad = minRad + endHalfWidth;
		break;
	case SpiralDrawMode::inner:
		startInnerRad = std::max(maxRad - startWidth, 0.f);
		startOuterRad = maxRad;
		endInnerRad = std::max(minRad - endWidth, 0.f);
		endOuterRad = minRad;
		break;
	case SpiralDrawMode::outer:
		startInnerRad = maxRad;
		startOuterRad = maxRad + startWidth;
		endInnerRad = minRad;
		endOuterRad = minRad + endWidth;
		break;
	case SpiralDrawMode::extrema:
		startInnerRad = maxRad - startWidth;
		startOuterRad = maxRad;
		endInnerRad = minRad;
		endOuterRad = minRad + endWidth;
		break;
	}

	return SpiralRadInfo{ startInnerRad, startOuterRad, endInnerRad, endOuterRad };
}