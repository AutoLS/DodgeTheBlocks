#include "game_profile.h"

void SaveProfile(player_profile* Profile)
{
	char Path[255] = {};
	stbsp_sprintf(Path, "profiles/%s.sav", Profile->Name);
	SDL_RWops* SaveFile = SDL_RWFromFile(Path, "w+");
	if(SaveFile)
	{
		char Buffer[255] = {};
		SDL_RWwrite(SaveFile, (void*)Profile, sizeof(player_profile), 1);
		SDL_RWclose(SaveFile);
	}
}

void LoadProfile(player_profile* Profile, char* Name)
{
	Profile->ProfileLoaded = false;
	
	char Path[255] = {};
	stbsp_sprintf(Path, "profiles/%s.sav", Name);
	debug_file SaveFile = ReadEntireFile(Path, "r");
	
	if(SaveFile.Data)
	{
		*Profile = *(player_profile*)SaveFile.Data;
		Profile->ProfileLoaded = true;
	}
	else
	{
		CreateDirectoryA("profiles", 0);
		player_profile NewProfile = {};
		NewProfile.Level = 1;
		NewProfile.SkillPoints = 5;
		strcpy(NewProfile.Name, "Default");
		SaveProfile(&NewProfile);
		*Profile = NewProfile;
	}
}

array_c GetProfileList()
{
	array_c List = InitArray_c(5);
	if(!ReadDirectory("profiles", &List, true))
	{
		if(!CreateDirectoryA("profiles", 0))
		{
			printf("Error creating directory\n");
		}
	}
	
	return List;
}

player_profile_status 
InitProfileStatus(player_profile* Profile, game_renderer* Renderer, render* Graphics)
{
	TTF_Font* FontM = Renderer->RenderFonts[DEFAULT_FONT_MEDIUM];
	object_data Object = Renderer->RenderObjects[R_OBJECT_RECT];
	
	player_profile_status ProfileStatus = {};
	ProfileStatus.Pos = V2(Graphics->WinDim.x * 0.5f, Graphics->WinDim.y * 0.9f);
	ProfileStatus.Dim = V2(400, 100);
	ProfileStatus.Rect = WinRect32(ProfileStatus.Pos, ProfileStatus.Dim);
	
	char Buffer[255] = {};
	stbsp_sprintf(Buffer, "Profile: %s", Profile->Name);
	ProfileStatus.ProfileName = LoadRenderText(FontM, Buffer, Object);
	SetRenderTextPos(&ProfileStatus.ProfileName, &ProfileStatus.Rect, V2(10, 10), 
					 TEXT_POSITION_TOP_LEFT);
	stbsp_sprintf(Buffer, "Lv: %d", Profile->Level);
	ProfileStatus.Level = LoadRenderText(FontM, Buffer, Object);
	SetRenderTextPos(&ProfileStatus.Level, &ProfileStatus.Rect, V2(-10, 10), 
					 TEXT_POSITION_TOP_RIGHT);
					 
	SetRect32(&ProfileStatus.ProgressBarRect, &ProfileStatus.Rect, V2(10, -20), IV2(), 
			  V2(380, 10), POSITION_BOTTOM_LEFT);
			  
	real32 Requirement = GetScoreRequitementForLeveling(Profile->Level+1) - 
						 GetScoreRequitementForLeveling(Profile->Level);
	real32 Progress = Profile->TotalScore - GetScoreRequitementForLeveling(Profile->Level);
	real32 ProgressPercentage = Progress / Requirement;
	
	v2 ProgressDim = V2(380 * ProgressPercentage, 10);
	SetRect32(&ProfileStatus.ProgressRect, &ProfileStatus.ProgressBarRect, 
			  V2(-ProfileStatus.ProgressBarRect.Dim.x*0.5f, -ProgressDim.y*0.5f), IV2(), 
			  ProgressDim, POSITION_TOP_LEFT);
	
	return ProfileStatus;
}

