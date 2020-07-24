#ifndef GAME_PROFILE_H
#define GAME_PROFILE_H

enum profile_selection_text
{
	PROFILE_SELECTION_TITLE,
	PROFILE_SELECTION_PROFILE,
	PROFILE_SELECTION_LEVEL,
	PROFILE_SELECTION_PLAYTIME,
	PROFILE_SELECTION_HIGHSCORE,
	PROFILE_SELECTION_TOTALSCORE,
	PROFILE_SELECTION_BLOCKS,
	PROFILE_SELECTION_ITEMS,
	PROFILE_SELECTION_ENEMY,
	PROFILE_SELECTION_DEATH,
	PROFILE_SELECTION_MAX,
};

struct player_profile
{
	char Name[100];
	real32 Highscore;
	real32 TotalScore;
	
	uint8 Level;
	int64 nBlocksDodged;
	int64 nItemsPickedUp;
	int64 nEnemyKilled;
	int64 DeathCount;
	real32 PlayTimeInHours;
	
	uint32 UpgradeLevels[UPGRADE_TYPE_MAX];
	
	int SkillPoints;
	bool ProfileLoaded;
};

struct player_profile_status
{
	v2 Pos;
	v2 Dim;
	
	render_text ProfileName;
	render_text Level;
	
	rect32 Rect;
	rect32 ProgressBarRect;
	rect32 ProgressRect;
};

struct profile_selection
{
	render_text Text[PROFILE_SELECTION_MAX];
	
	rect32 SelectRect;
	rect32 StatRect;
	
	button* Buttons;
	button BackButton;
	
	button DeleteButton;
	message_box DeleteConfirm;
	
	scroll_bar ScrollBar;
	
	int SelectedProfile;
	
	array_c List;
};

struct profile_creation
{
	text_editing Name;
	render_text EnterNameText;
	render_text RestrictionText;
	
	render_text NameText;
	
	button BackButton;
	button OkButton;
	
	message_box ZeroLength;
	message_box ContainsPeriod;
	
	bool Exit;
};

//NOTE: Save and load profiles
void SaveProfile(player_profile* Profile);
void LoadProfile(player_profile* Profile, char* Name);
array_c GetProfileList();				   
void LoadProfileStats(game_renderer* Renderer, render_text* Stats,
					  profile_selection* ProfileSelection,
					  player_profile* Profile);
					  
//NOTE: Profile selection menu
profile_selection InitProfileSelectionMenu(player_profile* Profile, 
										   game_renderer* Renderer, 
										   render* Graphics);
void FreeProfileSelectionMenu(profile_selection* ProfileSelection);
void RenderProfileSelectionMenu(profile_selection* ProfileSelection,
								game_renderer* Renderer, 
								render* Graphics);

//NOTE: Profile creation menu		   
profile_creation 
InitProfileCreationMenu(game_renderer* Renderer, render* Graphics);

void UpdateProfileCreationMenu(profile_creation* ProfileCreation,
							   profile_selection* ProfileSelection,
							   player_profile* Profile,
							   render* Graphics,
							   game_renderer* Renderer,
							   game_input* Input,
							   game_sound* Audio,
							   SDL_Event* Event, 
							   real32 t);
							   
void RenderProfileCreationMenu(profile_creation* ProfileCreation, 
							   game_renderer* Renderer,
							   render* Graphics);
#endif