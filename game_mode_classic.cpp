#include "game_mode_classic.h"

void InitModeClassicDiffSelect(game_mode_classic* ModeClassic,
							   game_renderer* Renderer,
							   render* Graphics)
{
	for(int i = 0; i < MC_DIFF_BUTTON_MAX; ++i)
	{
		if(ModeClassic->DiffSelect.Buttons[i].Text.Texture)
		{
			glDeleteTextures(1, 
			&ModeClassic->DiffSelect.Buttons[i].Text.Texture);
		}
	}
	
	mc_diff_select Result = {};
	
	Result.SelectDiffText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
				   "Select a difficulty",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &Result.SelectDiffText, V2(0, -200));
	
	render_text Text[MC_DIFF_BUTTON_MAX] = {};
	
	Text[MC_DIFF_BUTTON_BACK] = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
				   "BACK",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &Text[MC_DIFF_BUTTON_BACK], V2(15, -15),
					 TEXT_POSITION_BOTTOM_LEFT);
	
	char* DifficultyText = 0;
	
	switch(ModeClassic->Difficulty)
	{
		case DIFFICULTY_EASY: DifficultyText = "EASY"; break;
		case DIFFICULTY_NORMAL: DifficultyText = "NORMAL"; break;
		case DIFFICULTY_HARD: DifficultyText = "HARD"; break;
		default: 
		DifficultyText = "EASY";
		ModeClassic->Difficulty = DIFFICULTY_EASY;
	}
	
	Text[MC_DIFF_BUTTON_START] = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
				   DifficultyText,
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &Text[MC_DIFF_BUTTON_START]);
	
	Text[MC_DIFF_BUTTON_LEFT] = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
				   "<",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &Text[MC_DIFF_BUTTON_LEFT], V2(-150));
	
	Text[MC_DIFF_BUTTON_RIGHT] = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
				   ">",
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &Text[MC_DIFF_BUTTON_RIGHT], V2(150));
	
	Result.Buttons[MC_DIFF_BUTTON_BACK] =
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, Text[MC_DIFF_BUTTON_BACK]);
	
	Result.Buttons[MC_DIFF_BUTTON_START] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, Text[MC_DIFF_BUTTON_START]);
	
	Result.Buttons[MC_DIFF_BUTTON_LEFT] =
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, Text[MC_DIFF_BUTTON_LEFT]);
	
	Result.Buttons[MC_DIFF_BUTTON_RIGHT] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, Text[MC_DIFF_BUTTON_RIGHT]);
	
	ModeClassic->DiffSelect = Result;
}

void UpdateMCDiffSelectText(mc_diff_select* DiffSelect,
							game_renderer* Renderer,
							game_difficulty Difficulty,
							render* Graphics)
{
	char* DifficultyText = 0;
	
	switch(Difficulty)
	{
		case DIFFICULTY_EASY: DifficultyText = "EASY"; break;
		case DIFFICULTY_NORMAL: DifficultyText = "NORMAL"; break;
		case DIFFICULTY_HARD: DifficultyText = "HARD"; break;
	}
	
	glDeleteTextures(1, 
	&DiffSelect->Buttons[MC_DIFF_BUTTON_START].Text.Texture);
	
	render_text StartText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
				   DifficultyText,
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(Graphics, &StartText);
	
	DiffSelect->Buttons[MC_DIFF_BUTTON_START] = 
	CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, StartText);
}

void UpdateMCDifficultySelection(game_mode_classic* ModeClassic,
								 game_renderer* Renderer,
								 game_input* Input,
								 entity_pool* Pool,
								 render* Graphics,
								 Mix_Chunk** SFX)
{
	v2 WinDim = V2(Graphics->Dim);
	
	mc_diff_select* DiffSelect = &ModeClassic->DiffSelect;
	
	HandleButton(&DiffSelect->Buttons[MC_DIFF_BUTTON_LEFT], 
				 Input, 
				 SFX[SFX_ROLLOVER]);
	HandleButton(&DiffSelect->Buttons[MC_DIFF_BUTTON_RIGHT], 
				 Input, 
				 SFX[SFX_ROLLOVER]);
	HandleButton(&DiffSelect->Buttons[MC_DIFF_BUTTON_START], 
				 Input, 
				 SFX[SFX_MENU_CLICK]);
	HandleButton(&DiffSelect->Buttons[MC_DIFF_BUTTON_BACK], 
				 Input, 
				 SFX[SFX_MENU_BACK]);
	
	for(int ButtonIndex = 0; 
		ButtonIndex < MC_DIFF_BUTTON_MAX; 
		++ButtonIndex)
	{
		if(DiffSelect->Buttons[ButtonIndex].State == 
		   UI_BUTTON_STATE_CLICK_L)
		{
			switch(ButtonIndex)
			{
				case MC_DIFF_BUTTON_LEFT:
				{
					if(ModeClassic->Difficulty > DIFFICULTY_EASY)
					{
						ModeClassic->Difficulty = 
						(game_difficulty)(ModeClassic->Difficulty - 1);
						UpdateMCDiffSelectText(DiffSelect,
											   Renderer,
											   ModeClassic->Difficulty,
											   Graphics);
					}
				} break;
				case MC_DIFF_BUTTON_RIGHT:
				{
					if(ModeClassic->Difficulty < DIFFICULTY_HARD)
					{
						ModeClassic->Difficulty = 
						(game_difficulty)(ModeClassic->Difficulty + 1);
						UpdateMCDiffSelectText(DiffSelect,
											   Renderer,
											   ModeClassic->Difficulty,
											   Graphics);
					}
				} break;
			}
		}
	}
}

void RenderMCDiffSelection(game_mode_classic* ModeClassic,
						   game_renderer* Renderer, 
						   render* Graphics)
{
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
							 V2(Graphics->Dim));
	
	RenderText(Renderer->Shaders[RENDERER_TEXTURED],
			   &ModeClassic->DiffSelect.SelectDiffText,
			   Color(), false);
	
	RenderButtons(ModeClassic->DiffSelect.Buttons, MC_DIFF_BUTTON_MAX,
				  Renderer->Shaders[RENDERER_TEXTURED]);
}

real32 GetUpgradePercentage(int Level)
{
	if(!Level) return 0;
	return 0.05f + logf((real32)Level)*0.1f;
}

mc_block_properties SetBlockProperties(game_difficulty Difficulty)
{
	mc_block_properties Block = {};
	switch(Difficulty)
	{
		case DIFFICULTY_EASY:
		{
			Block.SpawnTime = 0.3f;
			Block.FallSpeed = 225;
			Block.MaxSpeed = 800;
			Block.MaxBlock = 50;
			Block.nPerSpawn = 2;
		} break;
		case DIFFICULTY_NORMAL:
		{
			Block.SpawnTime = 0.25f;
			Block.FallSpeed = 300;
			Block.MaxSpeed = 900;
			Block.MaxBlock = 75;
			Block.nPerSpawn = 3;
		} break;
		case DIFFICULTY_HARD:
		{
			Block.SpawnTime = 0.15f;
			Block.FallSpeed = 500;
			Block.MaxSpeed = 1000;
			Block.MaxBlock = 100;
			Block.nPerSpawn = 3;
		} break;
	}
	
	Block.DefaultFallSpeed = Block.FallSpeed;
	Block.DefaultMaxBlock = Block.MaxBlock;
	
	return Block;
}

mc_meteor_properties SetMeteorProperties(game_difficulty Difficulty, player_profile* Profile)
{
	mc_meteor_properties Meteor = {};
	real32 MeteorSpawnTimePercentage = 
	GetUpgradePercentage(Profile->UpgradeLevels[UPGRADE_TYPE_METEOR_SPAWN]);
	switch(Difficulty)
	{
		case DIFFICULTY_EASY:
		{
			Meteor.SpawnTime = 20.0f + (50.0f * MeteorSpawnTimePercentage);
			Meteor.Speed = 700.0f;
			Meteor.nFragments = 6;
		} break;
		case DIFFICULTY_NORMAL:
		{
			Meteor.SpawnTime = 15.0f + (50.0f * MeteorSpawnTimePercentage);
			Meteor.Speed = 800.0f;
			Meteor.nFragments = 9;
		} break;
		case DIFFICULTY_HARD:
		{
			Meteor.SpawnTime = 10.0f + (50.0f * MeteorSpawnTimePercentage);
			Meteor.Speed = 900.0f;
			Meteor.nFragments = 12;
		} break;
	}
	
	return Meteor;
}