void UpdateProfileStatus(player_profile_status* ProfileStatus, player_profile* Profile, 
						 game_renderer* Renderer, render* Graphics)
{
	TTF_Font* FontM = Renderer->RenderFonts[DEFAULT_FONT_MEDIUM];
	object_data Object = Renderer->RenderObjects[R_OBJECT_RECT];
	
	glDeleteTextures(1, &ProfileStatus->ProfileName.Texture);
	glDeleteTextures(1, &ProfileStatus->Level.Texture);
	char Buffer[255] = {};
	stbsp_sprintf(Buffer, "Profile: %s", Profile->Name);
	ProfileStatus->ProfileName = LoadRenderText(FontM, Buffer, Object);
	SetRenderTextPos(&ProfileStatus->ProfileName, 
					 &ProfileStatus->Rect, V2(10, 10), 
					 TEXT_POSITION_TOP_LEFT);
	stbsp_sprintf(Buffer, "Lv: %d", Profile->Level);
	ProfileStatus->Level = LoadRenderText(FontM, Buffer, Object);
	SetRenderTextPos(&ProfileStatus->Level, &ProfileStatus->Rect, V2(-10, 10), 
					 TEXT_POSITION_TOP_RIGHT);
					 
	real32 Requirement = GetScoreRequitementForLeveling(Profile->Level+1) - 
						 GetScoreRequitementForLeveling(Profile->Level);
	real32 Progress = Profile->TotalScore - 
					  GetScoreRequitementForLeveling(Profile->Level);
	real32 ProgressPercentage = Progress / Requirement;
	
	v2 ProgressDim = V2(380 * ProgressPercentage, 10);
	SetRect32(&ProfileStatus->ProgressRect, &ProfileStatus->ProgressBarRect, 
			  V2(-ProfileStatus->ProgressBarRect.Dim.x*0.5f, -ProgressDim.y*0.5f), IV2(), 
			  ProgressDim, POSITION_TOP_LEFT);
}

void RenderProfileStatus(player_profile_status* ProfileStatus, game_renderer* Renderer)
{
	BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES],
			  &Renderer->RenderObjects[R_OBJECT_RECT],
			  V3(ProfileStatus->Pos), V3(ProfileStatus->Dim),
			  Color(),
			  0, {}, false, true);
			  
	BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES],
			  &Renderer->RenderObjects[R_OBJECT_RECT],
			  V3(ProfileStatus->ProgressBarRect.Pos), 
			  V3(ProfileStatus->ProgressBarRect.Dim),
			  Color(),
			  0, {}, false, true);
	BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES],
			  &Renderer->RenderObjects[R_OBJECT_RECT],
			  V3(ProfileStatus->ProgressRect.Pos), 
			  V3(ProfileStatus->ProgressRect.Dim),
			  Color(),
			  0, {}, true, true);
	RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
			   &ProfileStatus->ProfileName, 
			   Color(), false);
	RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
			   &ProfileStatus->Level, 
			   Color(), false);
}

profile_creation 
InitProfileCreationMenu(game_renderer* Renderer, render* Graphics)
{
	profile_creation Result = {};
	TTF_Font* FontL = Renderer->RenderFonts[DEFAULT_FONT_LARGE];
	TTF_Font* FontM = Renderer->RenderFonts[DEFAULT_FONT_MEDIUM];
	object_data Object = Renderer->RenderObjects[R_OBJECT_RECT];
	Result.EnterNameText = LoadRenderText(FontL, "Enter a name", Object);
	SetRenderTextPos(Graphics, &Result.EnterNameText, V2(0, -300));
	Result.RestrictionText = LoadRenderText(FontM, "Max character: 12", Object);
	SetRenderTextPos(Graphics, &Result.RestrictionText, V2(0, -250));
	
	render_text BackText = LoadRenderText(FontL, "BACK", Object); 
	SetRenderTextPos(Graphics, &BackText, V2(15, -15), 
					 TEXT_POSITION_BOTTOM_LEFT);
	Result.BackButton = CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, BackText);
	
	render_text OKText = LoadRenderText(FontL, "OK", Object); 
	SetRenderTextPos(Graphics, &OKText, V2(0, 50));
	Result.OkButton = CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, OKText);
	
	Result.ZeroLength = 
	CreateMessageBox(UI_MSGBOX_OK, Renderer, "Info", "Name cannot be empty!",
					 V2(Graphics->Dim)*0.5f, V2(400, 300));
					 
	Result.ContainsPeriod = 
	CreateMessageBox(UI_MSGBOX_OK, Renderer, "Info", 
					 "'.' Is not allowed!",
					 V2(Graphics->Dim)*0.5f, V2(400, 300));
	
	return Result;
}

