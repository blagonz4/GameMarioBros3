#include "Font.h"

Font::Font()
{
	Tex = CTextures::GetInstance()->Get(TEXID_FONT);
}
void Font::Draw(float x, float y, const string& s) {
	for (UINT i = 0; i < s.size(); i++)
	{
		if (s[i] == ' ')
			continue;
		
		if (s[i] == '!') {
			LPSPRITE spriteTile = new CSprite(99, 208, 0, 212, 8, Tex);
			spriteTile->Draw(x + i * TILE_FONT_WIDTH, y);
		}

		if (s[i] >= 'A' && s[i] <= 'Z')
		{
			int id = s[i] - 'A';
			int left = id % 26 * TILE_FONT_WIDTH;
			int top = id / 26 * TILE_FONT_A_HEIGHT;
			int right = left + TILE_FONT_WIDTH;
			int bottom = top + TILE_FONT_A_HEIGHT;
			LPSPRITE SpriteTile = new CSprite(id, left, top, right, bottom, Tex);
			SpriteTile->Draw(x + i * TILE_FONT_WIDTH, y);
		}

		else if (s[i] >= '0' && s[i] <= '9')
		{
			int id = s[i] - '0' + 26;
			int left = id % 26 * TILE_FONT_WIDTH;
			int top = (id / 26 * TILE_FONT_NUMBER_HEIGHT) + 1;
			int right = left + TILE_FONT_WIDTH;
			int bottom = top + TILE_FONT_NUMBER_HEIGHT;
			LPSPRITE SpriteTile = new CSprite(id, left, top, right, bottom, Tex);
			SpriteTile->Draw(x + i * TILE_FONT_WIDTH, y);
		}
	}

}

Font::~Font()
{
}