mc_game_info SetGameInfo(game_difficulty Difficulty, player_profile* Profile)
{
	mc_game_info Result = {};
	
	real32 StageDuration = 5.0f;
	real32 StageDurationPercentage = 
	GetUpgradePercentage(Profile->UpgradeLevels[UPGRADE_TYPE_STAGE_DURATION]);
	for(int i = 0; i < MC_STAGE_EFFECT_MAX; ++i)
	{
		Result.StageDuration[i] = StageDuration - (StageDuration * StageDurationPercentage);
	}
	real32 ItemSpawnTime = 30.0f;
	real32 ItemSpawnPercentage =
	GetUpgradePercentage(Profile->UpgradeLevels[UPGRADE_TYPE_ITEM_SPAWN]);	
	Result.ItemSpawnTime = ItemSpawnTime - (ItemSpawnTime*ItemSpawnPercentage);
	switch(Difficulty)
	{
		case DIFFICULTY_EASY:
		{
			Result.ScorePerTick = 500;
			Result.ScoreMultiplier = 1.06f;
		} break;
		case DIFFICULTY_NORMAL:
		{
			Result.ScorePerTick = 1000;
			Result.ScoreMultiplier = 1.12f;
		} break;
		case DIFFICULTY_HARD:
		{
			Result.ScorePerTick = 2500;
			Result.ScoreMultiplier = 1.18f;
		} break;
	}
	
	return Result;
}

mc_player_info SetPlayerInfo(real32 Speed, int HP, player_profile* Profile)
{
	mc_player_info Player = {};
	Player.Speed = Speed;
	Player.Health = HP + Profile->UpgradeLevels[UPGRADE_TYPE_HP]/5;
	real32 AtkSpdPercentage = 
	GetUpgradePercentage(Profile->UpgradeLevels[UPGRADE_TYPE_ATK_SPD]);
	real32 AtkSpd = 0.25f - (0.25f * AtkSpdPercentage);
	AtkSpd = AtkSpd < 0 ? 0 : AtkSpd;
	Player.ShootTimer = CreateTimer(AtkSpd);
	real32 TimeWarpDuration = 5.0f;
	real32 InvincibleDuration = 3.0f;
	real32 PowerupPercentage = 
	GetUpgradePercentage(Profile->UpgradeLevels[UPGRADE_TYPE_ITEM_DURATION]);
	Player.PowerupTimers[MC_ITEM_TIMEWARP] = 
	CreateTimer(TimeWarpDuration + (TimeWarpDuration * PowerupPercentage), false);
	Player.PowerupTimers[MC_ITEM_INVINCIBLE] = 
	CreateTimer(InvincibleDuration + (InvincibleDuration * PowerupPercentage), false);
	
	return Player;
}

bool PlayerOutOfHealth(mc_player_info* PlayerInfo)
{
	return PlayerInfo->Health <= 0;
}

void InitModeClassic(game_mode_classic* ModeClassic,
					 player_profile* Profile,
					 game_renderer* Renderer, 
					 texture_data* Textures,
					 v2 PlayArea,
					 entity_pool* Pool, 
					 game_difficulty Difficulty)
{
	game_mode_classic Result = {};
	Result.PlayArea = PlayArea;
	Result.Difficulty = Difficulty;
	Result.BlockSettings = SetBlockProperties(Difficulty);
	Result.MeteorSettings = SetMeteorProperties(Difficulty, Profile);
	Result.GameInfo = SetGameInfo(Difficulty, Profile);
	Result.PlayerInfo = SetPlayerInfo(Result.BlockSettings.FallSpeed*0.8f, 1, Profile);
	
	if(ModeClassic->ParticleGenerator.Particles)
		ClearParticleGenerator(&ModeClassic->ParticleGenerator);
	Result.ParticleGenerator = CreateParticleGenerator(1000, 30);
	
	CreateTimers(Result.ParticleSpawnTimers, MC_PARTICLE_MAX, 0.05f);
	
	Result.Timers[MC_GAME_TIMER] = CreateTimer(0);
	Result.Timers[MC_SB_ANIMATION_TIMER] = CreateTimer(0, false);
	
	Result.Timers[MC_BLOCK_SPAWN_TIMER] = 
	CreateTimer(Result.BlockSettings.SpawnTime);
	
	Result.Timers[MC_METEOR_SPAWN_TIMER] = 
	CreateTimer(Result.MeteorSettings.SpawnTime);
	
	Result.Timers[MC_ENEMY_SPAWN_TIMER] = CreateTimer(15.0f);
	
	Result.Timers[MC_SCORE_TICK_TIMER] = CreateTimer(1.0f);
	
	Result.Timers[MC_ITEM_DROP_TIMER] = 
	CreateTimer(Result.GameInfo.ItemSpawnTime);
	
	Result.ScoreBoardScrollBar = CreateScrollBar(V2(50, 1000), 
							     {V2(), V2(PlayArea.x, 350)}, 20);
	
	if(ModeClassic->SpawnedBlockIndices)
		free(ModeClassic->SpawnedBlockIndices);
	
	Result.SpawnedBlockIndices = 
	(int*)malloc(sizeof(int) * Result.BlockSettings.nPerSpawn);
	
	ClearEntityPool(Pool);
	
	rigid_body PlayerBody = InitBody(V2(0, 0), V2(), V2(30, 30),
									 10, 90, POLYGON_TRIANGLE);
	Result.PlayerInfo.PlayerIndex =
	AddEntity(Pool, PlayerBody,
			  &Renderer->RenderObjects[R_OBJECT_RECT], 
			  &Textures[SKIN_PLAYER],
			  Color(),
			  ENTITY_PLAYER);
	*ModeClassic = Result;
}

void SpawnBlockCustom(entity_pool* Pool, game_renderer* Renderer, 
					   v2 Pos, v2 Vel, v2 Dim, texture_data* Texture,
					   entity_type Type)
{
	rigid_body Block = InitBody(Pos, Vel, Dim, 10, 0, 
								POLYGON_RECTANGLE);
								
	AddEntity(Pool, Block, &Renderer->RenderObjects[R_OBJECT_RECT],
			  Texture, Color(255),
			  Type);
}

void SpawnBlocksModeClassic(entity_pool* Pool, game_mode_classic* ModeClassic,
							game_renderer* Renderer, texture_data* Texture)
{
	timer* Timer = &ModeClassic->Timers[MC_BLOCK_SPAWN_TIMER];

	if(Timer->Complete)
	{
		StartTimer(Timer);
		mc_block_properties* BlockSetting = &ModeClassic->BlockSettings;
		
		if(BlockSetting->BlockCount < BlockSetting->MaxBlock)
		{
			for(uint32 i = 0; i < BlockSetting->nPerSpawn; ++i)
			{
				v2 Area = ModeClassic->PlayArea * 0.5f;
				v2 Pos = V2(Rand32(-Area.x, Area.x), ModeClassic->PlayArea.y);
				v2 Vel = V2(0, -1) * BlockSetting->FallSpeed;
				
				real32 Width = Rand32(30, 50);
				real32 Height = Width;
				v2 Dim = V2(Width, Height);
				
				rigid_body Block = InitBody(Pos, Vel, Dim, 10, Rand32(0, 360), POLYGON_RECTANGLE);
				int Index = AddEntity(Pool, Block, 
									  &Renderer->RenderObjects[R_OBJECT_RECT],
									  Texture,
									  Color(255, 0, 0),
									  ENTITY_BLOCK);
				++BlockSetting->BlockCount;
				
				ModeClassic->SpawnedBlockIndices[i] = Index;
			}
		}
	}
}

