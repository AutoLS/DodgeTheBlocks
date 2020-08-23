#include "dtb.h"

game_core InitGame(game_state State, render* Graphics)
{
	game_core Game = {};
	Game.State = State;
	Game.Running = true;
	Game.Renderer = InitRenderer();
	
	Game.Audio.SFX[SFX_SWITCH] = Mix_LoadWAV("ast/switch.wav");
	
	option_config OptionConfig = LoadOptionConfig(Graphics);
	Game.Skin = LoadSkin(OptionConfig.SelectedSkin, &Game.Audio);
	Game.OptionMenu = InitOptionMenu(OptionConfig, &Game.Skin, 
									 &Game.Renderer, Graphics);
	
	LoadProfile(&Game.Profile, OptionConfig.Profile);
	Game.ProfileCreation = InitProfileCreationMenu(&Game.Renderer, Graphics);
	
	Game.UpgradeMenu = InitUpgradeMenu(&Game.Renderer, Graphics, &Game.Profile);
	Game.ProfileStatus = InitProfileStatus(&Game.Profile, &Game.Renderer, Graphics);

	return Game;
}

void InitGameUI(game_core* Game, render* Graphics)
{
	v2 WinDim = V2(Graphics->Dim);
	
	game_ui* UI = &Game->UI;
	game_renderer* Renderer = &Game->Renderer;
	game_skin* Skin = &Game->Skin;
	
	//NOTE: Gameover state
	UI->LevelUPMsg = CreateMessageBox(UI_MSGBOX_OK, Renderer, "Info",
									 "You leveled up!",
									 WinDim*0.5f, V2(500, 300));
	UI->GameOverText = LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], 
									   "Game over!", 
									   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &UI->GameOverText);
	
	UI->ContinueText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], 
				   "Press space to continue..", 
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &UI->ContinueText, V2(0, 100));
	
	//NOTE: Message box
	UI->QuitConfirmMessageBox = 
	CreateMessageBox(UI_MSGBOX_YESNO, 
					 Renderer, 
					 "Confirmation",
					 "Are you sure you want to quit?",
					 WinDim * 0.5f, V2(400, 300));
	
	//NOTE: Init main menu
	TTF_Font* LogoFont = TTF_OpenFont("ast/fonts/font.ttf", 72);
	UI->GameLogo = LoadRenderText(LogoFont, "D.T.B",
								  Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &UI->GameLogo, V2(0, -200));
	TTF_CloseFont(LogoFont);
	
	render_text StartText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "START",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &StartText, V2(0, -80));
	UI->MenuButtons[MENU_BUTTON_START] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, StartText);
	
	render_text OptionsText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "OPTIONS",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &OptionsText);
	UI->MenuButtons[MENU_BUTTON_OPTIONS] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, OptionsText);
	
	render_text ProfileText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "PROFILE",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &ProfileText, V2(0, 80));
	UI->MenuButtons[MENU_BUTTON_PROFILE] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, ProfileText);
	
	render_text HelpText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "HELP",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &HelpText, V2(0, 160));
	UI->MenuButtons[MENU_BUTTON_HELP] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, HelpText);
	
	render_text QuitText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "QUIT",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &QuitText, V2(0, 240));
	UI->MenuButtons[MENU_BUTTON_QUIT] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, QuitText);
	
	//NOTE: Init main lobby
	render_text PlayText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "PLAY",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &PlayText, V2(15, 15), TEXT_POSITION_TOP_LEFT);
	UI->LobbyButtons[LOBBY_BUTTON_PLAY] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, PlayText);
	
	render_text LobbyOptionText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "OPTIONS",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &LobbyOptionText, 
					 GetMaxPosFromRectX(
					 &UI->LobbyButtons[LOBBY_BUTTON_PLAY].Rect) + V2(30), 
					 TEXT_POSITION_TOP_LEFT);
	UI->LobbyButtons[LOBBY_BUTTON_OPTIONS] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, LobbyOptionText);
	
	render_text UpgradeText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "UPGRADES",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &UpgradeText, 
					 GetMaxPosFromRectX(
					 &UI->LobbyButtons[LOBBY_BUTTON_OPTIONS].Rect) + V2(30), TEXT_POSITION_TOP_LEFT);
	UI->LobbyButtons[LOBBY_BUTTON_UPGRADES] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, UpgradeText);
	
	render_text LobbyBackText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "BACK",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &LobbyBackText, 
					 V2(15, -15), TEXT_POSITION_BOTTOM_LEFT);
	UI->LobbyButtons[LOBBY_BUTTON_BACK] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, LobbyBackText);
	
	//NOTE: Init pause menu buttons
	render_text ContinueText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], "Continue",
				   Renderer->RenderObjects[R_OBJECT_RECT]); 
	SetRenderTextPos(Graphics, &ContinueText, V2(0, -80));
	UI->PauseButtons[PAUSE_BUTTON_CONTINUE] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, ContinueText);

	render_text RetryText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], "Retry",
				   Renderer->RenderObjects[R_OBJECT_RECT]); 
	SetRenderTextPos(Graphics, &RetryText);
	UI->PauseButtons[PAUSE_BUTTON_RETRY] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, RetryText);
	
	render_text BackText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], "Back To Menu",
				   Renderer->RenderObjects[R_OBJECT_RECT]); 
	SetRenderTextPos(Graphics, &BackText, V2(0, 80));
	UI->PauseButtons[PAUSE_BUTTON_BACK] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, BackText);
	
	//NOTE: Init help menu
	char* HelpMenuText[HELP_TEXT_MAX] = 
	{
		"Gameplay - Classic Mode",
		"Movement",
		"Press arrow keys to move, shift key to dash",
		" - Don't touch this",
		" - Gains HP",
		" - Invincible for 3 seconds",
		" - Timewarp. Reduces block speed for 5 seconds",
		"BACK"
	};
	
	UI->HelpText[HELP_TEXT_TITLE_1] =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], 
				   HelpMenuText[HELP_TEXT_TITLE_1],
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	UI->HelpText[HELP_TEXT_SUBTITLE_1] =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], 
				   HelpMenuText[HELP_TEXT_SUBTITLE_1],
				   Renderer->RenderObjects[R_OBJECT_RECT]);
				   
	SetRenderTextPos(Graphics, 
					 &UI->HelpText[HELP_TEXT_TITLE_1], 
					 V2(15, 15),
					 TEXT_POSITION_TOP_LEFT);
	SetRenderTextPos(Graphics, 
					 &UI->HelpText[HELP_TEXT_SUBTITLE_1], 
					 V2(15, 60),
					 TEXT_POSITION_TOP_LEFT);
	int Count = 0;
	for(int i = HELP_TEXT_BODY_1; i < HELP_TEXT_BODY_6; ++i)
	{
		UI->HelpText[i] =
		LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], 
					   HelpMenuText[i],
					   Renderer->RenderObjects[R_OBJECT_RECT]);
		SetRenderTextPos(Graphics, 
						 &UI->HelpText[i], 
						 V2(50, 125 + (Count++ * 60.0f)),
						 TEXT_POSITION_TOP_LEFT);
	}
	UI->HelpText[HELP_TEXT_BODY_6] =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], 
				   HelpMenuText[HELP_TEXT_BODY_6],
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, 
					 &UI->HelpText[HELP_TEXT_BODY_6], 
					 V2(10, -10),
					 TEXT_POSITION_BOTTOM_LEFT);
	UI->HelpBackButton = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, UI->HelpText[HELP_TEXT_BODY_6]);
}

