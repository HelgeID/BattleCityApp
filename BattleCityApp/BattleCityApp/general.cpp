#include "general.hpp"

bool p_zoom = false;
bool p_no_sound = false;
int p_level = 1;
int p_player = 1;

unsigned int life_counter1 = 3;
unsigned int star_counter1 = 0;
unsigned int life_counter2 = 3;
unsigned int star_counter2 = 0;

bool p_showframe = false;
bool p_showfps = false;

int SizeCell = 16;
float SizeField = 208.f;

bool KeyActive = false;
bool Key_A = false;
bool Key_D = false;
bool Key_W = false;
bool Key_S = false;
bool Key_Left = false;
bool Key_Right = false;
bool Key_Up = false;
bool Key_Down = false;

bool Key_F = false;
bool Key_Ctrl = false;
bool Key_Space = false;
bool Key_Return = false;

bool undying_enemy = false;
bool undying_players = false;
bool undying_hit_on_player = false;
bool blocking_firing = false;
bool blocking_hit_for_enemy = true;

bool undying_emblem_hit_by_tank = false;
bool undying_emblem_boom_by_tank = false;
bool undying_emblem_absence_players = false;


bool f_f = false;

bool gameover = false; //default =>FALSE
bool level_finish = false; //default =>FALSE
bool level_exit = false; //default =>FALSE
bool pause = false; //default =>FALSE
bool no_close = true; //default =>TRUE