void SetMeteorTargetModeClassic(entity* Player, 
								game_mode_classic* ModeClassic)
{
	timer* Timer = &ModeClassic->Timers[MC_METEOR_SPAWN_TIMER];
	mc_meteor_properties* MeteorSettings = &ModeClassic->MeteorSettings;
	
	if(Timer->Time + 1.0f > Timer->EndTime && 
		!MeteorSettings->IsTargetLocked)
	{
		MeteorSettings->SpawnPos = 
		V2(Rand32(-ModeClassic->PlayArea.x*0.5f, 
				  ModeClassic->PlayArea.x*0.5f),
		   ModeClassic->PlayArea.y);
		MeteorSettings->Dir = 
		Normalize(Player->Body.Pos - MeteorSettings->SpawnPos);
		MeteorSettings->IsTargetLocked = true;
	}
}

void SpawnMeteorModeClassic(entity_pool* Pool, game_mode_classic* ModeClassic,
							game_renderer* Renderer, texture_data* Texture)
{
	timer* Timer = &ModeClassic->Timers[MC_METEOR_SPAWN_TIMER];
	mc_meteor_properties* MeteorSettings = &ModeClassic->MeteorSettings;

	if(Timer->Complete && MeteorSettings->IsTargetLocked)
	{
		StartTimer(Timer);
		
		v2 Vel = MeteorSettings->Dir * MeteorSettings->Speed;
		rigid_body MeteorBody = InitBody(MeteorSettings->SpawnPos,
										 Vel,
										 V2(50, 50),
										 10, 0, POLYGON_RECTANGLE);
		AddEntity(Pool, MeteorBody, 
				  &Renderer->RenderObjects[R_OBJECT_RECT],
				  Texture,
				  Color(255, 0, 0),
				  ENTITY_BLOCK_METEOR);
		MeteorSettings->IsTargetLocked = false;
	}
}

void CreateEnemy(game_mode_classic* ModeClassic, entity_pool* Pool, 
				 entity_type Type,
				 int AssetID,
				 real32 Scale,
				 game_renderer* Renderer)
{
	mc_enemy_info* Enemy = &ModeClassic->Enemy;
	
	v2 Area = ModeClassic->PlayArea * 0.5f;
	v2 Pos = V2(Rand32(-Area.x*0.8f, Area.x*0.8f), ModeClassic->PlayArea.y);
	v2 Vel = V2(0, -200);
	rigid_body EnemyBody = 
	InitBody(Pos, Vel, Renderer->Assets[AssetID].Dim * Scale,
			 10, 0, POLYGON_RECTANGLE);
			 
	Enemy->EntityIndex = 
	AddEntity(Pool, EnemyBody, 
			  &Renderer->RenderObjects[R_OBJECT_RECT],
			  &Renderer->Assets[AssetID],
			  Color(),
			  Type);
	Enemy->Active = true;
	Enemy->HP = 10;
}

void SpawnEnemyModeClassic(game_mode_classic* ModeClassic, 
						   game_renderer* Renderer,
						   mc_enemy_info* Enemy,
						   entity_pool* Pool,
						   entity_type Type)
{
	if(ModeClassic->Timers[MC_ENEMY_SPAWN_TIMER].Complete)
	{
		Enemy->Type = Type;
		StartTimer(&ModeClassic->Timers[MC_ENEMY_SPAWN_TIMER]);
		switch(Type)
		{
			case ENTITY_ENEMY_1:
			{
				CreateEnemy(ModeClassic, Pool, Type, ASSET_ENEMY_1, 1, Renderer);
			} break;
			case ENTITY_ENEMY_2:
			{
				CreateEnemy(ModeClassic, Pool, Type, ASSET_ENEMY_2, 1, Renderer);
			} break;
			case ENTITY_ENEMY_3:
			{
				CreateEnemy(ModeClassic, Pool, Type, ASSET_ENEMY_3, 0.5f,
							Renderer);
			} break;
			default:
			{
				
			}
		}
	}
}

void EnterStageEffect(game_mode_classic* ModeClassic, 
					  mc_stage_effects Type, real32 Duration)
{
	ModeClassic->Enemy.Active = false;
	ModeClassic->StageEffects[Type] = true;
	ModeClassic->StageEffectTimers[Type] = CreateTimer(Duration);
}

void ProcessStageEffect(mc_stage_effects Type, 
						game_mode_classic* ModeClassic, 
						game_sound* Audio)
{
	if(ModeClassic->StageEffectTimers[Type].Complete)
	{
		ModeClassic->StageEffectTimers[Type].Complete = false;
		ModeClassic->StageEffects[Type] = false;
		PlayChannelSafe(Audio->SFX[SFX_CONDITION_OVER]);
	}
}

void SpawnItemModeClassic(game_mode_classic* ModeClassic,
						  game_renderer* Renderer,
						  texture_data* Textures,
						  entity_pool* Pool, 
						  mode_classic_item ItemType)
{
	timer* Timer = &ModeClassic->Timers[MC_ITEM_DROP_TIMER];
	timer* GameTimer = &ModeClassic->Timers[MC_GAME_TIMER];
	if(Timer->Complete && GameTimer->Time > 15.0f)
	{
		StartTimer(Timer);
		ModeClassic->CurrentItemDrop = ItemType;
		
		mc_block_properties* BlockSetting = &ModeClassic->BlockSettings;
		
		v2 Area = ModeClassic->PlayArea * 0.5f;
		v2 Pos = V2(Rand32(-Area.x, Area.x), ModeClassic->PlayArea.y);
		v2 Vel = V2(0, -1) * BlockSetting->FallSpeed;
		
		real32 Width = 30;
		real32 Height = 30;
		v2 Dim = V2(Width, Height);
		
		texture_data* Texture;
		switch(ItemType)
		{
			case MC_ITEM_TIMEWARP: 
			Texture = &Textures[SKIN_ITEM_TIMEWARP]; break;
			case MC_ITEM_INVINCIBLE: 
			Texture = &Textures[SKIN_ITEM_INVINCIBLE]; break;
			case MC_ITEM_HEALTH_DROP: 
			Texture = &Textures[SKIN_ITEM_HP]; break;
		}
		
		rigid_body ItemBody = InitBody(Pos, Vel, Dim, 10, 0, POLYGON_RECTANGLE);
		int Index = AddEntity(Pool, ItemBody, 
							  &Renderer->RenderObjects[R_OBJECT_RECT],
							  Texture,
							  Color(), ENTITY_ITEM);
		
		entity* Item = &Pool->Entities[Index];
		int* Indices = ModeClassic->SpawnedBlockIndices;
		for(uint32 i = 0; i < BlockSetting->nPerSpawn; ++i)
		{
			entity* Block = &Pool->Entities[Indices[i]];
			collide_info Collide = TestSAT(&Item->Body.Shape, 
										   &Block->Body.Shape);
			if(Collide.Collide)
			{
				Item->Body.Pos += Collide.d;
				UpdateVertices(&Item->Body.Shape, Collide.d);
			}
		}
	}
}

void ScoreTickModeClassic(game_mode_classic* ModeClassic)
{
	timer* Timer = &ModeClassic->Timers[MC_SCORE_TICK_TIMER];
	if(Timer->Complete)
	{
		StartTimer(Timer);
		mc_game_info* GameInfo = &ModeClassic->GameInfo;
		mc_player_info* PlayerInfo = &ModeClassic->PlayerInfo;
		mc_block_properties* BlockSettings = &ModeClassic->BlockSettings;
		
		real32 t = ModeClassic->Timers[MC_GAME_TIMER].Time;
		
		++GameInfo->ScoreTicks;
		GameInfo->Score += GameInfo->ScorePerTick;
		GameInfo->ScorePerTick += logf(t) * GameInfo->ScoreMultiplier;
		
		if(BlockSettings->SpawnTime > 0.01f)
		{
			BlockSettings->SpawnTime -= 0.0001f;
			ModeClassic->Timers[MC_BLOCK_SPAWN_TIMER] = 
			CreateTimer(BlockSettings->SpawnTime);
		}
		
		if(BlockSettings->FallSpeed < BlockSettings->MaxSpeed)
		{
			if(t > 1.0f)
			{
				BlockSettings->FallSpeed = BlockSettings->DefaultFallSpeed + 
				(logf(t) * GameInfo->ScoreMultiplier);
				PlayerInfo->Speed = BlockSettings->FallSpeed * 0.8f;
				if(BlockSettings->FallSpeed > BlockSettings->MaxSpeed)
				{
					BlockSettings->FallSpeed = BlockSettings->MaxSpeed;
				}
			}
		}
		
		//printf("Score: %d Health: %d\n", GameInfo->Score, 
			//							 PlayerInfo->Health);
	}
}

