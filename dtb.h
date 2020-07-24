#ifndef DTB_H
#define DTB_H

#include "render.h"
#include "ui.h"

#include "physics.h"
#include "collision.h"
#include "entity.h"
#include "particle.h"

enum game_state
{
	STATE_MAIN_MENU,
	STATE_MAIN_LOBBY,
	STATE_OPTIONS,
	STATE_OPTIONS_KEYBIND,
	STATE_HELP,
	STATE_PROFILE_SELECTION,
	STATE_PROFILE_CREATION,
	STATE_UPGRADES,
	STATE_MC_DIFF_SELECT,
	STATE_MODE_CLASSIC,
	STATE_GAMEOVER,
	STATE_PAUSED,
	STATE_DEBUG,
};

enum game_sfx
{
	SFX_MENU_CLICK,
	SFX_MENU_BACK,
	SFX_ROLLOVER,
	SFX_SWITCH,
	SFX_PLAYER_HIT,
	SFX_POWERUP,
	SFX_LASER,
	SFX_ENEMY_HIT,
	SFX_ENEMY_DEAD,
	SFX_ENTER_CONDITION,
	SFX_CONDITION_OVER,
	SFX_MAX,
};

enum game_music
{
	MUS_BACKGROUND,
	MUS_GAMEOVER,
	MUS_MAX,
};

enum game_difficulty
{
	DIFFICULTY_EASY,
	DIFFICULTY_NORMAL,
	DIFFICULTY_HARD,
};

enum game_menu_button
{
	MENU_BUTTON_START,
	MENU_BUTTON_OPTIONS,
	MENU_BUTTON_QUIT,
	MENU_BUTTON_HELP,
	MENU_BUTTON_PROFILE,
	MENU_BUTTON_MAX,
};

enum game_lobby_button
{
	LOBBY_BUTTON_PLAY,
	LOBBY_BUTTON_OPTIONS,
	LOBBY_BUTTON_UPGRADES,
	LOBBY_BUTTON_BACK,
	LOBBY_BUTTON_MAX,
};

enum game_pause_button
{
	PAUSE_BUTTON_RETRY,
	PAUSE_BUTTON_CONTINUE,
	PAUSE_BUTTON_BACK,
	PAUSE_BUTTON_MAX,
};

enum game_help_text
{
	HELP_TEXT_TITLE_1,
	HELP_TEXT_SUBTITLE_1,
	HELP_TEXT_BODY_1,
	HELP_TEXT_BODY_2,
	HELP_TEXT_BODY_3,
	HELP_TEXT_BODY_4,
	HELP_TEXT_BODY_5,
	HELP_TEXT_BODY_6,
	HELP_TEXT_MAX,
};

enum mc_diff_select_button
{
	MC_DIFF_BUTTON_BACK,
	MC_DIFF_BUTTON_LEFT,
	MC_DIFF_BUTTON_RIGHT,
	MC_DIFF_BUTTON_START,
	MC_DIFF_BUTTON_MAX,
};

struct player_data
{
	real32 ModeClassicHighScore;
	real32 ScoreRequirementForLeveling;
	bool LevelUP;
};

enum game_keybind
{
	KEYBIND_UP,
	KEYBIND_DOWN,
	KEYBIND_LEFT,
	KEYBIND_RIGHT,
	KEYBIND_DASH,
	KEYBIND_SHRINK,
	KEYBIND_SHOOT,
	KEYBIND_MAX,
};

struct game_input_map
{
	union
	{
		game_button_state Keys[KEYBIND_MAX];
		struct
		{
			game_button_state Up;
			game_button_state Down;
			game_button_state Left;
			game_button_state Right;
			game_button_state Dash;
			game_button_state Shrink;
			game_button_state Shoot;
		};
	};
	
};

struct game_ui
{	
	render_text GameLogo;
	
	button LobbyButtons[LOBBY_BUTTON_MAX];
	
	button MenuButtons[MENU_BUTTON_MAX];
	button PauseButtons[PAUSE_BUTTON_MAX];
	
	render_text HelpText[HELP_TEXT_MAX];
	button HelpBackButton;
	
	message_box LevelUPMsg;

	render_text GameOverText;
	render_text ContinueText;
	
	message_box QuitConfirmMessageBox;
};

struct game_sound
{
	Mix_Chunk* SFX[SFX_MAX];
	Mix_Music* Music[MUS_MAX];
};

real32 GetScoreRequitementForLeveling(int Level);

#include "skin.h"
#include "game_upgrade.h"
#include "game_profile.cpp"
#include "game_option.cpp"
#include "game_upgrade.cpp"
#include "game_mode_classic.cpp"

struct game_core
{
	game_renderer Renderer;
	game_input_map InputMap;
	
	game_skin Skin;
	
	game_ui UI;
	game_option OptionMenu;
	upgrade_menu UpgradeMenu;
	player_profile_status ProfileStatus;
	profile_selection ProfileSelection;
	profile_creation ProfileCreation;
	game_background_scroll BackgroundScroll;
	
	game_state State;
	game_state LastState;
	
	player_profile Profile;
	player_data PlayerData;
	game_mode_classic ModeClassic;
	
	game_sound Audio;
	
	bool Running;
};

game_core InitGame(game_state State, render* Graphics);
void InitGameUI(game_core* Game, render* Graphics);

void MainGame(game_core* Game, game_input* Input, render* Graphics);
void UpdateGameState(game_core* Game, game_state NewState);

#endif