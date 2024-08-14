#include "pch.h"
#include "Lighter.h"

Lighter::Lighter(Point2f startPos, float hitRadius, float hitRadiusWidth)
	: m_Pos{ startPos }
	, m_InnerRadius{ hitRadius - hitRadiusWidth / 2.f }
	, m_OuterRadius{ hitRadius + hitRadiusWidth / 2.f }
{
}

void Lighter::Draw() const
{
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawEllipse(m_Pos, m_InnerRadius, m_InnerRadius);
	utils::DrawEllipse(m_Pos, m_OuterRadius, m_OuterRadius);
}

Lighter::Data Lighter::GetData() const
{
	return Data{ m_Pos, m_InnerRadius, m_OuterRadius };
}
