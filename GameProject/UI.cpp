#include "pch.h"
#include "UI.h"
#include <sstream>
#include <cassert>

UI::UI(const Rectf& viewport)
	: m_Viewport{ viewport }
	, m_Score{ 0 }
	, m_pNumberTextures{ std::vector<Texture*>(10) }
	, m_LivesBarHeight{ 30.f }
	, m_ScoreBarHeight{ m_Viewport.height / 20.f }
{
	for (int i{}; i < 10; ++i)
		m_pNumberTextures[i] = new Texture(std::to_string(i), "DIN-Light.otf", 40, Color4f{1.f, 1.f, 1.f, 1.f});
}

UI::~UI()
{
	for (Texture* pTexture : m_pNumberTextures)
		delete pTexture;
}

void UI::Draw(int score, int lives, int maxLives, float percOfLastLifeRemaining) const
{
	glPushMatrix();
	DrawLives(lives, maxLives, percOfLastLifeRemaining, m_LivesBarHeight);
	glTranslatef(0.f, m_ScoreBarHeight, 0.f);
	DrawScore(score, m_ScoreBarHeight);
	glPopMatrix();
}

float UI::GetHeight() const
{
	return m_LivesBarHeight + m_ScoreBarHeight;
}

void UI::DrawScore(int score, float drawHeight) const
{
	std::string digistString{ std::to_string(score) };
	std::reverse(digistString.begin(), digistString.end());
	std::stringstream digitsStream{ digistString };

	float maxDigitWidth{ m_Viewport.width / 9.f };
	int numDigits{ int(digitsStream.str().size()) };
	float digitWidth{ std::min(maxDigitWidth, m_Viewport.width / numDigits) };
	float scoreWidth{ numDigits * digitWidth };
	float padding{ (m_Viewport.width - scoreWidth) * 0.5f };

	int digitCount{};
	char digitChar;
	//Draw the score
	while (digitsStream >> digitChar)
	{
		++digitCount;
		int digit{ digitChar - '0' }; //ascii math
		assert(digit >= 0 && digit < m_pNumberTextures.size());
		float rightPos{ m_Viewport.width - padding - digitCount * digitWidth };
		Rectf dstRect{ rightPos, 0.f, digitWidth, drawHeight };
		m_pNumberTextures[digit]->Draw(dstRect);
	}
}

void UI::DrawLives(int lives, int maxLives, float percOfLastLifeRemaining, float drawHeight) const
{
	float padding{ m_Viewport.width * 0.05f };

	float lifeWidth{ (m_Viewport.width - padding * 2) / maxLives };

	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	for (int i{}; i < lives - 1; ++i)
	{
		float leftPos{ padding + i * lifeWidth };
		utils::FillRect(leftPos, 0.f, lifeWidth, drawHeight);
	}
	float lastLifeLeftPos{ padding + (lives - 1) * lifeWidth };
	float lastLifeWidth{ percOfLastLifeRemaining * lifeWidth };
	utils::FillRect(lastLifeLeftPos, 0.f, lastLifeWidth, drawHeight);

	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	for (int i{}; i < maxLives; ++i)
	{
		float leftPos{ padding + i * lifeWidth };

		utils::DrawRect(leftPos, 0.f, lifeWidth, drawHeight);
	}
}
