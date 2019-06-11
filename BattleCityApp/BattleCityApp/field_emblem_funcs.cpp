#include "field.h"
#include "general.hpp"

void GameField::CreateEmblem()
{
	emblem.setPosObj(128.f, 208.f);
	emblem.setPosFrame(
		emblem.takeObj().getPosition().x,
		emblem.takeObj().getPosition().y
	);
	return;
}
