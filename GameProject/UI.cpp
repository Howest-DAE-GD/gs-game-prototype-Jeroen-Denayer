#include "pch.h"
#include "UI.h"
#include <sstream>

UI::UI(const Rectf& viewport)
	: m_Viewport{ viewport }
	, m_Score{ 0 }
	, m_pNumberTextures{ std::vector<Texture*>(10) }
	, m_MaxDigitWidth{ m_Viewport.width / 9.f }
	, m_MaxDigitHeight{ m_Viewport.height / 20.f }
{
	for (int i{}; i < 10; ++i)
		m_pNumberTextures[i] = new Texture(std::to_string(i), "DIN-Light.otf", 40, Color4f{1.f, 1.f, 1.f, 1.f});
}

UI::~UI()
{
	for (Texture* pTexture : m_pNumberTextures)
		delete pTexture;
}

void UI::Draw() const
{
	float number{ float(m_Score) };
	std::stringstream digitsStream{};
	while (number >= 0.f)
	{
		number = int(number);
		int digit{ int(number - int(number / 10) * 10) };
		digitsStream << digit;
		number = number / 10;
		if (number < 1.f)
			break;
	}

	int numDigits{ int(digitsStream.str().size()) };
	float digitWidth{ std::min(m_MaxDigitWidth, m_Viewport.width / numDigits) };
	float scoreWidth{ numDigits * digitWidth };
	float padding{ (m_Viewport.width - scoreWidth) * 0.5f };

	int digitCount{};
	char digitChar;
	while (digitsStream >> digitChar)
	{
		++digitCount;
		int digit{ digitChar - '0' }; //ascii math
		Rectf dstRect{ m_Viewport.width - padding - digitCount * digitWidth, m_Viewport.height - m_MaxDigitHeight, digitWidth, m_MaxDigitHeight };
		m_pNumberTextures[digit]->Draw(dstRect);
	}
}

void UI::IncreaseScore(int addedScore)
{
	m_Score += addedScore;
}
