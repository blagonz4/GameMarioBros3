#include "Card.h"

Card::Card()
{

}
void Card::Render()
{
	animation_set->at(0)->Render(x, y);
}
void Card::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = t = r = b = 0;
}


Card::~Card()
{
}
