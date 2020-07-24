#ifndef GAME_MODE_CLASSIC_H

enum mc_stage_effects
{
	MC_STAGE_REVERSE,
	MC_STAGE_HIDDEN,
	MC_STAGE_FLASHLIGHT,
	MC_STAGE_EFFECT_MAX,
};

enum mode_classic_item
{
	MC_ITEM_TIMEWARP,
	MC_ITEM_INVINCIBLE,
	MC_ITEM_HEALTH_DROP,
	MC_ITEM_MAX,
};

enum mode_classic_timer
{
	MC_BLOCK_SPAWN_TIMER,
	MC_METEOR_SPAWN_TIMER,
	MC_ENEMY_SPAWN_TIMER,
	MC_SCORE_TICK_TIMER,
	MC_ITEM_DROP_TIMER,
	MC_GAME_TIMER,
	MC_SB_ANIMATION_TIMER,
	MC_MAX_TIMER,
};

enum mode_classic_particle
{
	MC_PARTICLE_SMOKE,
	MC_PARTICLE_AFTERIMAGE,
	MC_PARTICLE_MAX,
};

enum scoreboard_animation_type
{
	SB_ANI_TYPE_ALPHA,
	SB_ANI_TYPE_INCREMENT,
	SB_ANI_TYPE_ENLARGE,
};

enum scoreboard_animation_state
{
	SB_ANI_STATE_HIGHSCORE,
	SB_ANI_STATE_ACTUAL_HS,
	SB_ANI_STATE_STATS,
	SB_ANI_STATE_PLAYTIME,
	SB_ANI_STATE_PLAYTIME_N,
	SB_ANI_STATE_SCORE,
	SB_ANI_STATE_SCORE_N,
	SB_ANI_STATE_ITEMS,
	SB_ANI_STATE_ITEMS_N,
	SB_ANI_STATE_BLOCKS_DODGED,
	SB_ANI_STATE_BLOCKS_DODGED_N,
	SB_ANI_STATE_ENEMY_KILLED,
	SB_ANI_STATE_ENEMY_KILLED_N,
	SB_ANI_STATE_COMPLETE,
};

struct mc_diff_select
{
	render_text SelectDiffText;
	button Buttons[MC_DIFF_BUTTON_MAX];
};

struct mc_block_properties
{
	real32 SpawnTime;
	real32 FallSpeed;
	real32 MaxSpeed;
	real32 DefaultFallSpeed;
	uint32 nPerSpawn;
	
	uint32 BlockCount;
	uint32 BlockCountOnTW;
	uint32 MaxBlock;
	uint32 DefaultMaxBlock;
};

struct mc_meteor_properties
{
	real32 SpawnTime;
	v2 SpawnPos;
	v2 Dir;
	bool IsTargetLocked;
	real32 Speed;
	int nFragments;
};

struct mc_game_info
{
	real32 Score;
	bool HighScore;
	
	real32 ScoreMultiplier;
	real32 ScorePerTick;
	uint32 ScoreTicks;
	
	real32 SpawnMultiplier;
	
	real32 ItemSpawnTime;
	real32 StageDuration[MC_STAGE_EFFECT_MAX];
};

struct mc_player_info
{
	real32 Speed;
	int Health;
	timer ShootTimer;
	timer PowerupTimers[MC_ITEM_MAX];
	int nItemPickups;
	int nBlocksDodged;
	int nEnemyKilled;
	int DeathCount;
	int PlayerIndex;
};

struct mc_enemy_info
{
	int HP;
	bool Active;
	entity_type Type;
	int EntityIndex;
};

struct sb_animation_temp_values
{
	real32 HighScore;
	real32 Score;
	real32 PlayTime;
	real32 Scale;
	int Alpha[SB_ANI_STATE_COMPLETE];
};

struct mc_sb_animation
{
	scoreboard_animation_state State;
	bool Complete[SB_ANI_STATE_COMPLETE];
	sb_animation_temp_values TempValues;
	
	render_text Text[SB_ANI_STATE_COMPLETE];
	bool Initialized;
};

struct game_mode_classic
{
	v2 PlayArea;
	
	game_difficulty Difficulty;
	mc_diff_select DiffSelect;
	