void UpdateProfileCreationMenu(profile_creation* ProfileCreation,
							   profile_selection* ProfileSelection,
							   player_profile* Profile,
							   render* Graphics,
							   game_renderer* Renderer,
							   game_input* Input,
							   game_sound* Audio,
							   SDL_Event* Event, 
							   real32 t)
{
	if(!ProfileCreation->ZeroLength.Active &&
	   !ProfileCreation->ContainsPeriod.Active)
	{
		HandleButton(&ProfileCreation->BackButton, Input, 
					 Audio->SFX[SFX_MENU_BACK]);
		HandleButton(&ProfileCreation->OkButton, Input,
					 Audio->SFX[SFX_MENU_CLICK]);
		
		ProcessTextInput(Input, &ProfileCreation->Name, Event, t, 12);		
		
		if(ProfileCreation->Name.Text.length() > 0)
		{
			ProfileCreation->NameText = 
			LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], 
						   (char*)ProfileCreation->Name.Text.c_str(), 
						   Renderer->RenderObjects[R_OBJECT_RECT], 
						   false, 0, {}, 255);
			SetRenderTextPos(Graphics, &ProfileCreation->NameText, V2(0, -100));
		}
					   
		if(ProfileCreation->OkButton.State == UI_BUTTON_STATE_CLICK_L)
		{
			if(ProfileCreation->Name.Text.length() == 0)
			{
				PromptMessageBox(&ProfileCreation->ZeroLength);
			}
			else if(DoesStrContainsChar((char*)ProfileCreation->
										 Name.Text.c_str(), '.'))
			{
				PromptMessageBox(&ProfileCreation->ContainsPeriod);
			}
			else
			{
				SDL_StopTextInput();
				player_profile NewProfile = {};
				NewProfile.Level = 1;
				NewProfile.SkillPoints = 5;
				strcpy(NewProfile.Name, ProfileCreation->Name.Text.c_str());
				SaveProfile(&NewProfile);
				*Profile = NewProfile;
				
				FreeProfileSelectionMenu(ProfileSelection);
				*ProfileSelection = 
				InitProfileSelectionMenu(Profile, Renderer, Graphics);
				
				ProfileCreation->Exit = true;
				glDeleteTextures(1, &ProfileCreation->NameText.Texture);
				ProfileCreation->Name.Text = "";
			}
		}		
		
		if(Input->Keyboard.Keys[KEY_ESC].EndedDown ||
		   ProfileCreation->BackButton.State == UI_BUTTON_STATE_CLICK_L)
		{
			SDL_StopTextInput();
			ProfileCreation->Exit = true;
			glDeleteTextures(1, &ProfileCreation->NameText.Texture);
			ProfileCreation->Name.Text = "";
		}
	}
	ProcessMessageBox(&ProfileCreation->ZeroLength, Input);
	ProcessMessageBox(&ProfileCreation->ContainsPeriod, Input);
}

void RenderProfileCreationMenu(profile_creation* ProfileCreation, 
							   game_renderer* Renderer,
							   render* Graphics)
{
	v2 WinDim = V2(Graphics->Dim);
	
	uint32 TextRenderer = Renderer->Shaders[RENDERER_TEXTURED];
	uint32 PrimitiveRenderer = Renderer->Shaders[RENDERER_PRIMITIVES];
	SetScreenSpaceProjection(TextRenderer, WinDim);
	SetScreenSpaceProjection(PrimitiveRenderer, WinDim);
	
	RenderButton(&ProfileCreation->BackButton, Renderer);
	RenderButton(&ProfileCreation->OkButton, Renderer);
	
	RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
			   &ProfileCreation->EnterNameText, 
			   Color(), false);
	RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
			   &ProfileCreation->RestrictionText, 
			   Color(), false);
	
	if(ProfileCreation->Name.Text.length() > 0)
	{
		RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
			   &ProfileCreation->NameText, 
			   Color());
	}
	
	RenderMessageBox(&ProfileCreation->ZeroLength, Renderer);
	RenderMessageBox(&ProfileCreation->ContainsPeriod, Renderer);
}

