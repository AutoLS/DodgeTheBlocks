#include "game_option.h"

void SaveOptionConfig(option_config* Config, 
					  game_skin* Skin, 
					  player_profile* Profile)
{
	SDL_RWops* File = SDL_RWFromFile("options.cfg", "w+");
	if(File)
	{
		char Buffer[255] = {};
		stbsp_sprintf(Buffer, "Skin=%s\r\n", Skin->SelectedSkin);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "BackgroundScroll=%d\r\n", 
					  Config->BackgroundScroll);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "ShowFPS=%d\r\n", Config->DisplayFPS);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "TargetFramerate=%d\r\n", Config->FrameLimit);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "MasterVolume=%d\r\n", Config->MasterVolume);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "MusicVolume=%d\r\n", Config->MusicVolume);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "SFXVolume=%d\r\n", Config->SFXVolume);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "Profile=%s\r\n", Profile->Name);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		SDL_RWclose(File);
	}
}

option_config LoadOptionConfig(render* Graphics)
{
	option_config Config = {};
	Config.SelectedSkin = (char*)malloc(100);
	Config.FrameLimit = (int)Graphics->Display.GameUpdateHZ;
	Config.MasterVolume = 100;
	Config.MusicVolume = 50;
	Config.SFXVolume = 50;
	Config.Profile = (char*)malloc(100);
	
	debug_file ConfigFile = ReadEntireFile("options.cfg", "r");
	
	if(ConfigFile.Data)
	{
		attribute SkinConfig = GetAttribute((char*)ConfigFile.Data, "Skin");
		GetAttributeValue_s(&SkinConfig, Config.SelectedSkin);
		
		attribute ScrollConfig = 
		GetAttribute((char*)ConfigFile.Data, "BackgroundScroll");
		Config.BackgroundScroll = GetAttributeValue_b(&ScrollConfig);
		
		attribute FPSConfig = GetAttribute((char*)ConfigFile.Data, "ShowFPS");
		Config.DisplayFPS = GetAttributeValue_b(&FPSConfig);
		
		attribute FrameLimitConfig = 
		GetAttribute((char*)ConfigFile.Data, "TargetFramerate");
		Config.FrameLimit = GetAttributeValue_i(&FrameLimitConfig, 
							(int)Graphics->Display.GameUpdateHZ);
		
		attribute MasterVolume = 
		GetAttribute((char*)ConfigFile.Data, "MasterVolume");
		Config.MasterVolume = Min(GetAttributeValue_i(&MasterVolume), 100, 100);
		
		attribute MusicVolume = 
		GetAttribute((char*)ConfigFile.Data, "MusicVolume");
		Config.MusicVolume = Min(GetAttributeValue_i(&MusicVolume), 100, 100);
		
		attribute SFXVolume = 
		GetAttribute((char*)ConfigFile.Data, "SFXVolume");
		Config.SFXVolume = Min(GetAttributeValue_i(&SFXVolume), 100, 100);
		
		attribute ProfileConfig = 
		GetAttribute((char*)ConfigFile.Data, "Profile");
		GetAttributeValue_s(&ProfileConfig, Config.Profile);
		
		FreeFileFromMemory(&ConfigFile);
	}
	else
	{
		strcpy(Config.SelectedSkin, "Default");
		strcpy(Config.Profile, "Default");
		printf("Unable to find and open the file. Creating a new file...\n");
		SDL_RWops* File = SDL_RWFromFile("options.cfg", "w+");
		if(File)
		{
			char Buffer[255] = {};
			stbsp_sprintf(Buffer, "Skin=%s\r\n", Config.SelectedSkin);
			SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
			stbsp_sprintf(Buffer, "BackgroundScroll=%d\r\n", Config.BackgroundScroll);
			SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
			stbsp_sprintf(Buffer, "ShowFPS=%d\r\n", Config.DisplayFPS);
			SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
			stbsp_sprintf(Buffer, "TargetFramerate=%d\r\n", Config.FrameLimit);
			SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
			stbsp_sprintf(Buffer, "MasterVolume=%d\r\n", Config.MasterVolume);
			SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
			stbsp_sprintf(Buffer, "MusicVolume=%d\r\n", Config.MusicVolume);
			SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
			stbsp_sprintf(Buffer, "SFXVolume=%d\r\n", Config.SFXVolume);
			SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
			stbsp_sprintf(Buffer, "Profile=%s\r\n", Config.Profile);
			SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
			SDL_RWclose(File);
		}
	}

	return Config;
}

