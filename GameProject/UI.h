#pragma once
#include "Texture.h"

class UI
{
public:
	UI(const Rectf& viewport);
	~UI();

	void Draw(std::int64_t score, int multiplier, int lives, int maxLives, float percOfLastLifeRemaining) const;
	float GetHeight() const;
private:
	void DrawStringInRect(const std::string& text, const Rectf& rect, int maxCharWidth, const Color4f& color) const;
	void DrawScore(std::int64_t score, int multiplier, float drawHeight) const;
	void DrawLives(int lives, int maxLives, float percOfLastLifeRemaining, float drawHeight) const;

	const Rectf& m_Viewport;
	std::vector<Texture*> m_pNumberTextures;

	float m_LivesBarHeight;
	float m_ScoreBarHeight;
};