void PlayerHitByEnemy(game_mode_classic* ModeClassic, 
			   entity_pool* Pool,
			   int Index,
			   mc_stage_effects StageEffect,
			   real32 Duration,
			   Mix_Chunk** SFX)
{
	mc_player_info* PlayerInfo = &ModeClassic->PlayerInfo;
	KillEntity(Pool, Index);
	if(!PlayerInfo->PowerupTimers[MC_ITEM_INVINCIBLE].Active)
	{
		--PlayerInfo->Health;
		++PlayerInfo->DeathCount;
		PlayChannelSafe(SFX[SFX_ENTER_CONDITION]);
		EnterStageEffect(ModeClassic, StageEffect, Duration);
	}
}

void UpdatePlayerModeClassic(entity_pool* Pool, texture_data* Textures,
							 game_renderer* Renderer,
							 game_sound* Audio,
							 game_mode_classic* ModeClassic, 
							 game_input_map* Input, real32 t)
{
	mc_player_info* PlayerInfo = &ModeClassic->PlayerInfo;
	entity* Player = &Pool->Entities[PlayerInfo->PlayerIndex];
	Player->Body.Vel = V2();
	
	if(Input->Left.IsDown)
		Player->Body.Vel += V2(-1) * PlayerInfo->Speed;
	if(Input->Right.IsDown)
		Player->Body.Vel += V2(1) * PlayerInfo->Speed;
	if(Input->Up.IsDown)
		Player->Body.Vel += V2(0, 1) * PlayerInfo->Speed;
	if(Input->Down.IsDown)
		Player->Body.Vel += V2(0, -1) * PlayerInfo->Speed;
	if(Input->Dash.IsDown)
	{
		timer* ParticleTimer = ModeClassic->ParticleSpawnTimers;
		if(ParticleTimer[MC_PARTICLE_AFTERIMAGE].Complete)
		{
			v4 RenderColor = 
			PlayerInfo->PowerupTimers[MC_ITEM_INVINCIBLE].Active ?
			Color(255, 255, 255, 0.4f) : Color();
			StartTimer(&ParticleTimer[MC_PARTICLE_AFTERIMAGE]);
			SpawnParticle(&ModeClassic->ParticleGenerator, 
						  Player->Body.Pos, Player->Body.Shape.Dim,
						  1000,
						  Textures[SKIN_PLAYER],
						  RenderColor);
		}
		Player->Body.Vel = Player->Body.Vel * 2;
	}
	if(Input->Shrink.IsDown)
	{
		ResizeVertices(&Player->Body.Shape, 
					   Player->Body.Pos, 
					   Player->Body.Dim * 0.5f);
		Player->Body.Vel = Player->Body.Vel * 0.5f;
	}
	else
	{
		ResizeVertices(&Player->Body.Shape, 
					   Player->Body.Pos, 
					   Player->Body.Dim);
	}
	if(Input->Shoot.IsDown)
	{
		if(PlayerInfo->ShootTimer.Complete)
		{
			StartTimer(&PlayerInfo->ShootTimer);
			PlayChannelSafe(Audio->SFX[SFX_LASER]);
			real32 Scale = Input->Shrink.IsDown ? 0.5f:1.0f;
			v2 PosA = Player->Body.Pos + 
					  V2(-Player->Body.Shape.Dim.x*0.3f, 
						 Player->Body.Shape.Dim.y*0.7f);
			v2 PosB = Player->Body.Pos + 
					  V2(Player->Body.Shape.Dim.x*0.3f, 
						 Player->Body.Shape.Dim.y*0.7f);
			v2 Dim = V2(9, 33) * Scale;
			
			v2 Vel = V2(0, 1000);
			
			rigid_body Laser = InitBody(PosA, Vel, Dim, 5, 0, POLYGON_RECTANGLE);
			AddEntity(Pool, Laser, &Renderer->RenderObjects[R_OBJECT_RECT],
					  &Textures[SKIN_LASER], Color(), ENTITY_LASER);
			Laser = InitBody(PosB, Vel, Dim, 5, 0, POLYGON_RECTANGLE);
			AddEntity(Pool, Laser, &Renderer->RenderObjects[R_OBJECT_RECT],
					  &Textures[SKIN_LASER], Color(), ENTITY_LASER);
		}
	}

	IntegrateLinearMotion(&Player->Body, t);
	
	if(Player->Body.Pos.x > ModeClassic->PlayArea.x*0.5f)
	{
		v2 NewPos = V2(-ModeClassic->PlayArea.x*0.5f, Player->Body.Pos.y);
		SetBodyPosition(&Player->Body, NewPos);
	}
	if(Player->Body.Pos.x < -ModeClassic->PlayArea.x*0.5f)
	{
		v2 NewPos = V2(ModeClassic->PlayArea.x*0.5f, Player->Body.Pos.y);
		SetBodyPosition(&Player->Body, NewPos);
	}
	if(Player->Body.Pos.y - (Player->Body.Dim.y * 0.5f) < 0)
	{
		v2 NewPos = V2(Player->Body.Pos.x, Player->Body.Dim.y * 0.5f);
		SetBodyPosition(&Player->Body, NewPos);
	}
	if(Player->Body.Pos.y + (Player->Body.Dim.y * 0.5f) > 
		ModeClassic->PlayArea.y)
	{
		v2 NewPos = V2(Player->Body.Pos.x, ModeClassic->PlayArea.y - 
					  (Player->Body.Dim.y * 0.5f));
		SetBodyPosition(&Player->Body, NewPos);
	}
}