void MapKeyInput(game_input_map* InputMap, game_input* Input, 
				 option_keybind* Keybind)
{
	for(int KeyIndex = 0; KeyIndex < KEYBIND_MAX; ++KeyIndex)
	{
		InputMap->Keys[KeyIndex] = Input->Keyboard.Keys[Keybind->Keys[KeyIndex]];
	}
}

void UpdateGameState(game_core* Game, game_state NewState)
{
	Game->LastState = Game->State;
	Game->State = NewState;
}

real32 GetScoreRequitementForLeveling(int Level)
{
	if(Level <= 100)
	{
		return 5000/3 * 
			  (4*powf((real32)Level, 3) - 3*powf((real32)Level, 2) - Level) + 
			   1.25f*powf(1.8f, (real32)Level - 60);
	}
	else
	{
		return 26931190827.0f + 99999999999.0f * (Level - 100);
	}
}

void MainGame(game_core* Game, game_input* Input, render* Graphics)
{			  
	v2 WinDim = V2(Graphics->Dim);
	SDL_Event Event;
	
	real32 t = 0;
	uint64 LastCount = SDL_GetPerformanceCounter();
	
	entity_pool EntityPool = InitializeEntityPool(1000, 10);
	
	timer GlobalTimer = CreateTimer(0);
										   
	game_renderer* Renderer = &Game->Renderer;
	game_ui* UI = &Game->UI;
	
	ResetBackgroundScroll(&Game->BackgroundScroll,
						  Game->Skin.Textures[SKIN_BACKGROUND],
						  V2(Graphics->Dim));
	
	text_editing TextEdit = {};
	
	if(!Mix_PlayingMusic())
	{
		Mix_PlayMusic(Game->Audio.Music[MUS_BACKGROUND], -1);
	}
	
	while(Game->Running)
	{
		glViewport(0, 0, Graphics->Dim.x, Graphics->Dim.y);
		
		ProcessTimer(&GlobalTimer, t);
		
		ResetKeyState(&Input->Keyboard, KEY_MAX);
		ResetKeyState(&Input->Mouse, BUTTON_MAX);
		Input->KeyEndedDown = false;
		while(SDL_PollEvent(&Event) != 0)
		{
			if(HandleEvents(&Event, Input, Graphics))
			{
				Game->Running = false;
			}
		}
		MapKeyInput(&Game->InputMap, Input, &Game->OptionMenu.KeyBind);

		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//Logic
		if(Game->OptionMenu.Checkbox[OPTION_CHECKBOX_SCROLL].Active)
		{
			ScrollBackground(&Game->BackgroundScroll, WinDim, t);			
		}
		else
		{
			ResetBackgroundScroll(&Game->BackgroundScroll, 
								   Game->Skin.Textures[SKIN_BACKGROUND],
								   WinDim);
		}
		
		switch(Game->State)
		{
			case STATE_MAIN_MENU:
			{
				if(UI->QuitConfirmMessageBox.Active)
				{
					ui_msg_box_state State = 
					ProcessMessageBox(&UI->QuitConfirmMessageBox, Input);
					
					switch(State)
					{
						case UI_MSGBOX_STATE_YES:
						{
							Game->Running = false;
						} break;
					}
				}
				else
				{
					for(int ButtonIndex = 0; 
						ButtonIndex < MENU_BUTTON_MAX; 
						++ButtonIndex)
					{
						HandleButton(&UI->MenuButtons[ButtonIndex], Input,
									 Game->Audio.SFX[SFX_MENU_CLICK]);
						
						if(UI->MenuButtons[ButtonIndex].State == 
							UI_BUTTON_STATE_CLICK_L)
						{
							switch(ButtonIndex)
							{
								case MENU_BUTTON_START:
								{
									UpdateProfileStatus(&Game->ProfileStatus, 
														&Game->Profile,
														Renderer,
														Graphics);
									UpdateGameState(Game, STATE_MAIN_LOBBY);
								} break;
								
								case MENU_BUTTON_OPTIONS:
								{
									Game->OptionMenu.OptionEnterState = Game->State;
									UpdateGameState(Game, STATE_OPTIONS);
								} break;
								
								case MENU_BUTTON_QUIT:
								{
									PromptMessageBox(&UI->QuitConfirmMessageBox);
									UI->MenuButtons[ButtonIndex].State =
									UI_BUTTON_STATE_NONE;
								} break;
								
								case MENU_BUTTON_HELP:
								{
									UpdateGameState(Game, STATE_HELP);
								} break;
								
								case MENU_BUTTON_PROFILE:
								{
									Game->ProfileSelection = 
									InitProfileSelectionMenu(&Game->Profile,
															 Renderer,
															 Graphics);
									UpdateGameState(Game, 
									STATE_PROFILE_SELECTION);
								} break;
							}
						}
					}
				}	
			} break;
			
			case STATE_MAIN_LOBBY:
			{
				for(int ButtonIndex = 0; 
						ButtonIndex < LOBBY_BUTTON_MAX; 
						++ButtonIndex)
					{
						if(ButtonIndex == LOBBY_BUTTON_BACK)
							HandleButton(&UI->LobbyButtons[ButtonIndex], Input,
										 Game->Audio.SFX[SFX_MENU_BACK]);
						else
							HandleButton(&UI->LobbyButtons[ButtonIndex], Input,
										 Game->Audio.SFX[SFX_MENU_CLICK]);
									 
						if(UI->LobbyButtons[ButtonIndex].State == 
							UI_BUTTON_STATE_CLICK_L)
						{
							switch(ButtonIndex)
							{
								case LOBBY_BUTTON_PLAY:
								{
									InitModeClassicDiffSelect(
								    &Game->ModeClassic, 
								    Renderer, 
								    Graphics);
									UpdateGameState(Game, STATE_MC_DIFF_SELECT);
								} break;
								
								case LOBBY_BUTTON_OPTIONS:
								{
									Game->OptionMenu.OptionEnterState = Game->State;
									UpdateGameState(Game, STATE_OPTIONS);
								} break;
								
								case LOBBY_BUTTON_UPGRADES:
								{
									UpdateUpgradeMenu(&Game->UpgradeMenu, &Game->Profile,
													  Renderer, Graphics);
									UpdateGameState(Game, STATE_UPGRADES);
								} break;
								
								case LOBBY_BUTTON_BACK:
								{
									UpdateGameState(Game, STATE_MAIN_MENU);
								} break;
							}
						}
					}
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown)
				{
					UpdateGameState(Game, STATE_MAIN_MENU);
				}
			} break;
			
			case STATE_OPTIONS:
			{
				game_option* OptionMenu = &Game->OptionMenu;
				UpdateOptionMenu(OptionMenu, 
								 &Game->Audio,
								 &Game->Skin,
								 &Game->BackgroundScroll,
								 Input, Renderer, Graphics);
				
				if(!OptionMenu->SkinSelection.Active)
				{
					if(!IsMenuActive(&OptionMenu->FrameLimit))
					{
						if(OptionMenu->Buttons[OPTION_BUTTON_KEYBIND].State == 
						   UI_BUTTON_STATE_CLICK_L)
						{
							UpdateGameState(Game, STATE_OPTIONS_KEYBIND);
						}
					}
				}
				
				//NOTE: This is stupid, perhaps use a stack instead to keep track of option states  
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown)
				{
					UpdateGameState(Game, OptionMenu->OptionEnterState);
				}
				if(Game->OptionMenu.Buttons[OPTION_BUTTON_BACK].State == 
				   UI_BUTTON_STATE_CLICK_L)
				{
					UpdateGameState(Game, OptionMenu->OptionEnterState);
				}
			} break;
			
			case STATE_OPTIONS_KEYBIND:
			{
				keybind_menu* Menu = &Game->OptionMenu.KeybindMenu;
				UpdateKeybindMenu(&Game->OptionMenu, 
								  &Game->Audio,
								  Input, 
								  Renderer);
				if((Input->Keyboard.Keys[KEY_ESC].EndedDown || 
					(Menu->BackButton.State == UI_BUTTON_STATE_CLICK_L)) 
					&& !Menu->IsKeybinding)
				{
					UpdateGameState(Game, Game->LastState);
				}
			} break;
			
			case STATE_HELP:
			{
				HandleButton(&UI->HelpBackButton, Input, 
							 Game->Audio.SFX[SFX_MENU_BACK]);
				
				if(UI->HelpBackButton.State == UI_BUTTON_STATE_CLICK_L)
				{
					UpdateGameState(Game, STATE_MAIN_MENU);
				}
				
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown)
				{
					UpdateGameState(Game, STATE_MAIN_MENU);
				}
			} break;
			
			case STATE_PROFILE_SELECTION:
			{
				UpdateProfileSelectionMenu(&Game->ProfileSelection, 
										   &Game->Profile, 
										   Input, 
										   Renderer,
										   &Game->Audio,
										   Graphics);
				
				int Len = Game->ProfileSelection.List.Length;
				if(Game->ProfileSelection.Buttons[Len].State == 
				   UI_BUTTON_STATE_CLICK_L)
				{
					UpdateGameState(Game, STATE_PROFILE_CREATION);
					SDL_StartTextInput();
				}
				
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown ||
				   Game->ProfileSelection.BackButton.State == 
				   UI_BUTTON_STATE_CLICK_L)
				{
					FreeProfileSelectionMenu(&Game->ProfileSelection);
					UpdateGameState(Game, STATE_MAIN_MENU);
				}
			} break;
			
			case STATE_PROFILE_CREATION:
			{
				UpdateProfileCreationMenu(&Game->ProfileCreation, 
										  &Game->ProfileSelection,
										  &Game->Profile,
										  Graphics,
										  Renderer, Input, 
										  &Game->Audio, &Event, t);
				if(Game->ProfileCreation.Exit)
				{
					Game->ProfileCreation.Exit = false;
					UpdateGameState(Game, Game->LastState);
				}
			} break;
			
			case STATE_UPGRADES:
			{
				upgrade_menu* UpgradeMenu = &Game->UpgradeMenu;
				HandleButton(&UpgradeMenu->BackButton, Input, 
							 Game->Audio.SFX[SFX_MENU_BACK]);
							 
				for(int i = 0; i < UPGRADE_TYPE_MAX; ++i)
				{
					HandleButton(&UpgradeMenu->Plus[i], Input);
					if(UpgradeMenu->Plus[i].State == UI_BUTTON_STATE_CLICK_L)
					{
						if(Game->Profile.SkillPoints > 0)
						{
							if(i == UPGRADE_TYPE_HP)
							{
								if(Game->Profile.UpgradeLevels[i] < 50)
								{
									--Game->Profile.SkillPoints;
									++Game->Profile.UpgradeLevels[i];
									UpdateUpgradeMenu(&Game->UpgradeMenu, &Game->Profile, 
													  Renderer, Graphics);
								}
							}
							else
							{
								--Game->Profile.SkillPoints;
								++Game->Profile.UpgradeLevels[i];
								UpdateUpgradeMenu(&Game->UpgradeMenu, &Game->Profile, 
											  Renderer, Graphics);
							}
						}
					}	
				}
				for(int i = 0; i < UPGRADE_TYPE_MAX; ++i)
				{
					HandleButton(&UpgradeMenu->Minus[i], Input);
					if(UpgradeMenu->Minus[i].State == UI_BUTTON_STATE_CLICK_L)
					{
						if(Game->Profile.UpgradeLevels[i] > 0)
						{
							--Game->Profile.UpgradeLevels[i];
							++Game->Profile.SkillPoints;
							UpdateUpgradeMenu(&Game->UpgradeMenu, &Game->Profile, 
											  Renderer, Graphics);
						}
					}	
				}
							 
				for(int i = 0; i < UPGRADE_TYPE_MAX; ++i)
				{
					ProcessHoverBlock(&UpgradeMenu->InfoBlock[i], Input);
				}
							 
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown ||
				   UpgradeMenu->BackButton.State == 
				   UI_BUTTON_STATE_CLICK_L)
				{
					SaveProfile(&Game->Profile);
					UpdateGameState(Game, Game->LastState);
				}
			} break;
			
			case STATE_MC_DIFF_SELECT:
			{
				mc_diff_select* DiffSelect = &Game->ModeClassic.DiffSelect;
				
				UpdateMCDifficultySelection(&Game->ModeClassic, 
											Renderer,
											Input, 
											&EntityPool, 
											Graphics,
											Game->Audio.SFX);
				if(DiffSelect->Buttons[MC_DIFF_BUTTON_BACK].State == 
					UI_BUTTON_STATE_CLICK_L)
				{
					UpdateGameState(Game, Game->LastState);
				}
				
				if(DiffSelect->Buttons[MC_DIFF_BUTTON_START].State == 
					UI_BUTTON_STATE_CLICK_L)
				{
					InitModeClassic(&Game->ModeClassic, 
									&Game->Profile,
									Renderer,
									Game->Skin.Textures,
									WinDim, &EntityPool, 
									Game->ModeClassic.Difficulty);
					UpdateGameState(Game, STATE_MODE_CLASSIC);
				}
				
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown)
				{
					UpdateGameState(Game, Game->LastState);
				}
			} break;
			
			case STATE_MODE_CLASSIC:
			{
				game_mode_classic* ModeClassic = &Game->ModeClassic;
				mc_game_info* GameInfo = &ModeClassic->GameInfo;
				mc_player_info* PlayerInfo = &ModeClassic->PlayerInfo;
				//UpdateGameState(Game, STATE_GAMEOVER);
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown)
				{
					UpdateGameState(Game, STATE_PAUSED);
				}
				UpdateModeClassic(ModeClassic, 
								  &Game->Profile,
								  &Game->Renderer,
								  Game->Skin.Textures,
								  &Game->PlayerData,
								  &EntityPool, Input,
								  &Game->InputMap, 
								  &Game->Audio, t);
								  
				if(PlayerOutOfHealth(&ModeClassic->PlayerInfo))
				{
					for(int i = 0; i < MC_STAGE_EFFECT_MAX; ++i)
					{
						ModeClassic->StageEffects[i] = false;
					}
					
					ModeClassic->GameInfo.HighScore = 
					ModeClassic->GameInfo.Score > 
					Game->Profile.Highscore;
					
					ModeClassic->SBAnimation.State = SB_ANI_STATE_STATS;
					if(ModeClassic->GameInfo.HighScore)
					{
						ModeClassic->SBAnimation.State = SB_ANI_STATE_HIGHSCORE;
						Game->PlayerData.ModeClassicHighScore = 
						ModeClassic->GameInfo.Score;
						
						Game->Profile.Highscore = ModeClassic->GameInfo.Score;
					}
					UpdateGameState(Game, STATE_GAMEOVER);
					Mix_HaltMusic();
					Mix_RewindMusic();
					Game->Profile.PlayTimeInHours += 
					ModeClassic->Timers[MC_GAME_TIMER].Time / 3600;
					Game->Profile.nBlocksDodged += PlayerInfo->nBlocksDodged;
					Game->Profile.nItemsPickedUp += PlayerInfo->nItemPickups;
					Game->Profile.nEnemyKilled += PlayerInfo->nEnemyKilled;
					Game->Profile.DeathCount += PlayerInfo->DeathCount;
					Game->Profile.TotalScore += ModeClassic->GameInfo.Score;
					if(Game->Profile.TotalScore >= 
					   GetScoreRequitementForLeveling(Game->Profile.Level+1))
					{
						while(Game->Profile.TotalScore >= 
							  GetScoreRequitementForLeveling(
							  Game->Profile.Level+1))
					   {
						    ++Game->Profile.Level;
							Game->Profile.SkillPoints += Game->Profile.Level;
							Game->PlayerData.LevelUP = true;
							//printf("Leveled up!\n");
							// printf("Total score needed for next level: %.0f\n",
								   // GetScoreRequitementForLeveling(
								   // Game->Profile.Level+1));
					   }
					}
					if(Game->PlayerData.LevelUP)
					{
						PromptMessageBox(&UI->LevelUPMsg);
						Game->PlayerData.LevelUP = false;
					}
					SaveProfile(&Game->Profile);
				}
			} break;
			
			case STATE_PAUSED:
			{
				SDL_ShowCursor(SDL_ENABLE);
				switch(Game->LastState)
				{
					case STATE_MODE_CLASSIC:
					{
						game_mode_classic* ModeClassic = &Game->ModeClassic;
						if(HandleButton(&UI->PauseButtons[PAUSE_BUTTON_RETRY],
										Input,
										Game->Audio.SFX[SFX_MENU_CLICK]))
						{
							InitModeClassic(ModeClassic, 
											&Game->Profile,
											&Game->Renderer,
											Game->Skin.Textures,
											WinDim,
											&EntityPool, 
											ModeClassic->Difficulty);
							UpdateGameState(Game, Game->LastState);
						}
						if(HandleButton(
						  &UI->PauseButtons[PAUSE_BUTTON_CONTINUE], 
						  Input,
						  Game->Audio.SFX[SFX_MENU_CLICK]))
						{
							UpdateGameState(Game, Game->LastState);
						}
						if(HandleButton(&UI->PauseButtons[PAUSE_BUTTON_BACK], 
										Input,
										Game->Audio.SFX[SFX_MENU_BACK]))
						{
							UpdateProfileStatus(&Game->ProfileStatus, 
												&Game->Profile,
												Renderer,
												Graphics);
							UpdateGameState(Game, STATE_MAIN_LOBBY);
						}
					} break;
				}
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown)
				{
					UpdateGameState(Game, Game->LastState);
				}
			} break;
			
			case STATE_GAMEOVER:
			{
				if(!Mix_PlayingMusic())
				{
					Mix_PlayMusic(Game->Audio.Music[MUS_GAMEOVER], -1);
				}
				
				SDL_ShowCursor(SDL_ENABLE);
				switch(Game->LastState)
				{
					case STATE_MODE_CLASSIC:
					{
						game_mode_classic* ModeClassic = &Game->ModeClassic;
						ProcessMessageBox(&UI->LevelUPMsg, Input);
						
						if(!UI->LevelUPMsg.Active)
						{
							HandleScrollBar(&ModeClassic->ScoreBoardScrollBar, 
										Input, Graphics);
							if(Input->Keyboard.Keys[KEY_SPACE].EndedDown && 
							   ModeClassic->SBAnimation.State == 
							   SB_ANI_STATE_COMPLETE)
							{
								for(int i = 0; i < SB_ANI_STATE_COMPLETE; ++i)
								{
									glDeleteTextures(1, &ModeClassic->SBAnimation.
														Text[i].Texture);
								}
								
								InitModeClassic(ModeClassic, 
												&Game->Profile,
												&Game->Renderer,
												Game->Skin.Textures,
												WinDim,
												&EntityPool, 
												ModeClassic->Difficulty);
								UpdateGameState(Game, Game->LastState);
								
								Mix_HaltMusic();
								Mix_RewindMusic();
								Mix_PlayMusic(Game->Audio.Music[MUS_BACKGROUND], -1);
							}
							
							if(Input->Keyboard.Keys[KEY_SPACE].EndedDown)
							{
								if(ModeClassic->SBAnimation.State != 
								   SB_ANI_STATE_COMPLETE)
								{
									ModeClassic->SBAnimation.State =
									SB_ANI_STATE_COMPLETE;
								}
							}
							
							if(Input->Keyboard.Keys[KEY_ESC].EndedDown)
							{
								for(int i = 0; i < SB_ANI_STATE_COMPLETE; ++i)
								{
									glDeleteTextures(1, 
									&ModeClassic->SBAnimation.Text[i].Texture);
								}
								UpdateProfileStatus(&Game->ProfileStatus, 
													&Game->Profile,
													Renderer,
													Graphics);
								UpdateGameState(Game, STATE_MAIN_LOBBY);
								
								Mix_HaltMusic();
								Mix_RewindMusic();
								Mix_PlayMusic(Game->Audio.Music[MUS_BACKGROUND], -1);
							}
						}
						
					} break;
				}
			} break;
			
			case STATE_DEBUG:
			{
				//ProcessTextInput(Input, &TextEdit, &Event, t);
			} break;
		}

