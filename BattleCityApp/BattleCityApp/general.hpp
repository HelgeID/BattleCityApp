﻿//FINAL
#pragma once
#define TITLE "BattleCity"

extern bool p_zoom;
extern bool p_no_sound;
extern int p_level; // 1, 2, 3 , ...
extern int p_player; //1 or 2

extern unsigned int life_counter1;
extern unsigned int star_counter1;
extern unsigned int life_counter2;
extern unsigned int star_counter2;
extern bool save_counter1;
extern bool save_counter2;

//test
extern bool p_showframe;
extern bool p_showfps;

//consts
extern int SizeCell;
extern float SizeField;

//signals
extern bool KeyActive;
extern bool Key_A;
extern bool Key_D;
extern bool Key_W;
extern bool Key_S;
extern bool Key_Left;
extern bool Key_Right;
extern bool Key_Up;
extern bool Key_Down;

extern bool Key_F;
extern bool Key_Ctrl; //RControl
extern bool Key_Space;
extern bool Key_Return;

//parameters of the game
extern bool undying_enemy;
extern bool undying_players;
extern bool undying_hit_on_player;
extern bool blocking_firing;
extern bool blocking_hit_for_enemy;

extern bool undying_emblem_hit_by_tank;
extern bool undying_emblem_boom_by_tank;
extern bool undying_emblem_absence_players;

extern bool f_f;//for optimality

extern bool gameover;
extern bool level_finish;
extern bool level_exit;
extern bool pause;
extern bool no_close;