void LoadProfileStats(game_renderer* Renderer, 
					  render_text* Stats,
					  profile_selection* ProfileSelection,
					  player_profile* Profile)
{
	TTF_Font* FontM = Renderer->RenderFonts[DEFAULT_FONT_MEDIUM];
	object_data RenderObject = Renderer->RenderObjects[R_OBJECT_RECT];
	
	char* Text[] = 
	{
		ConcatStr("Profile: ", Profile->Name),
		ConcatStri("Level: ", (int)Profile->Level),
		ConcatStrf("Hours played: ", Profile->PlayTimeInHours, 1),
		ConcatStrf("Highscore: ", Profile->Highscore),
		ConcatStrf("Total sccre: ", Profile->TotalScore),
		ConcatStri64("Blocks dodged: ", Profile->nBlocksDodged),
		ConcatStri64("Items picked up: ", Profile->nItemsPickedUp),
		ConcatStri64("Enemy killed: ", Profile->nEnemyKilled),
		ConcatStri64("Death count: ", Profile->DeathCount)
	};
	
	for(int i = PROFILE_SELECTION_PROFILE; i < PROFILE_SELECTION_MAX; ++i)
	{
		Stats[i] = 
		LoadRenderText(FontM, Text[i-1], RenderObject, true);
		SetRenderTextPos(&Stats[i],
						 &ProfileSelection->StatRect,
						 V2(30, (i-1)*30.0f), TEXT_POSITION_TOP_LEFT);
	}
}

profile_selection 
InitProfileSelectionMenu(player_profile* Profile, 
						 game_renderer* Renderer, 
						 render* Graphics)
{
	v2 WinDim = V2(Graphics->Dim);
	
	TTF_Font* FontL = Renderer->RenderFonts[DEFAULT_FONT_LARGE];
	TTF_Font* FontM = Renderer->RenderFonts[DEFAULT_FONT_MEDIUM];
	object_data RenderObject = Renderer->RenderObjects[R_OBJECT_RECT];
	
	profile_selection Result = {};
	Result.Text[PROFILE_SELECTION_TITLE] = 
	LoadRenderText(FontL, "PROFILE SELECTION", RenderObject);
	SetRenderTextPos(Graphics, &Result.Text[PROFILE_SELECTION_TITLE], 
					 V2(15, 15), TEXT_POSITION_TOP_LEFT);
	
	rect32 StatsRect = {V2(WinDim.x * 0.5f, 100), V2(WinDim.x * 0.5f, 400)};
	Result.StatRect = StatsRect;
	
	LoadProfileStats(Renderer, Result.Text, &Result, Profile);
					
	render_text BackText = LoadRenderText(FontL, "BACK", RenderObject); 
	SetRenderTextPos(Graphics, &BackText, V2(15, -15), 
					 TEXT_POSITION_BOTTOM_LEFT);
	Result.BackButton = CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, BackText);
	
	Result.SelectRect = {V2(15, 100), V2(325, 500)};
	Result.List = GetProfileList();
	Result.Buttons = (button*)malloc(sizeof(button) * (Result.List.Length + 1));
	v4 DefaultColor = Color(150, 150, 150, 0.5f);
	v4 HoverColor = Color(0, 255, 255, 0.5f);
	for(int ButtonIndex = 0; 
		ButtonIndex < Result.List.Length; 
		++ButtonIndex)
	{
		v2 Dim = V2(Result.SelectRect.Dim.x, 100);
		v2 ActualPos = Result.SelectRect.Pos + 
					   V2(Result.SelectRect.Dim.x * 0.5f, 
						  Dim.y*0.5f + (ButtonIndex * Dim.y));
		v2 Pos = V2(Result.SelectRect.Dim.x * 0.5f, 
					(Dim.y * 0.5f) + (ButtonIndex * Dim.y));
		rect32 Rect = WinRect32(Pos, Dim);
		
		render_text ButtonText = 
		LoadRenderText(FontM, 
					  (char*)Result.List.Data[ButtonIndex], 
					  RenderObject);
		SetRenderTextPos(&ButtonText, &Rect, V2(), TEXT_POSITION_CENTERED);
		
		Result.Buttons[ButtonIndex] = 
		CreateButton(UI_BUTTON_UNTEXTURED, 0, 0, ButtonText, 
					 DefaultColor,
					 HoverColor,
					 ActualPos,
					 Dim,
					 Pos);
	}
	
	{
		v2 Dim = V2(Result.SelectRect.Dim.x, 100);
		v2 ActualPos = Result.SelectRect.Pos + 
					   V2(Result.SelectRect.Dim.x * 0.5f, 
						  Dim.y*0.5f + (Result.List.Length * Dim.y));
		v2 Pos = V2(Result.SelectRect.Dim.x * 0.5f, 
					(Dim.y * 0.5f) + (Result.List.Length * Dim.y));
		rect32 Rect = WinRect32(Pos, Dim);
		
		render_text ButtonText = 
		LoadRenderText(FontM, "Create new profile", RenderObject);
		SetRenderTextPos(&ButtonText, &Rect, V2(), TEXT_POSITION_CENTERED);
		
		Result.Buttons[Result.List.Length] = 
		CreateButton(UI_BUTTON_UNTEXTURED, 0, 0, ButtonText,
					 DefaultColor,
					 HoverColor,
					 ActualPos,
					 Dim,
					 Pos);
		//printf("Length: %d\n", Result.List.Length);
		Result.ScrollBar = 
		CreateScrollBar(V2(Result.SelectRect.Dim.x, 
						   Dim.y * (Result.List.Length+1)),
						Result.SelectRect, 20);	
	}
	
	Result.DeleteButton = 
	CreateButton(UI_BUTTON_TEXTURED,
				 Renderer->Assets[ASSET_TRASH_CAN].ID,
				 Renderer->Assets[ASSET_TRASH_CAN_OPEN].ID,
				 {}, Color(), Color(), 
				 WinDim*0.5f + V2(40, 200),
				 V2(100, 100));
					 
	Result.DeleteConfirm = 
	CreateMessageBox(UI_MSGBOX_YESNO, Renderer, "Info",
					 "Are you sure?",
					 WinDim*0.5f, V2(400, 300));
	
	return Result;
}