void UpdateEntitiesModeClassic(entity_pool* Pool, 
							   game_mode_classic* ModeClassic, 
							   game_renderer* Renderer,
							   game_sound* Audio,
							   texture_data* Texture,
							   game_input_map* Input, 
							   real32 t)
{
	for(uint32 i = 0; i < Pool->UsedEntities; ++i)
	{
		entity* Entity = &Pool->Entities[i];
		if(Entity->Alive)
		{
			switch(Entity->Type)
			{
				case ENTITY_PLAYER:
				{
					UpdatePlayerModeClassic(Pool, Texture, Renderer,
											Audio, ModeClassic, 
											Input, t);
				} break;
				case ENTITY_BLOCK:
				{
					mc_block_properties* BlockSetting = 
					&ModeClassic->BlockSettings;
					IntegrateLinearMotion(&Entity->Body, t);
					
					v2 Min = V2(-ModeClassic->PlayArea.x * 0.5f - 
								Entity->Body.Dim.x, -Entity->Body.Dim.y);
					v2 Max = V2(ModeClassic->PlayArea.x * 0.5f + 
								Entity->Body.Dim.x, 
								ModeClassic->PlayArea.y + Entity->Body.Dim.y);
					if(OutOfBound(Entity, Min, Max))
					{
						KillEntity(Pool, i);
						--BlockSetting->BlockCount;
						++ModeClassic->PlayerInfo.nBlocksDodged;
					}
				} break;
				case ENTITY_BLOCK_METEOR:
				{
					mc_meteor_properties* MeteorSettings = 
					&ModeClassic->MeteorSettings;
					
					timer* ParticleTimer = ModeClassic->ParticleSpawnTimers;
					if(ParticleTimer[MC_PARTICLE_SMOKE].Complete)
					{
						StartTimer(&ParticleTimer[MC_PARTICLE_SMOKE]);
						for(int i = 0; i < 3; ++i)
						{
							v2 Offset = V2(Rand32(-5, 5), Rand32(-5, 5));
							SpawnParticle(&ModeClassic->ParticleGenerator, 
										  Entity->Body.Pos + Offset,
										  Entity->Body.Dim * 0.5f, 
										  500,
										  Texture[SKIN_PARTICLE_SMOKE],
										  Color(255, 128, 0));
						}
					}
					
					IntegrateLinearMotion(&Entity->Body, t);
					
					v2 Min = V2(-ModeClassic->PlayArea.x * 0.5f - 
								Entity->Body.Dim.x, 0);
					v2 Max = V2(ModeClassic->PlayArea.x * 0.5f + 
								Entity->Body.Dim.x, 
								ModeClassic->PlayArea.y + Entity->Body.Dim.y);
					
					if(OutOfBound(Entity, Min, Max))
					{
						v2 Pos = Entity->Body.Pos;
						PrintV2(Pos);
						
						KillEntity(Pool, i);
						
						real32 Theta = 0;
						real32 DeltaTheta = 360.0f / MeteorSettings->nFragments;
						for(int i = 0; i < MeteorSettings->nFragments; ++i)
						{
							v2 Vel = V2(cosf(Radians(Theta)), 
										sinf(Radians(Theta))) * 
										MeteorSettings->Speed * 0.3f;
							v2 Dim = V2(20, 20);
							
							SpawnBlockCustom(Pool, Renderer, 
											 Pos, Vel, Dim, 
											 &Texture[SKIN_BLOCK],
											 ENTITY_METEOR_FRAGMENTS);
							assert(MeteorSettings->nFragments != 0);
							Theta += DeltaTheta;
						}
						
						++ModeClassic->PlayerInfo.nBlocksDodged;
					}
				} break;
				case ENTITY_ENEMY_1:
				{
					Entity->RenderColor = Color();
					IntegrateLinearMotion(&Entity->Body, t);
					v2 Min = V2(-ModeClassic->PlayArea.x, 0);
					v2 Max = V2(ModeClassic->PlayArea.x, 
								ModeClassic->PlayArea.y * 2);
					
					if(OutOfBound(Entity, Min, Max))
					{
						KillEntity(Pool, i);
						PlayChannelSafe(Audio->SFX[SFX_ENTER_CONDITION]);
						EnterStageEffect(ModeClassic, MC_STAGE_REVERSE, 
										 ModeClassic->GameInfo.
										 StageDuration[MC_STAGE_REVERSE]);
					}
				} break;
				case ENTITY_ENEMY_2:
				{
					Entity->RenderColor = Color();
					IntegrateLinearMotion(&Entity->Body, t);
					v2 Min = V2(-ModeClassic->PlayArea.x, 0);
					v2 Max = V2(ModeClassic->PlayArea.x, 
								ModeClassic->PlayArea.y * 2);
					
					if(OutOfBound(Entity, Min, Max))
					{
						KillEntity(Pool, i);
						PlayChannelSafe(Audio->SFX[SFX_ENTER_CONDITION]);
						EnterStageEffect(ModeClassic, MC_STAGE_FLASHLIGHT, 
										 ModeClassic->GameInfo.
										 StageDuration[MC_STAGE_FLASHLIGHT]);
					}
				} break;
				case ENTITY_ENEMY_3:
				{
					Entity->RenderColor = Color();
					IntegrateLinearMotion(&Entity->Body, t);
					v2 Min = V2(-ModeClassic->PlayArea.x, 0);
					v2 Max = V2(ModeClassic->PlayArea.x, 
								ModeClassic->PlayArea.y * 2);
					
					if(OutOfBound(Entity, Min, Max))
					{
						KillEntity(Pool, i);
						PlayChannelSafe(Audio->SFX[SFX_ENTER_CONDITION]);
						EnterStageEffect(ModeClassic, MC_STAGE_HIDDEN, 
										 ModeClassic->GameInfo.
										 StageDuration[MC_STAGE_HIDDEN]);
					}
				} break;
				case ENTITY_LASER:
				{
					Entity->Body.Force = V2(0, 9800);
					IntegrateLinearMotion(&Entity->Body, t);
					
					v2 Min = V2(-ModeClassic->PlayArea.x, 
								-ModeClassic->PlayArea.y * 0.5f);
					v2 Max = V2(ModeClassic->PlayArea.x, 
								ModeClassic->PlayArea.y * 2);
					
					if(OutOfBound(Entity, Min, Max))
					{
						KillEntity(Pool, i);
					}
				} break;
				default:
				{
					IntegrateLinearMotion(&Entity->Body, t);
					v2 Min = V2(-ModeClassic->PlayArea.x, 
								-ModeClassic->PlayArea.y * 0.5f);
					v2 Max = V2(ModeClassic->PlayArea.x, 
								ModeClassic->PlayArea.y * 2);
					
					if(OutOfBound(Entity, Min, Max))
					{
						KillEntity(Pool, i);
					}
				} break;
			}
		}
	}
}

void UpdateModeClassic(game_mode_classic* ModeClassic, 
					   player_profile* Profile,
					   game_renderer* Renderer,
					   texture_data* Textures,
					   player_data* PlayerData,
					   entity_pool* Pool, 
					   game_input* Input, 
					   game_input_map* InputMap, 
					   game_sound* Audio,
					   real32 t)
{
	SDL_ShowCursor(SDL_DISABLE);
	mc_block_properties* BlockSettings = &ModeClassic->BlockSettings;
	mc_player_info* PlayerInfo = &ModeClassic->PlayerInfo;
	entity* Player = &Pool->Entities[PlayerInfo->PlayerIndex];
	
	ProcessTimers(ModeClassic->Timers, t, MC_MAX_TIMER);
	ProcessTimers(ModeClassic->StageEffectTimers, t, MC_STAGE_EFFECT_MAX);
	ProcessTimers(ModeClassic->ParticleSpawnTimers, t, MC_PARTICLE_MAX);
	ProcessTimers(PlayerInfo->PowerupTimers, t, MC_ITEM_MAX);
	ProcessTimer(&PlayerInfo->ShootTimer, t);
	
	SetMeteorTargetModeClassic(Player, ModeClassic);
	SpawnMeteorModeClassic(Pool, ModeClassic, Renderer, &Textures[SKIN_BLOCK]);
	
	SpawnBlocksModeClassic(Pool, ModeClassic, Renderer, &Textures[SKIN_BLOCK]);
	SpawnItemModeClassic(ModeClassic, Renderer, Textures, Pool,
						(mode_classic_item)Rand(0, MC_ITEM_MAX-1));
	SpawnEnemyModeClassic(ModeClassic, Renderer, &ModeClassic->Enemy,
						  Pool,
						  (entity_type)Rand(ENTITY_ENEMY_1, ENTITY_ENEMY_3));
						  
	for(int Effect = MC_STAGE_REVERSE; 
		Effect < MC_STAGE_EFFECT_MAX; 
		++Effect)
	{
		ProcessStageEffect((mc_stage_effects)Effect, ModeClassic, Audio);
	}
	
	ScoreTickModeClassic(ModeClassic);

	UpdateEntitiesModeClassic(Pool, 
							  ModeClassic, 
							  Renderer,
							  Audio,
							  Textures, 
							  InputMap, t);
							  
	UpdateParticles(&ModeClassic->ParticleGenerator, t);
	
	CheckCollisionModeClassic(Pool, Player, ModeClassic, Audio->SFX);
	
	PowerupActive(ModeClassic, Pool, MC_ITEM_TIMEWARP);
	
	Player->RenderColor = 
	PlayerInfo->PowerupTimers[MC_ITEM_INVINCIBLE].Active ?
	Color(255, 255, 255, 0.4f) : Color();
	
	if(Input->KeyState[SDL_SCANCODE_LCTRL] && 
	   Input->Keyboard.Keys[KEY_R].EndedDown)
	{
		InitModeClassic(ModeClassic, 
						Profile,
						Renderer,
						Textures,
						ModeClassic->PlayArea, 
						Pool, 
						ModeClassic->Difficulty);
	}
}