void SaveKeybindConfig(option_keybind* KeyBind)
{
	SDL_RWops* File = SDL_RWFromFile("KeyBind.cfg", "w+");
	if(File)
	{
		char Buffer[255] = {};
		stbsp_sprintf(Buffer, "Up=%d\r\n", KeyBind->Up);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "Down=%d\r\n", KeyBind->Down);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "Left=%d\r\n", KeyBind->Left);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "Right=%d\r\n", KeyBind->Right);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "Dash=%d\r\n", KeyBind->Dash);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "Shrink=%d\r\n", KeyBind->Shrink);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		stbsp_sprintf(Buffer, "Shoot=%d\r\n", KeyBind->Shoot);
		SDL_RWwrite(File, (void*)Buffer, strlen(Buffer), 1);
		SDL_RWclose(File);
	}
}

option_keybind LoadKeyBind(char* Filename)
{
	option_keybind KeyBind = 
	{
		KEY_UP, 
		KEY_DOWN, 
		KEY_LEFT, 
		KEY_RIGHT, 
		KEY_LSHIFT, 
		KEY_LCTRL,
		KEY_Z
	};
	
	char* Action[KEYBIND_MAX] =
	{
		"Up",
		"Down",
		"Left",
		"Right",
		"Dash",
		"Shrink",
		"Shoot"
	};
	
	debug_file KeyBindConfig = ReadEntireFile(Filename, "r");
	
	if(KeyBindConfig.Data)
	{	
		for(int i = 0; i < KEYBIND_MAX; ++i)
		{
			attribute Key = GetAttribute((char*)KeyBindConfig.Data, Action[i]);
			KeyBind.Keys[i] = GetAttributeValue_i(&Key, KeyBind.Keys[i]);
		}
		
		FreeFileFromMemory(&KeyBindConfig);
	}
	else
	{
		printf("Unable to find and open the file. Creating a new file...\n");
		SaveKeybindConfig(&KeyBind);
	}
	
	return KeyBind;
}

keybind_menu InitKeybindMenu(render* Graphics, game_renderer* Renderer,
							 option_keybind* Keybind)
{
	v2 WinDim = V2(Graphics->Dim);
	
	keybind_menu KeybindMenu = {};
	char* Text[KEYBIND_TEXT_MAX] =
	{
		"KEY BINDING",
		"UP",
		"DOWN",
		"LEFT",
		"RIGHT",
		"DASH",
		"SHRINK",
		"SHOOT"
	};
	KeybindMenu.Text[KEYBIND_TEXT_TITLE] =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
				   Text[KEYBIND_TEXT_TITLE],
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &KeybindMenu.Text[KEYBIND_TEXT_TITLE], 
					 V2(15, 15), TEXT_POSITION_TOP_LEFT);
					 
	for(int Index = KEYBIND_TEXT_SUBTITLE_1;
		Index < KEYBIND_TEXT_MAX;
		++Index)
	{
		KeybindMenu.Text[Index] =
		LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
					   Text[Index],
					   Renderer->RenderObjects[R_OBJECT_RECT]);
		SetRenderTextPos(Graphics, &KeybindMenu.Text[Index], 
						 V2(15, 100 + ((Index-1) * 75.0f)), 
						 TEXT_POSITION_TOP_LEFT);
	}
	
	v4 DefaultColor = Color(150, 150, 150, 0.5f);
	v4 HoverColor = Color(0, 255, 255, 0.5f);
	
	for(int Index = 0;
		Index < KEYBIND_MAX;
		++Index)
	{
		KeybindMenu.Buttons[Index] =
		CreateButton(UI_BUTTON_UNTEXTURED, 0, 0, {},
					 DefaultColor,
					 HoverColor,
					 GetPos(V2(-200, 100 + (Index * 75.0f) + 15), WinDim, 
							POSITION_TOP_RIGHT), 
					 V2(300, 50));
		
		char* KeyText = GetKeyText(Keybind->Keys[Index]);
		
		KeybindMenu.KeyBindText[Index] =
		LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
					   KeyText,
					   Renderer->RenderObjects[R_OBJECT_RECT]);
		SetRenderTextPos(&KeybindMenu.KeyBindText[Index],
						 &KeybindMenu.Buttons[Index].Rect);
	}
	
	render_text BackText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], 
				   "BACK",
				   Renderer->RenderObjects[R_OBJECT_RECT]); 
	SetRenderTextPos(Graphics, &BackText, V2(15, -15), 
					 TEXT_POSITION_BOTTOM_LEFT);
	KeybindMenu.BackButton =
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, BackText);
	
	return KeybindMenu;
}