	mc_block_properties BlockSettings;
	mc_meteor_properties MeteorSettings;
	mc_enemy_info Enemy;
	mc_player_info PlayerInfo;
	mc_game_info GameInfo;
	
	bool StageEffects[MC_STAGE_EFFECT_MAX];
	timer StageEffectTimers[MC_STAGE_EFFECT_MAX];
	
	particle_generator ParticleGenerator;
	timer ParticleSpawnTimers[MC_PARTICLE_MAX];
	
	timer Timers[MC_MAX_TIMER];
	
	int* SpawnedBlockIndices;
	mode_classic_item CurrentItemDrop;
	
	scroll_bar ScoreBoardScrollBar;
	
	mc_sb_animation SBAnimation;
};


mc_block_properties SetBlockProperties(game_difficulty Difficulty);
mc_meteor_properties SetMeteorProperties(game_difficulty Difficulty, player_profile* Profile);
mc_game_info SetGameInfo(game_difficulty Difficulty, player_profile* Profile);
mc_player_info SetPlayerInfo(real32 Speed, int HP, player_profile* Profile);

bool PlayerOutOfHealth(mc_player_info* PlayerInfo);

void InitModeClassic(game_mode_classic* ModeClassic,
					 player_profile* Profile,
					 game_renderer* Renderer, 
					 texture_data* Textures,
					 v2 PlayArea,
					 entity_pool* Pool, 
					 game_difficulty Difficulty);

void SpawnBlockCustom(entity_pool* Pool, game_renderer* Renderer, 
					   v2 Pos, v2 Vel, v2 Dim, texture_data* Texture);

void SpawnBlocksModeClassic(entity_pool* Pool, 
							game_mode_classic* ModeClassic,
							game_renderer* Renderer,
							texture_data* Texture);
							
void SpawnItemModeClassic(game_mode_classic* ModeClassic,
						  game_renderer* Renderer,
						  texture_data* Textures,
						  entity_pool* Pool, 
						  mode_classic_item ItemType);
						  
void SetMeteorTargetModeClassic(entity* Player, game_mode_classic* ModeClassic);
void SpawnMeteorModeClassic(entity_pool* Pool, game_mode_classic* ModeClassic,
							game_renderer* Renderer, texture_data* Texture);

void ScoreTickModeClassic(game_mode_classic* ModeClassic);

void UpdatePlayerModeClassic(entity_pool* Pool, texture_data* Textures,
							 game_renderer* Renderer,
							 game_sound* Audio,
							 game_mode_classic* ModeClassic, 
							 game_input_map* Input, real32 t);

void UpdateEntitiesModeClassic(entity_pool* Pool, 
							   game_mode_classic* ModeClassic, 
							   game_renderer* Renderer,
							   game_sound* Audio,
							   texture_data* Texture,
							   game_input_map* Input, 
							   real32 t);
							   
void CheckCollisionModeClassic(entity_pool* Pool, entity* Player, 
							   game_mode_classic* ModeClassic,
							   Mix_Chunk** SFX);

void PowerupActive(game_mode_classic* ModeClassic, 
				   entity_pool* Pool, 
				   mode_classic_item PowerupType);
				   
void UpdateModeClassic(game_mode_classic* ModeClassic, 
					   player_profile* Profile,
					   game_renderer* Renderer,
					   texture_data* Textures,
					   player_data* PlayerData,
					   entity_pool* Pool, 
					   game_input* Input, 
					   game_input* InputMap, 
					   game_sound* Audio,
					   real32 t);
					   
void RenderModeClassic(game_mode_classic* ModeClassic, 
					   game_renderer* Renderer,
					   entity_pool* Pool, render* Graphics);
					   
void CompleteSBAnimationState(mc_sb_animation* Animation, 
							  scoreboard_animation_state NewState);
							  
void SBAnimationProgress(scoreboard_animation_type Type, 
						 game_mode_classic* ModeClassic,
						 scoreboard_animation_state NextState,
						 real32* SourceNum = 0,
						 real32* TargetNum = 0,
						 real32 t = 0);
						 
void RenderModeClassicScoreboard(game_mode_classic* ModeClassic, 
								 game_renderer* Renderer,
								 player_data* PlayerData,
								 scroll_bar* ScrollBar,
								 real32 t);
#define GAME_MODE_CLASSIC_H
#endif