#if 1 	//Enforcing framerate
		Graphics->MissedFrame = false;
		
		if(Game->OptionMenu.Config.FrameLimit > 0)
		{
			real32 TargetSecPerFrame = 1.0f / Game->OptionMenu.Config.FrameLimit;
			real32 FrameCompleteTime = Win32GetSecondElapsed(LastCount, SDL_GetPerformanceCounter());
			if(FrameCompleteTime < TargetSecPerFrame)
			{
				int SleepTime = (int)((TargetSecPerFrame - Win32GetSecondElapsed(LastCount, SDL_GetPerformanceCounter())) * 1000) - 1;
				if(SleepTime > 0)
				{
					Sleep(SleepTime);
				}
				//assert(Win32GetSecondElapsed(LastCount, SDL_GetPerformanceCounter()) < TargetSecPerFrame);
				while(Win32GetSecondElapsed(LastCount, SDL_GetPerformanceCounter()) < TargetSecPerFrame);
			}
			else
			{
				Graphics->MissedFrame = true;
			}
		}
#endif
		uint64 EndCount = SDL_GetPerformanceCounter();
		
		//Rendering
		
		if(Game->ModeClassic.StageEffects[MC_STAGE_FLASHLIGHT])
		{
			SetInt(Renderer->Shaders[RENDERER_TEXTURED], "StageEffect", 1);
		}
		SetWorldSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
								WinDim);
		DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
				   &Renderer->RenderObjects[R_OBJECT_RECT],
				   V3(Game->BackgroundScroll.Camera1), 
				   V3(Game->Skin.Textures[SKIN_BACKGROUND].Dim),
				   0, {}, Color(), 
				   Game->Skin.Textures[SKIN_BACKGROUND].ID,
				   true);
		DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
				   &Renderer->RenderObjects[R_OBJECT_RECT],
				   V3(Game->BackgroundScroll.Camera2), 
				   V3(Game->Skin.Textures[SKIN_BACKGROUND].Dim),
				   0, {}, Color(), 
				   Game->Skin.Textures[SKIN_BACKGROUND].ID,
				   true);
		SetInt(Renderer->Shaders[RENDERER_TEXTURED], "StageEffect", 0);
		
		switch(Game->State)
		{
			case STATE_DEBUG:
			{
				SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
										 WinDim);
				
				RenderLineOfText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
								 Renderer, "Are you sure you want to\nquit",
								 WinDim*0.5f, Color());
			} break;
			
			case STATE_MAIN_MENU:
			{
				SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
										 WinDim);
				SetScreenSpaceProjection(Renderer->
										 Shaders[RENDERER_PRIMITIVES],
										 WinDim);
				
				RenderText(Renderer->Shaders[RENDERER_TEXTURED], &UI->GameLogo,
						   Color(), false);
						   
				RenderButtons(UI->MenuButtons, MENU_BUTTON_MAX, 
							  Renderer->Shaders[RENDERER_TEXTURED]);
							  
				RenderMessageBox(&UI->QuitConfirmMessageBox, Renderer);
			} break;
			
			case STATE_MAIN_LOBBY:
			{
				SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
										 WinDim);
				SetScreenSpaceProjection(Renderer->
										 Shaders[RENDERER_PRIMITIVES],
										 WinDim);
				RenderButtons(UI->LobbyButtons, LOBBY_BUTTON_MAX, 
							  Renderer->Shaders[RENDERER_TEXTURED]);
				RenderProfileStatus(&Game->ProfileStatus, Renderer);
			} break;
			
			case STATE_UPGRADES:
			{
				RenderUpgradeMenu(&Game->UpgradeMenu, Renderer, Graphics);
			} break;
			
			case STATE_OPTIONS:
			{
				RenderOptionMenu(&Game->OptionMenu, Renderer, Graphics);
			} break;
			
			case STATE_OPTIONS_KEYBIND:
			{
				keybind_menu* Menu = &Game->OptionMenu.KeybindMenu;
				
				SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
										 WinDim);
				SetScreenSpaceProjection(Renderer->
										 Shaders[RENDERER_PRIMITIVES],
										 WinDim);
				for(int i = 0; i < KEYBIND_TEXT_MAX; ++i)
				{
					RenderText(Renderer->Shaders[RENDERER_TEXTURED],
							   &Menu->Text[i],
							   Color(), false);
				}
				for(int ButtonIndex = 0;
					ButtonIndex < KEYBIND_MAX; 
					++ButtonIndex)
				{
					RenderButton(&Menu->Buttons[ButtonIndex], Renderer);
					if(Menu->IsKeybinding)
					{
						if(ButtonIndex != Menu->CurrentBinding)
						{
							RenderText(Renderer->Shaders[RENDERER_TEXTURED],
									   &Menu->KeyBindText[ButtonIndex],
									   Color(), false);
						}
					}
					else
					RenderText(Renderer->Shaders[RENDERER_TEXTURED],
							   &Menu->KeyBindText[ButtonIndex],
							   Color(), false);
				}
				RenderButton(&Menu->BackButton, Renderer);
			} break;
			
			case STATE_HELP:
			{
				SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
											WinDim);
				
				RenderButton(&UI->HelpBackButton, Renderer);
				
				for(int i = 0; i < HELP_TEXT_MAX-1; ++i)
				{
					RenderText(Renderer->Shaders[RENDERER_TEXTURED],
							   &UI->HelpText[i],
							   Color(), false);
				}
				
				for(int i = SKIN_BLOCK; i < SKIN_ITEM_TIMEWARP+1; ++i)
				{
					DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
							   &Renderer->RenderObjects[R_OBJECT_RECT],
							   V3(30, 80 + i*60.0f), V3(30, 30),
							   0, {}, Color(), 
							   Game->Skin.Textures[i].ID,
							   true);
				}
			} break;
			
			case STATE_PROFILE_SELECTION:
			{
				RenderProfileSelectionMenu(&Game->ProfileSelection, 
										   Renderer,
										   Graphics);
			} break;
			
			case STATE_PROFILE_CREATION:
			{
				RenderProfileCreationMenu(&Game->ProfileCreation, Renderer,
										  Graphics);
			} break;
			
			case STATE_MC_DIFF_SELECT:
			{
				RenderMCDiffSelection(&Game->ModeClassic, 
									  Renderer,
									  Graphics);
			} break;
			
			case STATE_MODE_CLASSIC:
			{
				RenderModeClassic(&Game->ModeClassic, Renderer,
								  &EntityPool, Graphics);
			} break;
			
			case STATE_PAUSED:
			{
				SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
											WinDim);
				
				render_text PausedText = 
				LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
							   "Paused", 
							   Renderer->RenderObjects[R_OBJECT_RECT]); 
				SetRenderTextPos(Graphics, &PausedText, V2(0, -300));
				
				RenderText(Renderer->Shaders[RENDERER_TEXTURED],
						   &PausedText, 
						   Color());
				
				RenderButton(&UI->PauseButtons[PAUSE_BUTTON_RETRY], 
							 Renderer);
				RenderButton(&UI->PauseButtons[PAUSE_BUTTON_CONTINUE], 
							 Renderer);
				RenderButton(&UI->PauseButtons[PAUSE_BUTTON_BACK], 
							 Renderer);
			} break;
			
			case STATE_GAMEOVER:
			{
				game_mode_classic* ModeClassic = &Game->ModeClassic;
				player_data* PlayerData = &Game->PlayerData;

				ProcessTimer(&ModeClassic->Timers[MC_SB_ANIMATION_TIMER], t);

				RenderModeClassicScoreboard(&Game->ModeClassic,
											Renderer,
										   &Game->PlayerData,
										   &ModeClassic->ScoreBoardScrollBar,
										    t);
				
				glViewport(0, 0, Graphics->Dim.x, Graphics->Dim.y);
				
				SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
										 WinDim);
										 
				RenderText(Renderer->Shaders[RENDERER_TEXTURED],
						   &UI->GameOverText, 
						   Color(), false);
				
				real32 Alpha = (30 + abs(sinf(GlobalTimer.Time)) * 225) / 255;
				RenderText(Renderer->Shaders[RENDERER_TEXTURED],
						   &UI->ContinueText, 
						   Color(255, 255, 255, Alpha), false);
						   
				RenderScrollBar(&ModeClassic->ScoreBoardScrollBar, 
							    WinDim,
								Renderer->Shaders[RENDERER_PRIMITIVES],
								&Renderer->RenderObjects[R_OBJECT_RECT]);
				RenderMessageBox(&UI->LevelUPMsg, Renderer);
			} break;
		}
		
		if(Game->OptionMenu.Checkbox[OPTION_CHECKBOX_FPS].Active)
		{
			LoadAndRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
						  ConcatStrf("FPS: ", Graphics->Display.FPS),
						  WinDim,
						  Renderer->Shaders[RENDERER_TEXTURED],
						  Renderer->RenderObjects[R_OBJECT_RECT],
						  TEXT_POSITION_BOTTOM_RIGHT,
						  {}, true, 255, 1, 0, {}, 
						  TEXT_RENDER_SOLID);
		}
		
		SDL_GL_SwapWindow(Graphics->Window);
		
		//TODO: switch to semi-fixed timestep?
		real32 ElapsedTime = Win32GetSecondElapsed(LastCount, EndCount);
		if(Graphics->MissedFrame)
			t = 1.0f / Game->OptionMenu.Config.FrameLimit;
		else
			t = ElapsedTime;
		uint64 CounterElapsed = EndCount - LastCount;
		Graphics->Display.FPS = (real32)PerformanceFrequency / 
							   (real32)CounterElapsed;
		//PrintFPS(Graphics->Display.FPS);

		LastCount = EndCount;
	}
}