void RenderEntitiesModeClassic(game_mode_classic* ModeClassic, 
							   entity_pool* Pool, game_renderer* Renderer)
{
	for(uint32 i = 0; i < Pool->UsedEntities; ++i)
	{
		entity* Entity = &Pool->Entities[i];
		if(Entity->Alive)
		{
			if(Entity->Texture->Success)
			{
				switch(Entity->Type)
				{
					case ENTITY_PLAYER:
					{
						if(ModeClassic->StageEffects[MC_STAGE_HIDDEN])
						{
							SetInt(Renderer->Shaders[RENDERER_TEXTURED], 
								   "StageEffect", 0);
						}
						DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
								   Entity->RenderShape,
								   V3(Entity->Body.Pos), 
								   V3(Entity->Body.Shape.Dim * 1.2f),
								   Radians(Entity->Body.Shape.Angle), 
								   V3(0, 0, 1),
								   Entity->RenderColor, 
								   Entity->Texture->ID, true);
						if(ModeClassic->StageEffects[MC_STAGE_HIDDEN])
						{
							SetInt(Renderer->Shaders[RENDERER_TEXTURED], 
								   "StageEffect", 2);
						}
					} break;
					case ENTITY_ITEM:
					{
						DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
								   Entity->RenderShape,
								   V3(Entity->Body.Pos), 
								   V3(Entity->Body.Dim),
								   Radians(Entity->Body.Shape.Angle), 
								   V3(0, 0, 1),
								   Color(), Entity->Texture->ID, true);
					} break;
					case ENTITY_BLOCK:
					{
						DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
								   Entity->RenderShape,
								   V3(Entity->Body.Pos), 
								   V3(Entity->Body.Dim),
								   Radians(Entity->Body.Shape.Angle), 
								   V3(0, 0, 1),
								   Color(), 
								   Entity->Texture->ID, true);
					} break;
					case ENTITY_BLOCK_METEOR:
					{
						DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
								   Entity->RenderShape,
								   V3(Entity->Body.Pos), 
								   V3(Entity->Body.Dim),
								   Radians(Entity->Body.Shape.Angle), 
								   V3(0, 0, 1),
								   Color(255, 0, 0), Entity->Texture->ID, true);
					} break;
					default:
					{
						DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
								   Entity->RenderShape,
								   V3(Entity->Body.Pos), 
								   V3(Entity->Body.Dim),
								   Radians(Entity->Body.Shape.Angle), 
								   V3(0, 0, 1),
								   Entity->RenderColor, 
								   Entity->Texture->ID, true);
					}
				}
			}
			else
			{
				switch(Entity->Type)
				{
					case ENTITY_PLAYER:
					{
						BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
								  &Renderer->RenderObjects[R_OBJECT_TRI],
								  V3(Entity->Body.Pos), V3(Entity->Body.Dim),
								  Entity->RenderColor, 
								  Radians(Entity->Body.Shape.Angle), V3(0, 0, 1),
								  true);
					} break;
					default:
					{
						BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
								  Entity->RenderShape,
								  V3(Entity->Body.Pos), V3(Entity->Body.Dim),
								  Entity->RenderColor, 
								  Radians(Entity->Body.Shape.Angle), V3(0, 0, 1),
								  true);
					}
				}
			}
		}
	}
}

void RenderMeteorWarning(game_mode_classic* ModeClassic, 
						 game_renderer* Renderer)
{
	timer* GameTimer = &ModeClassic->Timers[MC_GAME_TIMER];
	if(ModeClassic->MeteorSettings.IsTargetLocked)
	{
		v2 Pos = ModeClassic->MeteorSettings.SpawnPos - V2(0, 25);
		v2i Dim = {};
		TTF_SizeText(Renderer->RenderFonts[FONT_04B_EXL], 
					 "!", &Dim.x, &Dim.y);
					 
		uint8 Alpha = 100 + (uint8)(abs(sinf(GameTimer->Time * 10))*150);
		uint32 Texture = LoadTextGL(Renderer->RenderFonts[FONT_04B_EXL],
									"!", {255, 255, 255, 255});

		DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
				   &Renderer->RenderObjects[R_OBJECT_RECT],
				   V3(Pos),
				   V3(Dim),
				   Radians(180), {0, 0, 1}, 
				   Color(255, 0, 0, Alpha/255.0f),
				   Texture,
				   true);
		glDeleteTextures(1, &Texture);
	}
}

void RenderModeClassic(game_mode_classic* ModeClassic, 
					   game_renderer* Renderer,
					   entity_pool* Pool, render* Graphics)
{
	v2 WinDim = V2(Graphics->Dim);
	mc_game_info* GameInfo = &ModeClassic->GameInfo;
	mc_player_info* PlayerInfo = &ModeClassic->PlayerInfo;
	entity* Player = &Pool->Entities[PlayerInfo->PlayerIndex];
	
	SetWorldSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
							ModeClassic->PlayArea);
	SetWorldSpaceProjection(Renderer->Shaders[RENDERER_PRIMITIVES],
							ModeClassic->PlayArea);

	if(ModeClassic->StageEffects[MC_STAGE_REVERSE])
	{
		SetWorldSpaceProjectionRev(Renderer->Shaders[RENDERER_TEXTURED],
								   ModeClassic->PlayArea);
		SetWorldSpaceProjectionRev(Renderer->Shaders[RENDERER_PRIMITIVES],
								   ModeClassic->PlayArea);
	}
	
	if(ModeClassic->StageEffects[MC_STAGE_FLASHLIGHT])
	{
		SetInt(Renderer->Shaders[RENDERER_TEXTURED], "StageEffect", 1);
		SetVec3(Renderer->Shaders[RENDERER_TEXTURED],
				"Light.Pos",
				V3(Player->Body.Pos));
				
		SetReal32(Renderer->Shaders[RENDERER_TEXTURED],
				  "Light.Radius", 300.0f);
	}
	
	if(ModeClassic->StageEffects[MC_STAGE_HIDDEN])
	{
		SetInt(Renderer->Shaders[RENDERER_TEXTURED], "StageEffect", 2);
	}
	
	RenderParticles(&ModeClassic->ParticleGenerator, Renderer,
					Graphics, ModeClassic->PlayArea);
	RenderEntitiesModeClassic(ModeClassic, Pool, Renderer);
	RenderMeteorWarning(ModeClassic, Renderer);
	SetInt(Renderer->Shaders[RENDERER_TEXTURED], "StageEffect", 0);
	
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
							 WinDim);
	
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_PRIMITIVES],
							 WinDim);
	
	LoadAndRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
					  ConcatStrf("Score: ", GameInfo->Score), 
					  WinDim,
					  Renderer->Shaders[RENDERER_TEXTURED],
					  Renderer->RenderObjects[R_OBJECT_RECT],
					  TEXT_POSITION_TOP_RIGHT,
					  {}, true, 255, 1, 0, {}, 
					  TEXT_RENDER_SOLID);
	
	for(int i = 0; i < PlayerInfo->Health; ++i)
	{
		if(Player->Texture->Success)
		{
			DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
					   Player->RenderShape,
					   V3(20 + i * (Player->Body.Dim.x + 5), 20), 
					   V3(Player->Body.Dim * 1.2f),
					   Radians(180), 
					   V3(0, 0, 1),
					   Color(), 
					   Player->Texture->ID, true);
		}
		else
		{
			BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
					  &Renderer->RenderObjects[R_OBJECT_TRI],
					  V3(20 + i * (Player->Body.Dim.x + 5), 20), 
					  V3(Player->Body.Dim * 1.2f),
					  Player->RenderColor, 
					  Radians(180), V3(0, 0, 1),
					  true);
		}
	}
}

