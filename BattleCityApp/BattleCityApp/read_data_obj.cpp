//FINAL
//READDATAOBJ

//The data should be as:
//1) position on the map
//2) texture coordinates


/* SUMM = 79 elem. : TileDynamic::TileDynamic
Obj: (6) tanks, (2) players
Anim: (6) BirthTank, (6) SkinTank, (2) BirthPl, (2) SkinPl
BoomAnim: (8) tankBoom, (8) bulletBoom
UITank: (20)
UINumber, UIFlag, UIPlayer, UImPlayer, UITank: (19)
*/

#include "field.h"

template <typename T>
void TakeDataObj(T& obj, TileCoords& data, int& index)
{
	data.mapCoords = obj.getPosObj();
	data.texCoords =
	{
		obj.takeObj().getTextureRect().left,
		obj.takeObj().getTextureRect().top
	};
	index++;
	return;
}

template <typename T>
void TakeDataAnim(T& obj, TileCoords& data, int& index)
{
	const std::size_t cf = obj.TakeAnim().GetCurrentFrame();
	data.mapCoords = obj.TakeAnim().getPosition();
	data.texCoords =
	{
		obj.TakeAnim().GetAnimation()->GetFrame(cf).left,
		obj.TakeAnim().GetAnimation()->GetFrame(cf).top
	};
	index++;
	return;
}

void TakeDataDefault(TileCoords& data, int& index, const sf::Vector2u textureSize)
{
	data.mapCoords = { 0.f, 0.f };
	data.texCoords = { int(textureSize.x - 16), int(textureSize.y - 16) };
	index++;
	return;
}

