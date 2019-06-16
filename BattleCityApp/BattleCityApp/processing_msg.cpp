#include "field.h"
#include "general.hpp"

void GameField::StartGameOverMSG() { uiGameOverMSG.GameOverMSG(); return; }

void GameField::StartPauseMSG() { uiPauseMSG.PauseMSG(true); return; }

void GameField::StopPauseMSG() { uiPauseMSG.PauseMSG(false); return; }
