#pragma once
#include "Texture.h"

class UI
{
public:
	UI(const Rectf& viewport);
	~UI();

	void Draw(int score, int lives, int maxLives, float percOfLastLifeRemaining) const;
	float GetHeight() const;
private:
	void DrawScore(int score, float drawHeight) const;
	void DrawLives(int lives, int maxLives, float percOfLastLifeRemaining, float drawHeight) const;

	const Rectf& m_Viewport;
	std::int64_t m_Score;
	std::vector<Texture*> m_pNumberTextures;

	float m_LivesBarHeight;
	float m_ScoreBarHeight;
};