void CheckCollisionModeClassic(entity_pool* Pool, entity* Player, 
							   game_mode_classic* ModeClassic,
							   Mix_Chunk** SFX)
{
	mc_player_info* PlayerInfo = &ModeClassic->PlayerInfo;
	
	for(uint32 EntityIndex = 0; 
		EntityIndex < Pool->UsedEntities;
		++EntityIndex)
	{
		entity* Entity = &Pool->Entities[EntityIndex];
		if(Entity->Alive)
		{
			for(uint32 OtherEntityIndex = 0;
				OtherEntityIndex < Pool->UsedEntities;
				++OtherEntityIndex)
			{
				entity* OtherEntity = &Pool->Entities[OtherEntityIndex];
				if(EntityIndex != OtherEntityIndex &&
				   OtherEntity->Alive &&
				   TestBoundingSphere(&Entity->Body.Shape, 
									  &OtherEntity->Body.Shape))
				{
					switch(Entity->Type)
					{
						case ENTITY_PLAYER:
						{
							if(TestGJK(&Entity->Body.Shape, 
									  &OtherEntity->Body.Shape))
							{
								switch(OtherEntity->Type)
								{
									case ENTITY_BLOCK_METEOR:
									case ENTITY_METEOR_FRAGMENTS:
									case ENTITY_BLOCK:
									{
										if(!PlayerInfo->
										PowerupTimers[MC_ITEM_INVINCIBLE].Active)
										{
											StartTimer(&PlayerInfo->PowerupTimers[MC_ITEM_INVINCIBLE]);

											PlayChannelSafe(SFX[SFX_PLAYER_HIT]);
											--PlayerInfo->Health;
											++PlayerInfo->DeathCount;
											KillEntity(Pool, OtherEntityIndex);
										}
									} break;
									case ENTITY_ITEM:
									{
										PlayChannelSafe(SFX[SFX_POWERUP]);
										++PlayerInfo->nItemPickups;
										switch(ModeClassic->CurrentItemDrop)
										{
											case MC_ITEM_HEALTH_DROP:
											{
												++PlayerInfo->Health;
											} break;
											case MC_ITEM_TIMEWARP:
											{
												ModeClassic->BlockSettings.BlockCountOnTW =
												ModeClassic->BlockSettings.BlockCount;
											} break;
										}
									
										KillEntity(Pool, OtherEntityIndex);
										ResetTimer(&PlayerInfo->PowerupTimers[ModeClassic->CurrentItemDrop]);
										StartTimer(&PlayerInfo->PowerupTimers[ModeClassic->CurrentItemDrop]);
									} break;
									case ENTITY_ENEMY_1:
									{
										PlayerHitByEnemy(ModeClassic,
														 Pool,
														 OtherEntityIndex,
														 MC_STAGE_REVERSE,
														 ModeClassic->GameInfo.
														 StageDuration[MC_STAGE_REVERSE],
														 SFX);
									} break;
									case ENTITY_ENEMY_2:
									{
										PlayerHitByEnemy(ModeClassic,
														 Pool,
														 OtherEntityIndex,
														 MC_STAGE_FLASHLIGHT,
														 ModeClassic->GameInfo.
														 StageDuration[MC_STAGE_FLASHLIGHT],
														 SFX);
									}
									case ENTITY_ENEMY_3:
									{
										PlayerHitByEnemy(ModeClassic,
														 Pool,
														 OtherEntityIndex,
														 MC_STAGE_HIDDEN,
														 ModeClassic->GameInfo.
														 StageDuration[MC_STAGE_HIDDEN],
														 SFX);
									}
								}
							}
						} break;
						case ENTITY_LASER:
						{
							if(ModeClassic->Enemy.Active)
							{
								if(TestGJK(&Entity->Body.Shape, 
									  &OtherEntity->Body.Shape))
								{
									switch(OtherEntity->Type)
									{
										case ENTITY_ENEMY_1:
										case ENTITY_ENEMY_2:
										case ENTITY_ENEMY_3:
										{
											KillEntity(Pool, EntityIndex);
											OtherEntity->RenderColor = 
											Color(255, 0, 0);
											--ModeClassic->Enemy.HP;
											PlayChannelSafe(SFX[SFX_ENEMY_HIT]);
											if(ModeClassic->Enemy.HP <= 0)
											{
												++ModeClassic->
												PlayerInfo.nEnemyKilled;
												ModeClassic->GameInfo.Score +=
												ModeClassic->GameInfo.ScorePerTick * 10;
												ModeClassic->Enemy.Active = 0;
												KillEntity(Pool, 
														   OtherEntityIndex);
												PlayChannelSafe(
												SFX[SFX_ENEMY_DEAD]);
											}
										} break;
									}
								}
							}
						} break;
					}
				}
			}	
		}
	}
}

void PowerupActive(game_mode_classic* ModeClassic, entity_pool* Pool, 
				   mode_classic_item PowerupType)
{
	mc_player_info* PlayerInfo = &ModeClassic->PlayerInfo;
	mc_block_properties* BlockSettings = &ModeClassic->BlockSettings;
	
	timer* Timer = &PlayerInfo->PowerupTimers[PowerupType];
	switch(PowerupType)
	{
		case MC_ITEM_TIMEWARP:
		{
			if(Timer->Active)
			{
				BlockSettings->MaxBlock = BlockSettings->BlockCountOnTW;
				SetEntitySpeed(Pool, 
							   ENTITY_BLOCK, 
							   BlockSettings->DefaultFallSpeed * 0.5f);
			}
			else if(Timer->Complete)
			{
				SetEntitySpeed(Pool, 
							   ENTITY_BLOCK, 
							   BlockSettings->FallSpeed);
				BlockSettings->MaxBlock = BlockSettings->DefaultMaxBlock;
			}
		} break;
	}
}

void CompleteSBAnimationState(mc_sb_animation* Animation, 
							  scoreboard_animation_state NewState)
{
    Animation->Complete[Animation->State] = true;
	Animation->State = NewState;
}


void SBAnimationProgress(scoreboard_animation_type Type, 
						 game_mode_classic* ModeClassic,
						 scoreboard_animation_state NextState,
						 real32* SourceNum,
						 real32* TargetNum,
						 real32 t)
{
	mc_sb_animation* SBAnimation = &ModeClassic->SBAnimation;
	scoreboard_animation_state State = SBAnimation->State;
	
	switch(Type)
	{
		case SB_ANI_TYPE_ALPHA:
		{
			StartTimer(&ModeClassic->Timers[MC_SB_ANIMATION_TIMER]);
			SBAnimation->TempValues.Alpha[State] = 50 +
			(int)(ModeClassic->Timers[MC_SB_ANIMATION_TIMER].Time * 200);
			
			if(SBAnimation->TempValues.Alpha[State] >= 255)
			{
				SBAnimation->TempValues.Alpha[State] = 255;
				ResetAndStopTimer(&ModeClassic->Timers[MC_SB_ANIMATION_TIMER]);
				CompleteSBAnimationState(SBAnimation, NextState);
			}
		} break;
		case SB_ANI_TYPE_INCREMENT:
		{
			*SourceNum += *TargetNum * t;
			if((int)*SourceNum >= 
			   (int)*TargetNum)
			{
				*SourceNum = *TargetNum;
				CompleteSBAnimationState(SBAnimation, NextState);
			}
		} break;
		case SB_ANI_TYPE_ENLARGE:
		{
			StartTimer(&ModeClassic->Timers[MC_SB_ANIMATION_TIMER]);
			SBAnimation->TempValues.Scale = 
			ModeClassic->Timers[MC_SB_ANIMATION_TIMER].Time;
			if(ModeClassic->Timers[MC_SB_ANIMATION_TIMER].Time >= 1.0f)
			{
				SBAnimation->TempValues.Scale = 1.0f;
				ResetAndStopTimer(&ModeClassic->Timers[MC_SB_ANIMATION_TIMER]);
				CompleteSBAnimationState(SBAnimation, NextState);
			}
		} break;
	}
}