void UpdateKeybindMenu(game_option* OptionMenu, 
					   game_sound* Audio,
					   game_input* Input,
					   game_renderer* Renderer)
{
	keybind_menu* Menu = &OptionMenu->KeybindMenu;
	option_keybind* KeyBind = &OptionMenu->KeyBind;
	
	HandleButton(&Menu->BackButton, Input, Audio->SFX[SFX_MENU_BACK]);
	
	if(!Menu->IsKeybinding)
	{
		for(int ButtonIndex = 0;
			ButtonIndex < KEYBIND_MAX; 
			++ButtonIndex)
		{
			HandleButton(&Menu->Buttons[ButtonIndex], Input);
			if(Menu->Buttons[ButtonIndex].State ==
			   UI_BUTTON_STATE_CLICK_L)
			{
				Menu->Buttons[ButtonIndex].RenderColor = 
				Menu->Buttons[ButtonIndex].HoverColor;
				Menu->CurrentBinding = (game_keybind)ButtonIndex;
				Menu->IsKeybinding = true;
				Menu->Buttons[ButtonIndex].State =
				UI_BUTTON_STATE_NONE;
			}
		}
	}
	
	if(Menu->IsKeybinding)
	{
		for(int KeyIndex = 0;
			KeyIndex < KEY_MAX;
			++KeyIndex)
		{
			if(Input->Keyboard.Keys[KeyIndex].EndedDown && KeyIndex != KEY_ESC)
			{
				char* KeyText = GetKeyText(KeyIndex);
				
				KeyBind->Keys[Menu->CurrentBinding] = KeyIndex;
				
				render_text* CurrentBindingText =
				&Menu->KeyBindText[Menu->CurrentBinding];
				
				UpdateRenderText(CurrentBindingText, 
				Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
				KeyText);
				
				Menu->Buttons[Menu->CurrentBinding].RenderColor = 
				Menu->Buttons[Menu->CurrentBinding].DefaultColor;
				Menu->IsKeybinding = false;
				break;
			}
		}
		if(!IsPointInRect(V2(Input->MouseState), 
			&Menu->Buttons[Menu->CurrentBinding].Rect))
		{
			if(Input->Mouse.Keys[BUTTON_LEFT].EndedDown)
			{
				Menu->Buttons[Menu->CurrentBinding].RenderColor = 
				Menu->Buttons[Menu->CurrentBinding].DefaultColor;
				Menu->IsKeybinding = false;
			}
		}
	}
}

