#include "ColorBlock.h"


ColorBlock::ColorBlock(float w, float h)
{

	this->width = w;
	this->height = h;

}
void ColorBlock::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y;
}
