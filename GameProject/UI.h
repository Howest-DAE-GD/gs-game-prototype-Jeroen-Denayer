#pragma once
#include "Texture.h"

class UI
{
public:
	UI(const Rectf& viewport);
	~UI();

	void Draw() const;
	void IncreaseScore(int addedScore);
private:
	const Rectf& m_Viewport;
	std::int64_t m_Score;
	std::vector<Texture*> m_pNumberTextures;
	float m_MaxDigitWidth;
	float m_MaxDigitHeight;
};