game_option InitOptionMenu(option_config Config, 
						   game_skin* Skin,
						   game_renderer* Renderer,
						   render* Graphics)
{
	v2 WinDim = V2(Graphics->Dim);
	
	game_option OptionMenu = {};
	OptionMenu.Config = Config;
	
	OptionMenu.KeyBind = LoadKeyBind("KeyBind.cfg");
	OptionMenu.KeybindMenu = 
	InitKeybindMenu(Graphics, Renderer, &OptionMenu.KeyBind);
	
	render_text OptionBackText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], 
				   "BACK",
				   Renderer->RenderObjects[R_OBJECT_RECT]); 
	SetRenderTextPos(Graphics, &OptionBackText, V2(15, -15), 
					 TEXT_POSITION_BOTTOM_LEFT);
	OptionMenu.Buttons[OPTION_BUTTON_BACK] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, OptionBackText);
	
	char* OptionMenuText[OPTION_TEXT_MAX] =
	{
		"OPTIONS",
		"SKINS",
		"Enable background scrolling",
		"Enable FPS display",
		"Frame limit",
		"Key binds",
		"AUDIO",
		"Master",
		"Music",
		"SFX"
	};
	OptionMenu.Text[OPTION_TEXT_TITLE] = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], 
				   OptionMenuText[OPTION_TEXT_TITLE],
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &OptionMenu.Text[OPTION_TEXT_TITLE], 
					 V2(15, 15),
					 TEXT_POSITION_TOP_LEFT);
					 
	for(int i = OPTION_TEXT_SUBTITLE_1; 
		i < OPTION_TEXT_MAX;
		++i)
	{
		OptionMenu.Text[i] = 
		LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], 
					   OptionMenuText[i],
					   Renderer->RenderObjects[R_OBJECT_RECT]);
		SetRenderTextPos(Graphics, &OptionMenu.Text[i], 
						 V2(15, 100 + (i - OPTION_TEXT_SUBTITLE_1) * 50.0f),
						 TEXT_POSITION_TOP_LEFT);
	}
	
	//NOTE: Init skin selection menu
	v2 ItemDim = V2(300, 
	(real32)TTF_FontHeight(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM]));
	v4 DefaultColor = Color(150, 150, 150, 0.5f);
	v4 HoverColor = Color(0, 255, 255, 0.5f);
	v2 ExpandPos = GetPos(V2(-ItemDim.x * 0.5f - 30, 
						  OptionMenu.Text[OPTION_TEXT_SUBTITLE_1].Pos.y), 
						  WinDim,
						  POSITION_TOP_RIGHT);
	
	OptionMenu.SkinSelection = CreateDropMenu(ItemDim, Skin->List.Length+1, 
											  ExpandPos, Skin->SelectedSkin,
											  DefaultColor, HoverColor,
											  Renderer);
	{
		rect32 ExpandRect = OptionMenu.SkinSelection.Expand.Rect;
		real32 ViewHeight = 250;
		v2 ViewDim = V2(ExpandRect.Dim.x, ViewHeight);
		rect32 Rect = {ExpandRect.Pos + V2(0, ItemDim.y), ViewDim};
		OptionMenu.SkinSelection.Rect = Rect;
		for(int i = 0; i < Skin->List.Length; ++i)
		{
			render_text ButtonText = 
			LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], 
						   (char*)Skin->List.Data[i].c_str(), 
						   Renderer->RenderObjects[R_OBJECT_RECT]);
			SetRenderTextPos(&ButtonText, ViewDim, 
							 V2(10, i * ItemDim.y), TEXT_POSITION_TOP_LEFT);
			
			v2 Pos = V2(ExpandPos.x, 
						ExpandPos.y + ItemDim.y + (i * ItemDim.y));
			v2 RenderPos = V2(ItemDim.x * 0.5f, 
							  i * ItemDim.y + (ItemDim.y*0.5f));
			
			OptionMenu.SkinSelection.Items[i] = 
			CreateButton(UI_BUTTON_UNTEXTURED, 0, 0, 
					     ButtonText, 
					     DefaultColor, 
					     HoverColor,
					     Pos,
					     ItemDim,
					     RenderPos);
		}
		OptionMenu.SkinSelection.ScrollBar =
		CreateScrollBar(V2(ViewDim.x, 
						ItemDim.y * (OptionMenu.SkinSelection.ItemCount)), 
						Rect, 20);
	}
	{
		render_text ButtonText = 
		LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], 
					   "Open skin folder", 
					   Renderer->RenderObjects[R_OBJECT_RECT]);
		SetRenderTextPos(&ButtonText, OptionMenu.SkinSelection.Rect.Dim, 
						 V2(10, Skin->List.Length * ItemDim.y), 
						 TEXT_POSITION_TOP_LEFT);
		v2 Pos = V2(OptionMenu.SkinSelection.Expand.Pos.x, 
					OptionMenu.SkinSelection.Expand.Pos.y + 
					ItemDim.y + (Skin->List.Length * ItemDim.y));
		v2 RenderPos = V2(ItemDim.x * 0.5f, 
						  Skin->List.Length * ItemDim.y + (ItemDim.y*0.5f));
		
		OptionMenu.SkinSelection.Items[Skin->List.Length] = 
		CreateButton(UI_BUTTON_UNTEXTURED, 0, 0, ButtonText, 
					 DefaultColor, HoverColor,
					 Pos,
					 ItemDim,
					 RenderPos);
		OptionMenu.SkinSelection.ItemCount = Skin->List.Length+1;
	}
	
	//End of skin selection
	//NOTE: Init frame limit drop menu
	{
		string FPSLimit[4] =
		{
			"60",
			"144",
			"240",
			"Unlimited"
		};
		
		v2 ExpandPos = GetPos(V2(-ItemDim.x * 0.5f - 30, 
						  OptionMenu.Text[OPTION_TEXT_SUBTITLE_4].Pos.y), 
						  WinDim,
						  POSITION_TOP_RIGHT);
		
		char Buffer[20] = "Unlimited";
		if(Config.FrameLimit > 0)
		stbsp_sprintf(Buffer, "%d", Config.FrameLimit);
		
		OptionMenu.FrameLimit = CreateDropMenu(ItemDim, ArraySize(FPSLimit), 
											   ExpandPos, Buffer,
											   DefaultColor, HoverColor,
											   Renderer);
		InitDropMenuButtonsAndScrollbar(&OptionMenu.FrameLimit, 
										Renderer,
										DefaultColor, HoverColor, 
										ItemDim.y * ArraySize(FPSLimit),
										FPSLimit);
	}
	//End of frame limit
	
	OptionMenu.Checkbox[OPTION_CHECKBOX_SCROLL] = 
	CreateCheckBox(GetPos(V2(-50, 150 + 15), WinDim, POSITION_TOP_RIGHT), 
				   V2(30, 30),
				   Config.BackgroundScroll);
	
	OptionMenu.Checkbox[OPTION_CHECKBOX_FPS] = 
	CreateCheckBox(GetPos(V2(-50, 200 + 15), WinDim, POSITION_TOP_RIGHT), 
				   V2(30, 30),
				   Config.DisplayFPS);
	
	OptionMenu.Buttons[OPTION_BUTTON_KEYBIND] = 
	CreateButton(UI_BUTTON_UNTEXTURED, 0, 0, {},
				 DefaultColor,
				 HoverColor,
				 GetPos(V2(-180, OptionMenu.Text[OPTION_TEXT_SUBTITLE_5].Pos.y), 
						WinDim, POSITION_TOP_RIGHT),
				 ItemDim);
	
	OptionMenu.KeyBindText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], 
				   "Reset key bindings",
				   Renderer->RenderObjects[R_OBJECT_RECT],
				   false);
	SetRenderTextPos(&OptionMenu.KeyBindText, 
					 &OptionMenu.Buttons[OPTION_BUTTON_KEYBIND].Rect,
					 V2(0, 0), TEXT_POSITION_CENTERED);
	
	//AUDIO 
	v2 MasterSliderPos = 
	GetPos(V2(-180, OptionMenu.Text[OPTION_TEXT_SUBTITLE_7].Pos.y), 
		   WinDim, POSITION_TOP_RIGHT);
		   
	v2 MusicSliderPos = 
	GetPos(V2(-180, OptionMenu.Text[OPTION_TEXT_SUBTITLE_8].Pos.y), 
		   WinDim, POSITION_TOP_RIGHT);
	
	v2 SFXSliderPos = 
	GetPos(V2(-180, OptionMenu.Text[OPTION_TEXT_SUBTITLE_9].Pos.y), 
		   WinDim, POSITION_TOP_RIGHT);
	
	OptionMenu.SlideBar[OPTION_SLIDEBAR_MASTER_VOL] =
	CreateSlidebar(Renderer->Assets[ASSET_GREY_SLIDER].ID, 
				   Renderer->Assets[ASSET_GREY_CIRCLE].ID,
				   MasterSliderPos, V2(300, 30), 
				   (real32)Config.MasterVolume, 100);
				   
	OptionMenu.SlideBar[OPTION_SLIDEBAR_MUSIC_VOL] =
	CreateSlidebar(Renderer->Assets[ASSET_GREY_SLIDER].ID, 
				   Renderer->Assets[ASSET_GREY_CIRCLE].ID,
				   MusicSliderPos, V2(300, 30), 
				   (real32)Config.MusicVolume, 100);
				   
	OptionMenu.SlideBar[OPTION_SLIDEBAR_SFX_VOL] =
	CreateSlidebar(Renderer->Assets[ASSET_GREY_SLIDER].ID, 
				   Renderer->Assets[ASSET_GREY_CIRCLE].ID,
				   SFXSliderPos, V2(300, 30), 
				   (real32)Config.SFXVolume, 100);
				   
	int MusicVolume = (int)((Config.MasterVolume / 100.0f) * Config.MusicVolume);
	int SFXVolume = (int)((Config.MasterVolume / 100.0f) * Config.SFXVolume);
	
	Mix_VolumeMusic(MusicVolume);
	Mix_Volume(-1, SFXVolume);
	
	return OptionMenu;
}

