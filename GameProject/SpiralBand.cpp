#include "pch.h"
#include "SpiralBand.h"

SpiralBand::SpiralBand(float startAngle, float endAngle, float startRad, float endRad, float startWidth, float endWidth, DrawMode drawMode)
	: m_StartAngle{ startAngle }
	, m_EndAngle{ endAngle }
	, m_StartRad{ startRad }
	, m_EndRad{ endRad }
	, m_StartWidth{ startWidth }
	, m_EndWidth{ endWidth }
	, m_DrawMode{ DrawMode::centered }
{
}

void SpiralBand::DrawBoundary(const DrawInfo& info)
{
	DrawRadInfo radInfo{ GetDrawRadInfo(info.startRad, info.endRad, info.startWidth, info.endWidth, info.drawMode) };

	glLineWidth(info.lineWidth);
	glBegin(GL_LINE_STRIP);
	{
		glVertex2f(info.center.x + radInfo.startInner * cos(info.startAngle), info.center.y + radInfo.startInner * sin(info.endAngle));
		utils::DrawSpiralVertices(info.center, radInfo.startOuter, radInfo.endOuter, info.startAngle, info.endAngle);
		utils::DrawSpiralVertices(info.center, radInfo.endInner, radInfo.startInner, info.endAngle, info.startAngle);
	}
	glEnd();
}

void SpiralBand::DrawFilled(const DrawInfo& info)
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

	float startHalfWidth{ 0.5f * info.startWidth };
	float endHalfWidth{ 0.5f * info.endWidth };

	DrawRadInfo radInfo{ GetDrawRadInfo(info.startRad, info.endRad, info.startWidth, info.endWidth, info.drawMode) };

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
	}
	glEnd();
}

SpiralBand::DrawRadInfo SpiralBand::GetDrawRadInfo(float startRad, float endRad, float startWidth, float endWidth, DrawMode drawMode)
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

void SpiralBand::DrawBoundary(const Point2f& center, float lineWidth) const
{
	DrawInfo drawInfo{center, m_StartRad, m_EndRad, m_StartAngle, m_EndAngle, m_StartWidth, m_EndWidth, lineWidth, m_DrawMode};
	DrawBoundary(drawInfo);
}

void SpiralBand::DrawFilled(const Point2f& center) const
{
	DrawInfo drawInfo{ center, m_StartRad, m_EndRad, m_StartAngle, m_EndAngle, m_StartWidth, m_EndWidth, 1.f, m_DrawMode };
	DrawFilled(drawInfo);
}

void SpiralBand::SetMode(DrawMode drawMode)
{
	m_DrawMode = drawMode;
}
