#include "game_upgrade.h"

void UpdateUpgradeMenu(upgrade_menu* UpgradeMenu, player_profile* Profile, 
					   game_renderer* Renderer, render* Graphics)
{
	TTF_Font* Font = Renderer->RenderFonts[DEFAULT_FONT_MEDIUM];
	object_data Object = Renderer->RenderObjects[R_OBJECT_RECT];
	
	glDeleteTextures(1, &UpgradeMenu->SkillPointText.Texture);
	char Buffer[50] = {};
	stbsp_sprintf(Buffer, "Skill points: %d", Profile->SkillPoints);
	UpgradeMenu->SkillPointText = LoadRenderText(Font, Buffer, Object);
	SetRenderTextPos(Graphics, &UpgradeMenu->SkillPointText, 
					 V2(-15, 15), TEXT_POSITION_TOP_RIGHT);
				 
	for(int i = 0; i < UPGRADE_TYPE_MAX; ++i)
	{
		glDeleteTextures(1, &UpgradeMenu->LevelText[i].Texture);
		char Buffer[10] = {};
		stbsp_sprintf(Buffer, "%d", Profile->UpgradeLevels[i]);
		UpgradeMenu->LevelText[i] = LoadRenderText(Font, Buffer, Object);
		SetRenderTextPos(Graphics, &UpgradeMenu->LevelText[i], 
						 V2(300 + 150/2, 
						 UpgradeMenu->UpgradeText[i].Pos.y-
						 UpgradeMenu->LevelText[i].Dim.y*0.5f),
						 TEXT_POSITION_TOP_LEFT);
	}
}

upgrade_menu InitUpgradeMenu(game_renderer* Renderer, render* Graphics, 
							 player_profile* Profile)
{
	TTF_Font* Font = Renderer->RenderFonts[DEFAULT_FONT_MEDIUM];
	TTF_Font* FontL = Renderer->RenderFonts[DEFAULT_FONT_LARGE];
	uint32 Shader = Renderer->Shaders[RENDERER_TEXTURED];
	object_data Object = Renderer->RenderObjects[R_OBJECT_RECT];
	
	upgrade_menu UpgradeMenu = {};
	char* UpgradeText[] = 
	{
		"Attack speed",
		"Health",
		"Meteor spawn",
		"Item spawn",
		"Item duration",
		"Stage duration"
	};
	char* InfoText[] = 
	{
		"Increases laser shooting speed.",
		"Increase HP by 1 each 5 levels. MAX LV: 50",
		"Increase the spawn time for meteor.",
		"Reduce the spawn time for items.",
		"Increase item's duration.",
		"Decrease stage effect duration."
	};
	
	render_text PlusText = LoadRenderText(FontL, "+", Object);
	render_text MinusText = LoadRenderText(FontL, "-", Object);
	
	uint32 InfoTexture = LoadTextureGL("ast/information.png", true, false);
	for(int i = 0; i < UPGRADE_TYPE_MAX; ++i)
	{
		UpgradeMenu.UpgradeText[i] = LoadRenderText(Font, UpgradeText[i], Object);
		SetRenderTextPos(Graphics, &UpgradeMenu.UpgradeText[i], 
						 V2(15, 100 + (i*50.0f)), TEXT_POSITION_TOP_LEFT);
		UpgradeMenu.InfoBlock[i] = 
		CreateHoverBlock(InfoTexture, InfoText[i],
						 Renderer, 
						 V2(250, UpgradeMenu.UpgradeText[i].Pos.y),
						 V2(30, 30), Color(0, 0, 0), true);
						 
		SetRenderTextPos(Graphics, &MinusText, 
						 V2(300, UpgradeMenu.UpgradeText[i].Pos.y-MinusText.Dim.y*0.5f), 
						 TEXT_POSITION_TOP_LEFT);
		UpgradeMenu.Minus[i] = CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, MinusText);
		SetRenderTextPos(Graphics, &PlusText, 
						 V2(450, UpgradeMenu.UpgradeText[i].Pos.y-PlusText.Dim.y*0.5f), 
						 TEXT_POSITION_TOP_LEFT);
		UpgradeMenu.Plus[i] = CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, PlusText);
		
		char Buffer[10] = {};
		stbsp_sprintf(Buffer, "%d", Profile->UpgradeLevels[i]);
		UpgradeMenu.LevelText[i] = LoadRenderText(Font, Buffer, Object);
		SetRenderTextPos(Graphics, &UpgradeMenu.LevelText[i], 
						 V2(300 + 150/2, 
							UpgradeMenu.UpgradeText[i].Pos.y-
							UpgradeMenu.LevelText[i].Dim.y*0.5f),
						 TEXT_POSITION_TOP_LEFT);
	}
	
	UpgradeMenu.TitleText = LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], 
										   "UPGRADES", Object);
	SetRenderTextPos(Graphics, &UpgradeMenu.TitleText, V2(15, 15), TEXT_POSITION_TOP_LEFT);
	
	char Buffer[50] = {};
	stbsp_sprintf(Buffer, "Skill points: %d", Profile->SkillPoints);
	UpgradeMenu.SkillPointText = LoadRenderText(Font, Buffer, Object);
	SetRenderTextPos(Graphics, &UpgradeMenu.SkillPointText, 
					 V2(-15, 15), TEXT_POSITION_TOP_RIGHT);
	
	render_text BackText =
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE], "BACK", Object);
	SetRenderTextPos(Graphics, &BackText, V2(15, -15), TEXT_POSITION_BOTTOM_LEFT);
	UpgradeMenu.BackButton = CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, BackText);
	
	return UpgradeMenu;
}

void RenderUpgradeMenu(upgrade_menu* UpgradeMenu, game_renderer* Renderer, render* Graphics)
{
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
							 Graphics->WinDim);
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_PRIMITIVES],
							 Graphics->WinDim);
	
	RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
			   &UpgradeMenu->TitleText,
			   Color(), false);
	RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
			   &UpgradeMenu->SkillPointText,
			   Color(), false);
	for(int i = 0; i < UPGRADE_TYPE_MAX; ++i)
	{
		RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
				   &UpgradeMenu->UpgradeText[i],
				   Color(), false);
		RenderButton(&UpgradeMenu->Plus[i], Renderer);
		RenderButton(&UpgradeMenu->Minus[i], Renderer);
		RenderText(Renderer->Shaders[RENDERER_TEXTURED],
				   &UpgradeMenu->LevelText[i], Color(), false);
		RenderHoverBlock(&UpgradeMenu->InfoBlock[i], Renderer);
	}
	
	RenderButton(&UpgradeMenu->BackButton, Renderer);
}