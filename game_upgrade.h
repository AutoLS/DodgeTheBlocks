#ifndef GAME_UPGRADE_H
#define GAME_UPGRADE_H

enum upgrade_type
{
	UPGRADE_TYPE_ATK_SPD,
	UPGRADE_TYPE_HP,
	UPGRADE_TYPE_METEOR_SPAWN,
	UPGRADE_TYPE_ITEM_SPAWN,
	UPGRADE_TYPE_ITEM_DURATION,
	UPGRADE_TYPE_STAGE_DURATION,
	UPGRADE_TYPE_MAX,
};

struct upgrade_menu
{
	button Plus[UPGRADE_TYPE_MAX];
	button Minus[UPGRADE_TYPE_MAX];
	
	render_text UpgradeText[UPGRADE_TYPE_MAX];
	render_text LevelText[UPGRADE_TYPE_MAX];
	
	text_hover_block InfoBlock[UPGRADE_TYPE_MAX];
	
	render_text TitleText;
	render_text SkillPointText;
	
	rect32 UpgradeRect;
	scroll_bar ScrollBar;
	
	button BackButton;
};

#endif