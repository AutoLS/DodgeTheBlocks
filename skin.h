#ifndef SKIN_H
#define SKIN_H

enum game_skinning_texture
{
	SKIN_PLAYER,
	SKIN_BACKGROUND,
	SKIN_BLOCK,
	SKIN_ITEM_HP,
	SKIN_ITEM_INVINCIBLE,
	SKIN_ITEM_TIMEWARP,
	SKIN_PARTICLE_SMOKE,
	SKIN_LASER,
	SKIN_MAX,
};

struct game_skin 
{
	array_c List;
	int Count;
	
	char* SelectedSkin;
	texture_data Textures[SKIN_MAX];
};

void OpenSkin(game_skin* Skin, int SkinIndex, char* SkinName, 
			  game_sound* Audio);

game_skin LoadSkin(char* SkinName, game_sound* Audio)
{
	game_skin Skin = {};
	
	Skin.List = InitArray_c(5);
		
	if(!ReadDirectory("skins", &Skin.List))
	{
		if(CreateDirectoryA("skins", 0))
		{
			CreateDirectoryA("skins/Default", 0);
		}
	}
	
	bool SkinExist = false;
	for(int i = 0; i < Skin.List.Length; ++i)
	{
		if(strcmp(Skin.List.Data[i], SkinName) == 0)
		{
			SkinExist = true;
			break;
		}
	}
	if(!SkinExist)
	{
		SkinName = (char*)Skin.List.Data[0];
	}

	OpenSkin(&Skin, 0, SkinName, Audio);
	
	return Skin;
}

void SafeLoadSkinTexture(game_skin* Skin, int ElementIndex, char* ElementName, 
						 bool Update = false,
						 bool Flip = true)
{
	char Buffer[500] = {};
	if(Update)
	{
		stbsp_sprintf(Buffer, "skins/%s/%s.png", 
					  Skin->SelectedSkin, ElementName);
		UpdateTextureGL(&Skin->Textures[ElementIndex], Buffer, true, Flip);
		if(!Skin->Textures[ElementIndex].Success)
		{
			printf("File does not exist: %s\n Now searching for .jpg\n", Buffer);
			stbsp_sprintf(Buffer, "skins/%s/%s.jpg", 
					  Skin->SelectedSkin, ElementName);
			UpdateTextureGL(&Skin->Textures[ElementIndex], Buffer, true, Flip);
			if(!Skin->Textures[ElementIndex].Success)
			{
				printf("File does not exist: %s\n", Buffer);
			}
		}
	}
	else
	{
		stbsp_sprintf(Buffer, "skins/%s/%s.png", 
					  Skin->SelectedSkin, ElementName);
		Skin->Textures[ElementIndex] = 
		LoadTextureGLEX(Buffer, true, Flip);
		if(!Skin->Textures[ElementIndex].Success)
		{
			printf("File does not exist: %s\n Now searching for .jpg\n", Buffer);
			stbsp_sprintf(Buffer, "skins/%s/%s.jpg", 
						  Skin->SelectedSkin, ElementName);
			Skin->Textures[ElementIndex] = 
			LoadTextureGLEX(Buffer, true, Flip);
			if(!Skin->Textures[ElementIndex].Success)
			{
				printf("File does not exist: %s\n", Buffer);
			}
		}
	}
}

void SafeLoadSkinSFX(game_skin* Skin, game_sound* Audio, 
					 int ElementIndex, char* ElementName)
{
	if(Audio->SFX[ElementIndex])
	{
		Mix_FreeChunk(Audio->SFX[ElementIndex]);
	}
	
	char Buffer[500] = {};
	stbsp_sprintf(Buffer, "skins/%s/%s.wav", Skin->SelectedSkin, ElementName);
	Audio->SFX[ElementIndex] = Mix_LoadWAV(Buffer);
	if(!Audio->SFX[ElementIndex])
	{
		printf("Cannot find %s! Make sure it is a .wav file!\n", Buffer);
	}
}

void SafeLoadSkinMUS(game_skin* Skin, game_sound* Audio, 
					 int ElementIndex, char* ElementName)
{
	if(Audio->Music[ElementIndex])
	{
		Mix_FreeMusic(Audio->Music[ElementIndex]);
	}
	
	char Buffer[500] = {};
	stbsp_sprintf(Buffer, "skins/%s/%s.mp3", Skin->SelectedSkin, ElementName);
	Audio->Music[ElementIndex] = Mix_LoadMUS(Buffer);
	if(!Audio->Music[ElementIndex])
	{
		printf("Cannot find %s! Make sure it is a .mp3 file!\n", Buffer);
	}
}

void LoadSkinElements(game_skin* Skin, game_sound* Audio, bool Update = false)
{
	SafeLoadSkinTexture(Skin, SKIN_PLAYER, "player", Update);
	SafeLoadSkinTexture(Skin, SKIN_BLOCK, "block", Update);
	SafeLoadSkinTexture(Skin, SKIN_BACKGROUND, "background", Update, false);
	SafeLoadSkinTexture(Skin, SKIN_ITEM_TIMEWARP, "item_timewarp", Update);
	SafeLoadSkinTexture(Skin, SKIN_ITEM_INVINCIBLE, "item_invincible", Update);
	SafeLoadSkinTexture(Skin, SKIN_ITEM_HP, "item_hp");
	SafeLoadSkinTexture(Skin, SKIN_PARTICLE_SMOKE, "particle_smoke", Update);
	SafeLoadSkinTexture(Skin, SKIN_LASER, "laser", Update);
	
	if(Audio->SFX)
	{
		SafeLoadSkinSFX(Skin, Audio, SFX_MENU_CLICK, "menuclick");
		SafeLoadSkinSFX(Skin, Audio, SFX_MENU_BACK, "menuback");
		SafeLoadSkinSFX(Skin, Audio, SFX_ROLLOVER, "rollover");
		SafeLoadSkinSFX(Skin, Audio, SFX_PLAYER_HIT, "player_hit");
		SafeLoadSkinSFX(Skin, Audio, SFX_POWERUP, "power_up");
		SafeLoadSkinSFX(Skin, Audio, SFX_LASER, "laser");
		SafeLoadSkinSFX(Skin, Audio, SFX_ENEMY_HIT, "enemy_hit");
		SafeLoadSkinSFX(Skin, Audio, SFX_ENEMY_DEAD, "enemy_dead");
		SafeLoadSkinSFX(Skin, Audio, SFX_ENTER_CONDITION, "enter_condition");
		SafeLoadSkinSFX(Skin, Audio, SFX_CONDITION_OVER, "condition_over");
	}
	
	if(Audio->Music)
	{
		SafeLoadSkinMUS(Skin, Audio, MUS_BACKGROUND, "bgm");
		SafeLoadSkinMUS(Skin, Audio, MUS_GAMEOVER, "scoreboard_bgm");
	}
}

void OpenSkin(game_skin* Skin, int SkinIndex, char* SkinName, game_sound* Audio)
{
	if(strlen(SkinName) > 0)
		Skin->SelectedSkin = SkinName;
	else
		Skin->SelectedSkin = (char*)Skin->List.Data[SkinIndex];
	
	Assert(Skin->SelectedSkin);
	LoadSkinElements(Skin, Audio, false);
}

void UpdateSkin(game_skin* Skin, int SkinIndex, game_sound* Audio)
{
	Skin->SelectedSkin = (char*)Skin->List.Data[SkinIndex];
	LoadSkinElements(Skin, Audio, true);
}

#endif