void READDATAOBJ(GameField* gFieldPtr)
{
	sf::Vector2u textureSize = gFieldPtr->texture.getSize();

	//form the data structure
	TileCoords data;
	int index(0);

	auto SetSprite = [&](const float size_TILE = 0.f) { gFieldPtr->tDynamic.setSprite(index - 1, data.mapCoords, data.texCoords, size_TILE); }; //var: NULL - by default (16px)

	//for obj tanks
	{
		std::for_each(gFieldPtr->tank.begin(), gFieldPtr->tank.end(), [&](Tank &tank)
		{
			tank.isTank() ? TakeDataObj<Tank>(tank, data, index) :
				TakeDataDefault(data, index, textureSize); SetSprite();
		});
	}

	//for obj players
	{
		gFieldPtr->firstPlayer->Presence() ? TakeDataObj<Player>(*gFieldPtr->firstPlayer, data, index) :
			TakeDataDefault(data, index, textureSize); SetSprite();
		gFieldPtr->secondPlayer->Presence() ? TakeDataObj<Player>(*gFieldPtr->secondPlayer, data, index) :
			TakeDataDefault(data, index, textureSize); SetSprite();
	}

	//for anims Birth && Skin of tanks
	{
		for (int animIndx(0); animIndx < 6; ++animIndx) {
			gFieldPtr->tankAnimArr[animIndx].tankBirth != nullptr ? TakeDataAnim<AnimBirth>(*gFieldPtr->tankAnimArr[animIndx].tankBirth.get(), data, index) :
				TakeDataDefault(data, index, textureSize); SetSprite();
			gFieldPtr->tankAnimArr[animIndx].tankSkin != nullptr ? TakeDataAnim<AnimSkin>(*gFieldPtr->tankAnimArr[animIndx].tankSkin.get(), data, index) :
				TakeDataDefault(data, index, textureSize); SetSprite();
		}
	}

	//for anims Birth && Skin of players
	{
		gFieldPtr->firstPlayerAnim.playerBirth != nullptr && !gFieldPtr->firstPlayerAnim.playerBirth->FinishTime() ? 
			TakeDataAnim<AnimBirth>(*gFieldPtr->firstPlayerAnim.playerBirth.get(), data, index) :
			TakeDataDefault(data, index, textureSize); SetSprite();
		gFieldPtr->secondPlayerAnim.playerBirth != nullptr && !gFieldPtr->secondPlayerAnim.playerBirth->FinishTime() ? 
			TakeDataAnim<AnimBirth>(*gFieldPtr->secondPlayerAnim.playerBirth.get(), data, index) :
			TakeDataDefault(data, index, textureSize); SetSprite();

		gFieldPtr->firstPlayerAnim.playerSkin != nullptr && !gFieldPtr->firstPlayerAnim.playerSkin->FinishTime() ? 
			TakeDataAnim<AnimSkin>(*gFieldPtr->firstPlayerAnim.playerSkin.get(), data, index) :
			TakeDataDefault(data, index, textureSize); SetSprite();
		gFieldPtr->secondPlayerAnim.playerSkin != nullptr && !gFieldPtr->secondPlayerAnim.playerSkin->FinishTime() ? 
			TakeDataAnim<AnimSkin>(*gFieldPtr->secondPlayerAnim.playerSkin.get(), data, index) :
			TakeDataDefault(data, index, textureSize); SetSprite();
	}

	//for anims Booms
	{
		const size_t bulletBoomSize(sizeof(gFieldPtr->bulletBoom) / sizeof(*gFieldPtr->bulletBoom));
		for (int animIndx(0); animIndx < bulletBoomSize; ++animIndx) {
			(gFieldPtr->bulletBoom[animIndx] != nullptr) ? TakeDataAnim<AnimBoom>(*gFieldPtr->bulletBoom[animIndx].get(), data, index) :
				TakeDataDefault(data, index, textureSize); SetSprite();
		}
		const size_t tankBoomSize(sizeof(gFieldPtr->tankBoom) / sizeof(*gFieldPtr->tankBoom));
		for (int animIndx(0); animIndx < tankBoomSize; ++animIndx) {
			(gFieldPtr->tankBoom[animIndx] != nullptr) ? TakeDataAnim<AnimBoom>(*gFieldPtr->tankBoom[animIndx].get(), data, index) :
				TakeDataDefault(data, index, textureSize); SetSprite();
		}
	}

	//for ui tanks
	{
		const size_t uiTankSize(gFieldPtr->storage_tanks.TakeArrUITankSize());
		for (int uiIndexTank(0); uiIndexTank < uiTankSize; ++uiIndexTank) {
			if (gFieldPtr->curtain == nullptr) {
				TakeDataObj<UITank>(gFieldPtr->storage_tanks.TakeArrUITank()[uiIndexTank], data, index); SetSprite(9.f);
			}
			else
				index++;
		}

		if (gFieldPtr->curtain == nullptr) {
			TakeDataObj<UIPlayer>(*gFieldPtr->number_lifes_first.uiPlayer, data, index); SetSprite();
			TakeDataObj<UINumber>(*gFieldPtr->number_lifes_first.uiNumber, data, index); SetSprite(8.f);
			TakeDataObj<UIPlayer>(*gFieldPtr->number_lifes_second.uiPlayer, data, index); SetSprite();
			TakeDataObj<UINumber>(*gFieldPtr->number_lifes_second.uiNumber, data, index); SetSprite(8.f);

			TakeDataObj<UIFlag>(*gFieldPtr->number_flags.uiFlag, data, index); SetSprite();
			TakeDataObj<UINumber>(*gFieldPtr->number_flags.uiNumber1, data, index); SetSprite(8.f);
			TakeDataObj<UINumber>(*gFieldPtr->number_flags.uiNumber2, data, index); SetSprite(8.f);

			TakeDataObj<UITank>(*gFieldPtr->numberAllTanks.uitank, data, index); SetSprite(9.f);
			TakeDataObj<UINumber>(*gFieldPtr->numberAllTanks.uiNumber1, data, index); SetSprite(8.f);
			TakeDataObj<UINumber>(*gFieldPtr->numberAllTanks.uiNumber2, data, index); SetSprite(8.f);
			TakeDataObj<UINumber>(*gFieldPtr->numberAllTanks.uiNumber3, data, index); SetSprite(8.f);
			
			TakeDataObj<UImPlayer>(*gFieldPtr->numberTanksForFirstPlayer.uimPlayer, data, index); SetSprite(9.f);
			TakeDataObj<UINumber>(*gFieldPtr->numberTanksForFirstPlayer.uiNumber1, data, index); SetSprite(8.f);
			TakeDataObj<UINumber>(*gFieldPtr->numberTanksForFirstPlayer.uiNumber2, data, index); SetSprite(8.f);
			TakeDataObj<UINumber>(*gFieldPtr->numberTanksForFirstPlayer.uiNumber3, data, index); SetSprite(8.f);

			TakeDataObj<UImPlayer>(*gFieldPtr->numberTanksForSecondPlayer.uimPlayer, data, index); SetSprite(9.f);
			TakeDataObj<UINumber>(*gFieldPtr->numberTanksForSecondPlayer.uiNumber1, data, index); SetSprite(8.f);
			TakeDataObj<UINumber>(*gFieldPtr->numberTanksForSecondPlayer.uiNumber2, data, index); SetSprite(8.f);
			TakeDataObj<UINumber>(*gFieldPtr->numberTanksForSecondPlayer.uiNumber3, data, index); SetSprite(8.f);
		}
		else
			index+=19;
	}
}
