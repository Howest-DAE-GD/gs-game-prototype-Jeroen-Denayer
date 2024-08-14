#include "pch.h"
#include "Lighter.h"

Lighter::Lighter(Point2f startPos, const std::vector<float>& sizes)
	: m_Pos{ startPos }
	, m_LighterSizes{ sizes }
	, m_HitRadiusPerctageOfSize{ 0.2f }
	, m_CurSizeIdx{ int(m_LighterSizes.size() / 2.f) }
{
}

void Lighter::Draw() const
{
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	Data lighterData{ GetData() };
	utils::DrawEllipse(lighterData.pos, lighterData.innerRadius, lighterData.innerRadius);
	utils::DrawEllipse(lighterData.pos, lighterData.outerRadius, lighterData.outerRadius);
}

void Lighter::IncreaseSize()
{
	if (m_CurSizeIdx < m_LighterSizes.size() - 1)
		++m_CurSizeIdx;
}

void Lighter::DecreaseSize()
{
	if (m_CurSizeIdx > 0)
		--m_CurSizeIdx;
}

Lighter::Data Lighter::GetData() const
{
	float size{ m_LighterSizes[m_CurSizeIdx] };
	float radius{ size / 2.f };
	float innerRadius{ radius - size * m_HitRadiusPerctageOfSize };
	float outerRadius{ radius + size * m_HitRadiusPerctageOfSize };
	return Data{ m_Pos, size, innerRadius, outerRadius };
}