void UpdateOptionMenu(game_option* OptionMenu, 
					  game_sound* Audio,
					  game_skin* Skin,
					  game_background_scroll* BackgroundScroll,
					  game_input* Input, 
					  game_renderer* Renderer,
					  render* Graphics)
{
	v2 WinDim = V2(Graphics->Dim);

	if(!IsMenuActive(&OptionMenu->SkinSelection) &&
	   !IsMenuActive(&OptionMenu->FrameLimit))
	{
		HandleButton(&OptionMenu->Buttons[OPTION_BUTTON_BACK], 
					 Input, Audio->SFX[SFX_MENU_BACK]);
		HandleButton(&OptionMenu->Buttons[OPTION_BUTTON_KEYBIND],
					 Input, Audio->SFX[SFX_MENU_CLICK]);
	}
	
	OptionMenu->Config.MasterVolume = 
	(int)HandleSlidebar(&OptionMenu->SlideBar[OPTION_SLIDEBAR_MASTER_VOL], 
						Input);
	OptionMenu->Config.MusicVolume = 
	(int)HandleSlidebar(&OptionMenu->SlideBar[OPTION_SLIDEBAR_MUSIC_VOL], 
						Input);
	OptionMenu->Config.SFXVolume = 
	(int)HandleSlidebar(&OptionMenu->SlideBar[OPTION_SLIDEBAR_SFX_VOL], 
						Input);
						
	int Master = OptionMenu->Config.MasterVolume;
	int Music = OptionMenu->Config.MusicVolume;
	int SFX = OptionMenu->Config.SFXVolume;
	int MusicVolume = (int)((Master / 100.0f) * Music);
	int SFXVolume = (int)((Master / 100.0f) * SFX);
	Mix_VolumeMusic(MusicVolume);
	Mix_Volume(-1, SFXVolume);
	
	HandleDropMenu(&OptionMenu->SkinSelection, Input, Graphics, 
				   Audio->SFX[SFX_ROLLOVER]);
	
	if(!IsMenuActive(&OptionMenu->SkinSelection))
	{
		HandleDropMenu(&OptionMenu->FrameLimit, Input, Graphics,
					   Audio->SFX[SFX_ROLLOVER]);		
	}
	
	if(IsMenuActive(&OptionMenu->SkinSelection))
	{
		for(int i = 0; i < OptionMenu->SkinSelection.ItemCount; ++i)
		{
			if(OptionMenu->SkinSelection.Items[i].State == 
			   UI_BUTTON_STATE_CLICK_L && i != Skin->List.Length)
			{
				Skin->SelectedSkin = 
				(char*)Skin->List.Data[i].c_str();
				
				OptionMenu->SkinSelection.ExpandText = 
				LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
							   Skin->SelectedSkin,
							   Renderer->RenderObjects[R_OBJECT_RECT]);
				SetRenderTextPos(&OptionMenu->SkinSelection.ExpandText, 
								 &OptionMenu->SkinSelection.Expand.Rect,
								 V2(10), TEXT_POSITION_TOP_LEFT);
				
				UpdateSkin(Skin, i, Audio);
				
				ResetBackgroundScroll(BackgroundScroll, 
				Skin->Textures[SKIN_BACKGROUND],
				WinDim);
				
				OptionMenu->SkinSelection.Active = false;
				
				OptionMenu->SkinSelection.Items[i].State = UI_BUTTON_STATE_NONE;
				
				Mix_PlayMusic(Audio->Music[MUS_BACKGROUND], -1);
				
				Input->Mouse.Keys[BUTTON_LEFT].EndedDown = false;
				
				break;
			}
			else if(i == Skin->List.Length && 
					OptionMenu->SkinSelection.Items[i].State == 
					UI_BUTTON_STATE_CLICK_L)
			{
				OptionMenu->SkinSelection.Active = false;
				Input->Mouse.Keys[BUTTON_LEFT].EndedDown = false;
				OptionMenu->SkinSelection.Items[i].State = 
				UI_BUTTON_STATE_NONE;
				char Buffer[500] = {};
				stbsp_sprintf(Buffer, 
							  "skins\\%s\\", 
							  Skin->SelectedSkin);
				ShellExecuteA(0, "open", Buffer, 0, 0, SW_SHOWDEFAULT);
				break;
			}
		}
	}
	
	if(IsMenuActive(&OptionMenu->FrameLimit))
	{
		for(int ButtonIndex = 0; 
			ButtonIndex < OptionMenu->FrameLimit.ItemCount; 
			++ButtonIndex)
		{
			if(OptionMenu->FrameLimit.Items[ButtonIndex].State == 
						UI_BUTTON_STATE_CLICK_L)
			{
				switch(ButtonIndex)
				{
					case 0: OptionMenu->Config.FrameLimit = 60; break;
					case 1: OptionMenu->Config.FrameLimit = 144; break;
					case 2: OptionMenu->Config.FrameLimit = 240; break;
					case 3: OptionMenu->Config.FrameLimit = -1; break;
				}
				
				char Buffer[20] = "Unlimited";
				if(OptionMenu->Config.FrameLimit > 0)
					stbsp_sprintf(Buffer, "%d", OptionMenu->Config.FrameLimit);
				
				OptionMenu->FrameLimit.ExpandText = 
				LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
							   Buffer,
							   Renderer->RenderObjects[R_OBJECT_RECT]);
				SetRenderTextPos(&OptionMenu->FrameLimit.ExpandText, 
								 &OptionMenu->FrameLimit.Expand.Rect,
								 V2(10), TEXT_POSITION_TOP_LEFT);
				
				OptionMenu->FrameLimit.Active = false;
				OptionMenu->FrameLimit.Items[ButtonIndex].State = UI_BUTTON_STATE_NONE;
				
				break;
			}
		}
	}
	
	if(!OptionMenu->SkinSelection.Active)
	{
		for(int i = 0; i < OPTION_CHECKBOX_MAX; ++i)
		{
			HandleCheckBox(&OptionMenu->Checkbox[i], Input, 
						   Audio->SFX[SFX_SWITCH]);
		}
		OptionMenu->Config.BackgroundScroll = 
		OptionMenu->Checkbox[OPTION_CHECKBOX_SCROLL].Active;
		OptionMenu->Config.DisplayFPS = 
		OptionMenu->Checkbox[OPTION_CHECKBOX_FPS].Active;
	}
}