void FreeProfileSelectionMenu(profile_selection* ProfileSelection)
{
	FreeArray(&ProfileSelection->List);
	if(ProfileSelection->Buttons) free(ProfileSelection->Buttons);
	for(int i = 0; i < PROFILE_SELECTION_MAX; ++i)
	{
		glDeleteTextures(1, &ProfileSelection->Text[i].Texture);
	}
	for(int i = 0; i < ProfileSelection->List.Length+1; ++i)
	{
		glDeleteTextures(1, &ProfileSelection->Buttons[i].Text.Texture);
	}
	glDeleteTextures(1, &ProfileSelection->BackButton.Text.Texture);
}

void UpdateProfileSelectionMenu(profile_selection* ProfileSelection,
								player_profile* Profile,
								game_input* Input,
								game_renderer* Renderer,
								game_sound* Audio,
								render* Graphics)
{
	if(!ProfileSelection->DeleteConfirm.Active)
	{
		v2 MouseState = V2(Input->MouseState);
		int ButtonCount = ProfileSelection->List.Length;
		for(int ButtonIndex = 0;
			ButtonIndex < ButtonCount + 1;
			++ButtonIndex)
		{
			ProfileSelection->Buttons[ButtonIndex].State = UI_BUTTON_STATE_NONE;
		}
		if(IsPointInRect(MouseState, &ProfileSelection->SelectRect))
		{
			HandleScrollBar(&ProfileSelection->ScrollBar, 
							Input,
							Graphics);
			
			for(int ButtonIndex = 0;
				ButtonIndex < ButtonCount + 1;
				++ButtonIndex)
			{
				ProfileSelection->Buttons[ButtonIndex].Pos.y = 
				ProfileSelection->Buttons[ButtonIndex].DefaultPos.y - 
				ProfileSelection->ScrollBar.Camera.y;
				ProfileSelection->Buttons[ButtonIndex].Rect = 
				WinRect32(ProfileSelection->Buttons[ButtonIndex].Pos, 
						  ProfileSelection->Buttons[ButtonIndex].Dim);

				if(!ProfileSelection->ScrollBar.Active)
				HandleButton(&ProfileSelection->Buttons[ButtonIndex], Input, 
							 Audio->SFX[SFX_MENU_CLICK]);
			}
			
			for(int ButtonIndex = 0;
				ButtonIndex < ButtonCount;
				++ButtonIndex)
			{
				if(ProfileSelection->Buttons[ButtonIndex].State == 
				   UI_BUTTON_STATE_CLICK_L)
				{
					char* ProfileName = 
					(char*)ProfileSelection->List.Data[ButtonIndex];
					LoadProfile(Profile, ProfileName);
					
					for(int i = PROFILE_SELECTION_PROFILE;
						i < PROFILE_SELECTION_MAX;
						++i)
					{
						glDeleteTextures(1, &ProfileSelection->Text[i].Texture);
					}
					
					LoadProfileStats(Renderer, ProfileSelection->Text, 
									 ProfileSelection, Profile);
								
					ProfileSelection->Buttons[ButtonIndex].State = 
					UI_BUTTON_STATE_NONE;
					break;
				}
			}
		}
		
		HandleButton(&ProfileSelection->DeleteButton, Input, 
					 Audio->SFX[SFX_MENU_BACK]);
					 
		if(ProfileSelection->DeleteButton.State == 
		   UI_BUTTON_STATE_CLICK_L)
		{
			PromptMessageBox(&ProfileSelection->DeleteConfirm);
		}
		
		HandleButton(&ProfileSelection->BackButton, Input, 
					 Audio->SFX[SFX_MENU_BACK]);
	}
	
	if(ProfileSelection->DeleteConfirm.Active)
	{
		switch(ProcessMessageBox(&ProfileSelection->DeleteConfirm, Input))
		{
			case UI_MSGBOX_STATE_YES:
			{
				char Path[255] = {};
				stbsp_sprintf(Path, "profiles/%s.sav", Profile->Name);
				remove(Path);
				
				FreeArray(&ProfileSelection->List);
				ProfileSelection->List = GetProfileList();
				if(ProfileSelection->List.Length == 0)
				{
					player_profile NewProfile = {};
					NewProfile.Level = 1;
					NewProfile.SkillPoints = 5;
					strcpy(NewProfile.Name, "Default");
					SaveProfile(&NewProfile);
					*Profile = NewProfile;
				}
				else
				{
					LoadProfile(Profile, 
								(char*)ProfileSelection->List.Data[0]);
				}
				FreeProfileSelectionMenu(ProfileSelection);
				*ProfileSelection = 
				InitProfileSelectionMenu(Profile, Renderer, Graphics);
			} break;
		}
	}
}

