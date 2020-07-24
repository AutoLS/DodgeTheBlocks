#ifndef GAME_OPTION_H
#define GAME_OPTION_H

enum game_option_text
{
	OPTION_TEXT_TITLE,
	OPTION_TEXT_SUBTITLE_1,
	OPTION_TEXT_SUBTITLE_2,
	OPTION_TEXT_SUBTITLE_3,
	OPTION_TEXT_SUBTITLE_4,
	OPTION_TEXT_SUBTITLE_5,
	OPTION_TEXT_SUBTITLE_6,
	OPTION_TEXT_SUBTITLE_7,
	OPTION_TEXT_SUBTITLE_8,
	OPTION_TEXT_SUBTITLE_9,
	OPTION_TEXT_MAX,
};

enum game_option_checkbox
{
	OPTION_CHECKBOX_SCROLL,
	OPTION_CHECKBOX_FPS,
	OPTION_CHECKBOX_MAX,
};

enum game_option_slide_bar
{
	OPTION_SLIDEBAR_MASTER_VOL,
	OPTION_SLIDEBAR_MUSIC_VOL,
	OPTION_SLIDEBAR_SFX_VOL,
	OPTION_SLIDEBAR_MAX,
};

enum game_option_button
{
	OPTION_BUTTON_BACK,
	OPTION_BUTTON_KEYBIND,
	OPTION_BUTTON_MAX,
};

enum option_keybind_text
{
	KEYBIND_TEXT_TITLE,
	KEYBIND_TEXT_SUBTITLE_1,
	KEYBIND_TEXT_SUBTITLE_2,
	KEYBIND_TEXT_SUBTITLE_3,
	KEYBIND_TEXT_SUBTITLE_4,
	KEYBIND_TEXT_SUBTITLE_5,
	KEYBIND_TEXT_SUBTITLE_6,
	KEYBIND_TEXT_SUBTITLE_7,
	KEYBIND_TEXT_MAX,
};

struct option_config
{
	bool BackgroundScroll;
	bool DisplayFPS;
	char* SelectedSkin;
	int FrameLimit;
	
	int MasterVolume;
	int MusicVolume;
	int SFXVolume;
	
	char* Profile;
};

struct option_keybind
{
	union
	{
		uint8 Keys[KEYBIND_MAX];
		struct
		{
			uint8 Up;
			uint8 Down;
			uint8 Left;
			uint8 Right;
			uint8 Dash;
			uint8 Shrink;
			uint8 Shoot;
		};
	};
};

struct keybind_menu
{
	bool IsKeybinding;
	
	button Buttons[KEYBIND_MAX];
	render_text Text[KEYBIND_TEXT_MAX];	
	render_text KeyBindText[KEYBIND_MAX];
	
	button BackButton;
	
	game_keybind CurrentBinding;
};

struct game_option
{
	option_config Config;
	option_keybind KeyBind;
	game_state OptionEnterState;
	
	button Buttons[OPTION_BUTTON_MAX];
	check_box Checkbox[OPTION_CHECKBOX_MAX];
	slide_bar SlideBar[OPTION_SLIDEBAR_MAX];
	render_text Text[OPTION_TEXT_MAX];
	
	drop_menu SkinSelection;
	drop_menu FrameLimit;
	
	keybind_menu KeybindMenu;
	render_text KeyBindText;
	
	scroll_bar ScrollBar;
};

void SaveOptionConfig(option_config* Config, 
					  game_skin* Skin, 
					  player_profile* Profile);
option_config LoadOptionConfig(render* Graphics);

void SaveKeybindConfig(option_keybind* KeyBind);
option_keybind LoadKeyBind(char* Filename);

keybind_menu InitKeybindMenu(render* Graphics, game_renderer* Renderer,
							 option_keybind* Keybind);
void UpdateKeybindMenu(game_option* OptionMenu, 
					   game_sound* Audio,
					   game_input* Input,
					   game_renderer* Renderer);

game_option InitOptionMenu(option_config Config, 
						   game_skin* Skin,
						   game_renderer* Renderer,
						   render* Graphics);
void UpdateOptionMenu(game_option* OptionMenu, 
					  game_sound* Audio,
					  game_skin* Skin,
					  game_background_scroll* BackgroundScroll,
					  game_input* Input, 
					  game_renderer* Renderer,
					  render* Graphics);
void RenderOptionMenu(game_option* OptionMenu, 
					  game_renderer* Renderer, 
					  render* Graphics);

#endif