void RenderOptionMenu(game_option* OptionMenu, 
					  game_renderer* Renderer, 
					  render* Graphics)
{
	v2 WinDim = V2(Graphics->Dim);
	
	RenderDropMenu(&OptionMenu->SkinSelection, Renderer, Graphics);
	
	if(!IsMenuActive(&OptionMenu->SkinSelection))
	{
		RenderDropMenu(&OptionMenu->FrameLimit, Renderer, Graphics);
	}
				
	glViewport(0, 0, Graphics->Dim.x, Graphics->Dim.y);
	
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
							 WinDim);
	SetScreenSpaceProjection(Renderer->
							 Shaders[RENDERER_PRIMITIVES],
							 WinDim);
							 
	if(!IsMenuActive(&OptionMenu->SkinSelection))
	{
		RenderCheckBoxes(OptionMenu->Checkbox, Renderer, 
					 OPTION_CHECKBOX_MAX);
	}
			  
	for(int i = 0; i < OPTION_TEXT_MAX; ++i)
	{
		RenderText(Renderer->Shaders[RENDERER_TEXTURED],
				   &OptionMenu->Text[i],
				   Color(), false);
	}
	
	RenderButton(&OptionMenu->Buttons[OPTION_BUTTON_BACK], Renderer);
	
	if(!IsMenuActive(&OptionMenu->SkinSelection) && 
	   !IsMenuActive(&OptionMenu->FrameLimit))
    {
		RenderButton(&OptionMenu->Buttons[OPTION_BUTTON_KEYBIND], Renderer);
	
		RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
				   &OptionMenu->KeyBindText, 
				   Color(), false);
    }
	
	RenderSlidebars(OptionMenu->SlideBar, Renderer, OPTION_SLIDEBAR_MAX);
}