void RenderProfileSelectionMenu(profile_selection* ProfileSelection,
								game_renderer* Renderer, 
								render* Graphics)
{
	v2 WinDim = V2(Graphics->Dim);
	uint32 TextRenderer = Renderer->Shaders[RENDERER_TEXTURED];
	uint32 PrimitiveRenderer = Renderer->Shaders[RENDERER_PRIMITIVES];
	object_data RenderObject = Renderer->RenderObjects[R_OBJECT_RECT];
	SetScreenSpaceProjection(TextRenderer, WinDim);
	SetScreenSpaceProjection(PrimitiveRenderer, WinDim);
	
	for(int TextIndex = 0; TextIndex < PROFILE_SELECTION_MAX; ++TextIndex)
	{
		RenderText(TextRenderer, 
				   &ProfileSelection->Text[TextIndex],
				   Color(), false);
	}
	
	RenderButton(&ProfileSelection->BackButton, Renderer);
	RenderButton(&ProfileSelection->DeleteButton, Renderer);
	
	{
		v2 Pos = ProfileSelection->SelectRect.Pos + 
				 (ProfileSelection->SelectRect.Dim * 0.5f);
		v3 Dim = V3(ProfileSelection->SelectRect.Dim);
		BasicDraw(PrimitiveRenderer, &RenderObject, V3(Pos), Dim,
				  Color(15, 15, 15, 0.5f), 0, {}, true, true);
	}
	
	v2 ViewPos = ProfileSelection->SelectRect.Pos;
	v2 ViewDim = ProfileSelection->SelectRect.Dim;
	real32 PosY = WinDim.y - ViewPos.y - ViewDim.y;
	glViewport((int)ViewPos.x, 
			   (int)PosY,
			   (int)ViewDim.x,
			   (int)ViewDim.y);
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
							 ViewDim, -ProfileSelection->ScrollBar.Camera);
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_PRIMITIVES],
							 ViewDim, -ProfileSelection->ScrollBar.Camera);
	for(int ButtonIndex = 0; 
		ButtonIndex < ProfileSelection->List.Length+1; 
		++ButtonIndex)
	{
		RenderButton(&ProfileSelection->Buttons[ButtonIndex], Renderer);
	}
	
	glViewport(0, 0, Graphics->Dim.x, Graphics->Dim.y);
	
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_PRIMITIVES],
							 WinDim);
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
							 WinDim);
	RenderScrollBar(&ProfileSelection->ScrollBar,
					WinDim, 
					Renderer->Shaders[RENDERER_PRIMITIVES],
					&Renderer->RenderObjects[R_OBJECT_RECT]);
	
	RenderMessageBox(&ProfileSelection->DeleteConfirm, Renderer);
}