#include "pch.h"
#include "UI.h"
#include <sstream>
#include <cassert>

UI::UI(const Rectf& viewport)
	: m_Viewport{ viewport }
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

void UI::Draw(std::int64_t score, int multiplier, int lives, int maxLives, float percOfLastLifeRemaining) const
{
	glPushMatrix();
	DrawLives(lives, maxLives, percOfLastLifeRemaining, m_LivesBarHeight);
	glTranslatef(0.f, m_ScoreBarHeight, 0.f);
	DrawScore(score, multiplier, m_ScoreBarHeight);
	glPopMatrix();
}

float UI::GetHeight() const
{
	return m_LivesBarHeight + m_ScoreBarHeight;
}

void UI::DrawStringInRect(const std::string& text, const Rectf& rect, int maxCharWidth, const Color4f& color) const
{
	std::string charString{ text };
	std::reverse(charString.begin(), charString.end());
	std::stringstream charStream{ charString };

	int numChars{ int(charStream.str().size()) };
	int charWidth{ std::min(maxCharWidth, int(rect.width / numChars)) };
	float textWidth{ float(numChars * charWidth) };
	float padding{ (rect.width - textWidth) * 0.5f };

	Rectf drawRect{ rect.left + padding, rect.bottom, textWidth, rect.height };
	Texture texture{ text, "DIN-Light.otf", charWidth, color };
	texture.Draw(drawRect);
}

void UI::DrawScore(std::int64_t score, int multiplier, float drawHeight) const
{
	float dividerPercentage{ 0.2f };
	//Draw multiplier
	int maxMultiplierDigitWidth{ int(m_Viewport.width / 18.f) };
	float maxMultiplierWidth{ m_Viewport.width * dividerPercentage };
	Rectf multiplierRect{ 0.f, 0.f, maxMultiplierWidth, drawHeight / 1.5f };
	DrawStringInRect('X' + std::to_string(multiplier), multiplierRect, maxMultiplierDigitWidth, Color4f{0.851f, 0.616f, 0.145f, 1.f});
	
	//Draw score
	int maxScoreDigitWidth{ int(m_Viewport.width / 9.f) };
	float maxScoreWidth{ m_Viewport.width * (1.f - 2 * dividerPercentage) };
	Rectf scoreRect{ m_Viewport.width * dividerPercentage, 0.f, maxScoreWidth, drawHeight };
	DrawStringInRect(std::to_string(score), scoreRect, maxScoreDigitWidth, Color4f{ 1.f, 1.f, 1.f, 1.f });
}

void UI::DrawLives(int lives, int maxLives, float percOfLastLifeRemaining, float drawHeight) const
{
	float padding{ m_Viewport.width * 0.05f };
	float lifeWidth{ (m_Viewport.width - padding * 2) / maxLives };

	//Draw the lives
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	for (int i{}; i < lives - 1; ++i)
	{
		float leftPos{ padding + i * lifeWidth };
		utils::FillRect(leftPos, 0.f, lifeWidth, drawHeight);
	}
	//Draw the last life
	if (lives > 0)
	{
		float lastLifeLeftPos{ padding + (lives - 1) * lifeWidth };
		float lastLifeWidth{ percOfLastLifeRemaining * lifeWidth };
		utils::FillRect(lastLifeLeftPos, 0.f, lastLifeWidth, drawHeight);
	}

	//Draw box for each life
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	for (int i{}; i < maxLives; ++i)
	{
		float leftPos{ padding + i * lifeWidth };
		utils::DrawRect(leftPos, 0.f, lifeWidth, drawHeight);
	}
}