void RenderModeClassicScoreboard(game_mode_classic* ModeClassic, 
								 game_renderer* Renderer,
								 player_data* PlayerData,
								 scroll_bar* ScrollBar,
								 real32 t)
{
	mc_game_info* GameInfo = &ModeClassic->GameInfo;
	mc_player_info* PlayerInfo = &ModeClassic->PlayerInfo;
	mc_sb_animation* SBAnimation = &ModeClassic->SBAnimation;
	
	v2i ScoreBoardViewDim = V2i(500, 400);
	
	glViewport(100, 400, ScoreBoardViewDim.x, ScoreBoardViewDim.y);

	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
							 V2(ScoreBoardViewDim), 
							 -ScrollBar->Camera);
	
	if(!SBAnimation->Initialized)
	{
		char* AnimationText[SB_ANI_STATE_COMPLETE] = 
		{
			"HIGH SCORE",
			ConcatStrf("", PlayerData->ModeClassicHighScore),
			"STATS",
			"Play time",
			ConcatStr(ConcatStrf("", ModeClassic->Timers[MC_GAME_TIMER].Time),
					  " Seconds"),
			"Score",
			ConcatStrf("", GameInfo->Score),
			"Items picked up",
			ConcatStri("", PlayerInfo->nItemPickups),
			"Blocks dodged",
			ConcatStri("", PlayerInfo->nBlocksDodged),
			"Enemy killed",
			ConcatStri("", PlayerInfo->nEnemyKilled)
		};
		
		bool FreeText[SB_ANI_STATE_COMPLETE] =
		{
			false,
			true,
			false,
			false,
			true,
			false,
			true,
			false,
			true,
			false,
			true,
			false,
			true
		};
		
		render_text_position TextPos[SB_ANI_STATE_COMPLETE] =
		{
			TEXT_POSITION_CENTERED,
			TEXT_POSITION_CENTERED,
			TEXT_POSITION_TOP_LEFT,
			TEXT_POSITION_TOP_LEFT,
			TEXT_POSITION_TOP_RIGHT,
			TEXT_POSITION_TOP_LEFT,
			TEXT_POSITION_TOP_RIGHT,
			TEXT_POSITION_TOP_LEFT,
			TEXT_POSITION_TOP_RIGHT,
			TEXT_POSITION_TOP_LEFT,
			TEXT_POSITION_TOP_RIGHT,
			TEXT_POSITION_TOP_LEFT,
			TEXT_POSITION_TOP_RIGHT
		};
		
		v2 TextOffset[SB_ANI_STATE_COMPLETE] =
		{
			V2(0, -80),
			V2(0, -50),
			V2(0, 180),
			V2(0, 230),
			V2(0, 230),
			V2(0, 280),
			V2(0, 280),
			V2(0, 330),
			V2(0, 330),
			V2(0, 380),
			V2(0, 380),
			V2(0, 430),
			V2(0, 430)
		};
		
		for(int Index = 0; 
			Index < SB_ANI_STATE_COMPLETE;
			++Index)
		{
			SBAnimation->Text[Index] = 
			LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
						   AnimationText[Index],
						   Renderer->RenderObjects[R_OBJECT_RECT],
						   FreeText[Index]);
			SetRenderTextPos(&SBAnimation->Text[Index],
							 V2(ScoreBoardViewDim), 
							 TextOffset[Index],
							 TextPos[Index]);
		}
		SBAnimation->Initialized = true;
	}
	
	scoreboard_animation_state State = SBAnimation->State;
	//TODO: Make sure we delete the textures after user press continue
	switch(State)
	{
		case SB_ANI_STATE_HIGHSCORE:
		{
			SBAnimationProgress(SB_ANI_TYPE_ENLARGE, 
								ModeClassic, 
								SB_ANI_STATE_ACTUAL_HS);
			
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State],
					   Color(), false, SBAnimation->TempValues.Scale);
		} break;
		case SB_ANI_STATE_ACTUAL_HS:
		{
			SBAnimationProgress(SB_ANI_TYPE_INCREMENT,
								ModeClassic,
								SB_ANI_STATE_STATS,
								&SBAnimation->TempValues.HighScore,
								&PlayerData->ModeClassicHighScore,
								t);
			
			LoadAndRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], 
							  ConcatStrf("", 
							  SBAnimation->TempValues.HighScore),
							  V2(ScoreBoardViewDim),
							  Renderer->Shaders[RENDERER_TEXTURED],
						      Renderer->RenderObjects[R_OBJECT_RECT],
							  TEXT_POSITION_CENTERED,
							  V2(0, -50), true, 255, 1, 0, {},
							  TEXT_RENDER_SOLID);
		} break;
		case SB_ANI_STATE_STATS:
		{
			SBAnimationProgress(SB_ANI_TYPE_ALPHA, 
								ModeClassic, 
								SB_ANI_STATE_PLAYTIME);	
			
			real32 Alpha = (real32)SBAnimation->TempValues.Alpha[State] / 255;
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State],
					   Color(255, 255, 255, Alpha), false);
		} break;
		case SB_ANI_STATE_PLAYTIME:
		{
			SBAnimationProgress(SB_ANI_TYPE_ALPHA, 
								ModeClassic, 
								SB_ANI_STATE_SCORE);		
			real32 Alpha = (real32)SBAnimation->TempValues.Alpha[State] / 255;
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State],
					   Color(255, 255, 255, Alpha), false);
			
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State+1],
					   Color(255, 255, 255, Alpha), false);
			if(SBAnimation->Complete[State]) 
				SBAnimation->Complete[State+1] = true;
		} break;
		case SB_ANI_STATE_SCORE:
		{
			SBAnimationProgress(SB_ANI_TYPE_ALPHA, 
								ModeClassic, 
								SB_ANI_STATE_ITEMS);
			real32 Alpha = (real32)SBAnimation->TempValues.Alpha[State] / 255;
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State],
					   Color(255, 255, 255, Alpha), false);
			
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State+1],
					   Color(255, 255, 255, Alpha), false);
			
			if(SBAnimation->Complete[State]) 
				SBAnimation->Complete[State+1] = true;
		} break;
		case SB_ANI_STATE_ITEMS:
		{
			SBAnimationProgress(SB_ANI_TYPE_ALPHA, 
								ModeClassic, 
								SB_ANI_STATE_BLOCKS_DODGED);
			real32 Alpha = (real32)SBAnimation->TempValues.Alpha[State] / 255;
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State],
					   Color(255, 255, 255, Alpha), false);
			
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State+1],
					   Color(255, 255, 255, Alpha), false);
			if(SBAnimation->Complete[State]) 
				SBAnimation->Complete[State+1] = true;
		} break;
		case SB_ANI_STATE_BLOCKS_DODGED:
		{
			SBAnimationProgress(SB_ANI_TYPE_ALPHA, 
								ModeClassic, 
								SB_ANI_STATE_ENEMY_KILLED);
			real32 Alpha = (real32)SBAnimation->TempValues.Alpha[State] / 255;
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State],
					   Color(255, 255, 255, Alpha), false);
			
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State+1],
					   Color(255, 255, 255, Alpha), false);
			if(SBAnimation->Complete[State]) 
				SBAnimation->Complete[State+1] = true;
		} break;
		case SB_ANI_STATE_ENEMY_KILLED:
		{
			SBAnimationProgress(SB_ANI_TYPE_ALPHA, 
								ModeClassic, 
								SB_ANI_STATE_COMPLETE);
			real32 Alpha = (real32)SBAnimation->TempValues.Alpha[State] / 255;
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State],
					   Color(255, 255, 255, Alpha), false);
			
			RenderText(Renderer->Shaders[RENDERER_TEXTURED],
					   &SBAnimation->Text[State+1],
					   Color(255, 255, 255, Alpha), false);
			if(SBAnimation->Complete[State]) 
				SBAnimation->Complete[State+1] = true;
		}
		case SB_ANI_STATE_COMPLETE:
		{
			if(GameInfo->HighScore)
			{
				RenderText(Renderer->Shaders[RENDERER_TEXTURED],
						   &SBAnimation->Text[SB_ANI_STATE_HIGHSCORE],
						   Color(), false);
			}
			
			if(GameInfo->HighScore)
			{
				RenderText(Renderer->Shaders[RENDERER_TEXTURED],
						   &SBAnimation->Text[SB_ANI_STATE_ACTUAL_HS],
						   Color(), false);
			}
			
			for(int i = SB_ANI_STATE_STATS; i < SB_ANI_STATE_COMPLETE; ++i)
			{
				RenderText(Renderer->Shaders[RENDERER_TEXTURED],
						   &SBAnimation->Text[i],
						   Color(), false);
			}
		} break;
	}
	
	if(State != SB_ANI_STATE_COMPLETE)
	{
		if(SBAnimation->Complete[SB_ANI_STATE_HIGHSCORE])
		{
			if(GameInfo->HighScore)
			{
				RenderText(Renderer->Shaders[RENDERER_TEXTURED],
						   &SBAnimation->Text[SB_ANI_STATE_HIGHSCORE],
						   Color(), false);
			}
		}
		
		if(SBAnimation->Complete[SB_ANI_STATE_ACTUAL_HS])
		{
			if(GameInfo->HighScore)
			{
				RenderText(Renderer->Shaders[RENDERER_TEXTURED],
						   &SBAnimation->Text[SB_ANI_STATE_ACTUAL_HS],
						   Color(), false);
			}
		}
		
		for(int i = SB_ANI_STATE_STATS; i < SB_ANI_STATE_COMPLETE; ++i)
		{
			if(SBAnimation->Complete[i])
			{
				RenderText(Renderer->Shaders[RENDERER_TEXTURED],
						   &SBAnimation->Text[i],
						   Color(), false);
			}
		}
	}
}
