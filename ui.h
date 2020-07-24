#ifndef UI_H
#define UI_H

enum ui_button_state
{
	UI_BUTTON_STATE_NONE,
	UI_BUTTON_STATE_CLICK_L,
	UI_BUTTON_STATE_CLICK_R,
	UI_BUTTON_STATE_CLICK_M,
	UI_BUTTON_STATE_HOVER,
};

enum ui_button_type
{
	UI_BUTTON_UNTEXTURED,
	UI_BUTTON_TEXTURED,
	UI_BUTTON_PLAIN_TEXT,
};

enum ui_msg_box_type
{
	UI_MSGBOX_YESNO,
	UI_MSGBOX_OK,
};

enum ui_msg_box_state
{
	UI_MSGBOX_STATE_NONE,
	UI_MSGBOX_STATE_YES,
	UI_MSGBOX_STATE_NO,
	UI_MSGBOX_STATE_OK,
};

struct button
{
	uint32 Texture;
	uint32 HoveredTexture;
	
	v2 DefaultPos;
	
	v2 Pos;
	v2 Dim;
	
	v2 RenderPos;
	
	render_text Text;
	ui_button_type Type;
	
	rect32 Rect;
	ui_button_state State;
	
	v4 RenderColor;
	v4 HoverColor;
	v4 DefaultColor;
};

button CreateButton(ui_button_type Type, 
					uint32 Texture, 
					uint32 HoveredTexture,
					render_text Text, 
					v4 RenderColor = Color(), 
					v4 HoverColor = Color(),
					v2 Pos = {}, v2 Dim = {},
					v2 RenderPos = {})
{
	button Button = {};
	Button.Type = Type;
	Button.RenderPos = RenderPos;
	Button.DefaultColor = RenderColor;
	if(Length(RenderPos) == 0)
	{
		Button.RenderPos = Pos;
	}

	switch(Type)
	{
		case UI_BUTTON_UNTEXTURED:
		{
			Button.Pos = Pos;
			Button.Dim = Dim;
			Button.RenderColor = RenderColor;
			Button.HoverColor = HoverColor;
			Button.Text = Text;
		} break;
		case UI_BUTTON_TEXTURED:
		{
			Button.Texture = Texture;
			Button.HoveredTexture = HoveredTexture;
			Button.Pos = Pos;
			Button.Dim = Dim;
		} break;
		case UI_BUTTON_PLAIN_TEXT:
		{
			Button.Text = Text;
			Button.Pos = Text.Pos;
			Button.Dim = V2(Text.Dim);
			Button.RenderColor = RenderColor;
		}
	}
	
	Button.DefaultPos = Button.Pos;
	Button.Rect = {Button.Pos - (Button.Dim * 0.5f), Button.Dim};
	
	return Button;
}

bool HandleButton(button* Button, game_input* Input, Mix_Chunk* SFX = 0)
{
	bool Result = false;
	
	Button->State = UI_BUTTON_STATE_NONE;
	
	switch(Button->Type)
	{
		case UI_BUTTON_UNTEXTURED:
		case UI_BUTTON_TEXTURED:
		case UI_BUTTON_PLAIN_TEXT:
		{
			if(IsPointInRect(V2(Input->MouseState), &Button->Rect))
			{
				Button->State = UI_BUTTON_STATE_HOVER;
				if(Input->Mouse.Keys[BUTTON_LEFT].EndedDown)
				{
					Button->State = UI_BUTTON_STATE_CLICK_L;
					Result = true;
					if(SFX)
					{
						Mix_PlayChannel(-1, SFX, 0);
					}
				}
				else if(Input->Mouse.Keys[BUTTON_RIGHT].EndedDown)
				{
					Button->State = UI_BUTTON_STATE_CLICK_R;
					Result = true;
					if(SFX)
					{
						Mix_PlayChannel(-1, SFX, 0);
					}
				}
			}
		} break;
	}
	
	return Result;
}

void HandleButtons(button* Button, game_input* Input, int Count)
{
	for(int i = 0; i < Count; ++i)
	{
		HandleButton(&Button[i], Input);
	}
}

bool IsButtonClicked(button* Button)
{
	return Button->State == UI_BUTTON_STATE_CLICK_L || 
		   Button->State == UI_BUTTON_STATE_CLICK_R ||
		   Button->State == UI_BUTTON_STATE_CLICK_M;
}

void RenderButton(button* Button, 
				  game_renderer* Renderer, 
				  bool DeleteTexture = false)
{
	switch(Button->Type)
	{
		case UI_BUTTON_TEXTURED:
		{
			switch(Button->State)
			{
				case UI_BUTTON_STATE_CLICK_L:
				case UI_BUTTON_STATE_CLICK_R:
				case UI_BUTTON_STATE_CLICK_M:
				case UI_BUTTON_STATE_NONE:
				{
					DrawObject(Renderer->Shaders[RENDERER_TEXTURED],
							   &Renderer->RenderObjects[R_OBJECT_RECT],
							   V3(Button->Pos), V3(Button->Dim), 
							   0, {}, Color(), Button->Texture, true);
				} break;
				case UI_BUTTON_STATE_HOVER:
				{
					DrawObject(Renderer->Shaders[RENDERER_TEXTURED],
							   &Renderer->RenderObjects[R_OBJECT_RECT],
							   V3(Button->Pos), V3(Button->Dim), 
							   0, {}, Color(), Button->HoveredTexture, true);
				} break;
			}
		} break;
		
		case UI_BUTTON_UNTEXTURED:
		{
			switch(Button->State)
			{
				case UI_BUTTON_STATE_CLICK_L:
				case UI_BUTTON_STATE_CLICK_R:
				case UI_BUTTON_STATE_CLICK_M:
				case UI_BUTTON_STATE_NONE:
				{
					BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
							  &Renderer->RenderObjects[R_OBJECT_RECT], 
							  V3(Button->RenderPos), V3(Button->Dim), 
							  Button->RenderColor, 0, {}, true);
					if(Button->Text.Text)
					{
						RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
									&Button->Text, 
									Color(), 
									DeleteTexture);
					}
				} break;
				case UI_BUTTON_STATE_HOVER:
				{
					BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
							  &Renderer->RenderObjects[R_OBJECT_RECT], 
							  V3(Button->RenderPos), V3(Button->Dim), 
							  Button->HoverColor, 0, {}, true);
					if(Button->Text.Text)
					{
						RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
									&Button->Text, 
									Color(), 
									DeleteTexture);
					}
				} break;
			}
		} break;
		
		case UI_BUTTON_PLAIN_TEXT:
		{
			switch(Button->State)
			{
				case UI_BUTTON_STATE_CLICK_L:
				case UI_BUTTON_STATE_CLICK_R:
				case UI_BUTTON_STATE_CLICK_M:
				case UI_BUTTON_STATE_NONE:
				{
					Button->RenderColor.a = 1.0f;
					RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
								&Button->Text, 
								Button->RenderColor, DeleteTexture);
				} break;
				case UI_BUTTON_STATE_HOVER:
				{
					Button->RenderColor.a = 0.5f;
					RenderText(Renderer->Shaders[RENDERER_TEXTURED], 
							   &Button->Text, 
							   Button->RenderColor,
							   DeleteTexture);
				} break;
			}
		} break;
	}
}

void RenderButtons(button* Buttons, int Count, uint32 Renderer, 
				   object_data* RenderObject = 0,
				   bool DeleteTexture = false)
{
	for(int ButtonIndex = 0; ButtonIndex < Count; ++ButtonIndex)
	{
		button* Button = &Buttons[ButtonIndex];
		switch(Button->Type)
		{
			case UI_BUTTON_PLAIN_TEXT:
			{
				switch(Button->State)
				{
					case UI_BUTTON_STATE_NONE:
					{
						Button->RenderColor.a = 1.0f;
						RenderText(Renderer, &Button->Text, 
									Button->RenderColor, DeleteTexture);
					} break;
					case UI_BUTTON_STATE_HOVER:
					{
						Button->RenderColor.a = 0.5f;
						RenderText(Renderer, &Button->Text, 
								   Button->RenderColor,
								   DeleteTexture);
					} break;
				}
			} break;
		}
	}
}

struct scroll_bar
{
	v2 Camera;
	rect32 ViewRect;
	v2 GripPos;
	v2 GripDim;
	v2 ActualGripPos;
	rect32 GripRect;
	v2 TrackDim;
	
	bool Active;
};

scroll_bar CreateScrollBar(v2 TrackDim, 
						   rect32 ViewRect, 
						   real32 ScrollBarWidth)
{
	scroll_bar Scrollbar = {};
	Scrollbar.TrackDim = TrackDim;
	Scrollbar.ViewRect = ViewRect;
	real32 GripHeight = (ViewRect.Dim.y / TrackDim.y) * ViewRect.Dim.y;
	if(TrackDim.y <= ViewRect.Dim.y)
	{
		GripHeight = ViewRect.Dim.y;
	}
	Scrollbar.GripDim = V2(ScrollBarWidth, GripHeight);
	Scrollbar.GripPos = V2(ViewRect.Pos.x + ViewRect.Dim.x - 
						  (Scrollbar.GripDim.x * 0.5f), 
						   ViewRect.Pos.y + Scrollbar.GripDim.y * 0.5f);
	Scrollbar.GripRect = {Scrollbar.GripPos - (Scrollbar.GripDim * 0.5f), Scrollbar.GripDim};
	
	return Scrollbar;
}

void HandleScrollBar(scroll_bar* ScrollBar, 
					 game_input* Input, 
					 render* Graphics)
{
	ScrollBar->Active = false;
	if(Input->Mouse.Keys[BUTTON_LEFT].IsDown)
	{
		if(IsPointInRect(V2(Input->MouseState), &ScrollBar->GripRect) &&
			ScrollBar->ViewRect.Dim.y < ScrollBar->TrackDim.y)
		{
			real32 DeltaY = Input->MouseState.y - ScrollBar->GripPos.y;
			ScrollBar->GripPos.y += DeltaY;
			ScrollBar->ActualGripPos.y += DeltaY;
			
			ScrollBar->Camera.y = ScrollBar->TrackDim.y * 
			(ScrollBar->ActualGripPos.y / ScrollBar->ViewRect.Dim.y);
			
			if(ScrollBar->GripPos.y - (ScrollBar->GripDim.y * 0.5f) <= 
			ScrollBar->ViewRect.Pos.y)
			{
				ScrollBar->GripPos.y = ScrollBar->ViewRect.Pos.y + 
									   ScrollBar->GripDim.y * 0.5f;
				DeltaY = 0;
				ScrollBar->Camera.y = 0;
				ScrollBar->ActualGripPos.y = 0;
			}
			if(ScrollBar->GripPos.y + (ScrollBar->GripDim.y * 0.5f) >= 
			   ScrollBar->ViewRect.Pos.y + ScrollBar->ViewRect.Dim.y)
			{
				ScrollBar->GripPos.y = ScrollBar->ViewRect.Pos.y + 
									   ScrollBar->ViewRect.Dim.y - 
									  (ScrollBar->GripDim.y * 0.5f);
				ScrollBar->ActualGripPos.y = ScrollBar->ViewRect.Dim.y -
											ScrollBar->GripDim.y;
				ScrollBar->Camera.y = ScrollBar->TrackDim.y * 
				(ScrollBar->ActualGripPos.y / ScrollBar->ViewRect.Dim.y);
			}
			ScrollBar->GripRect = {ScrollBar->GripPos - 
								   (ScrollBar->GripDim * 0.5f), 
								   ScrollBar->GripDim};
 			
			SDL_WarpMouseInWindow(Graphics->Window, 
								  (int)ScrollBar->GripPos.x,
								  (int)ScrollBar->GripPos.y);
			ScrollBar->Active = true;
		}
	}
	else if(Input->Scrolling)
	{
		if(IsPointInRect(V2(Input->MouseState), &ScrollBar->ViewRect))
		{
			Input->Scrolling = false;
			real32 Speed = 20.0f;
			real32 ScrollY = Input->Scroll.y * Speed;
			ScrollBar->GripPos.y += -ScrollY;
			ScrollBar->ActualGripPos.y += -ScrollY;
			
			ScrollBar->Camera.y = ScrollBar->TrackDim.y * 
			(ScrollBar->ActualGripPos.y / ScrollBar->ViewRect.Dim.y);
			
			if(ScrollBar->GripPos.y - (ScrollBar->GripDim.y * 0.5f) <= 
			ScrollBar->ViewRect.Pos.y)
			{
				ScrollBar->GripPos.y = ScrollBar->ViewRect.Pos.y + 
									   ScrollBar->GripDim.y * 0.5f;
				ScrollY = 0;
				ScrollBar->Camera.y = 0;
				ScrollBar->ActualGripPos.y = 0;
			}
			if(ScrollBar->GripPos.y + (ScrollBar->GripDim.y * 0.5f) >= 
			   ScrollBar->ViewRect.Pos.y + ScrollBar->ViewRect.Dim.y)
			{
				ScrollBar->GripPos.y = ScrollBar->ViewRect.Pos.y + 
									   ScrollBar->ViewRect.Dim.y - 
									  (ScrollBar->GripDim.y * 0.5f);
				ScrollBar->ActualGripPos.y = ScrollBar->ViewRect.Dim.y -
											 ScrollBar->GripDim.y;
				ScrollBar->Camera.y = ScrollBar->TrackDim.y * 
				(ScrollBar->ActualGripPos.y / ScrollBar->ViewRect.Dim.y);
			}
			ScrollBar->GripRect = {ScrollBar->GripPos - 
								   (ScrollBar->GripDim * 0.5f), 
								   ScrollBar->GripDim};
			ScrollBar->Active = true;
		}
	}
}

void RenderScrollBar(scroll_bar* ScrollBar, v2 WinDim, uint32 Renderer, 
					 object_data* RenderObject)
{
	SetScreenSpaceProjection(Renderer, WinDim);
	BasicDraw(Renderer, RenderObject,
			  V3(ScrollBar->GripPos), V3(ScrollBar->GripDim),
			  Color(),
			  0, {}, true);
}

struct message_box
{
	button* Buttons;
	
	render_text Title;
	render_text Body;
	
	v2 Pos;
	v2 Dim;
	rect32 Rect;
	
	ui_msg_box_type Type;
	
	bool Active;
};

message_box CreateMessageBox(ui_msg_box_type Type, game_renderer* Renderer,
							 char* Title, char* Body, 
							 v2 Pos, v2 Dim)
{
	message_box MsgBox = {};
	MsgBox.Type = Type;
	MsgBox.Pos = Pos;
	MsgBox.Dim = Dim;
	MsgBox.Rect = {Pos - (Dim * 0.5f), Dim};
	MsgBox.Title = LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
								  Title, Renderer->RenderObjects[R_OBJECT_RECT]);
	MsgBox.Body = LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
								 Body, Renderer->RenderObjects[R_OBJECT_RECT]);
	
	SetRenderTextPos(&MsgBox.Title, &MsgBox.Rect, V2(15, 15),
					 TEXT_POSITION_TOP_LEFT);
	SetRenderTextPos(&MsgBox.Body, &MsgBox.Rect, 
					 V2(), 
					 TEXT_POSITION_CENTERED);
	
	switch(Type)
	{
		case UI_MSGBOX_YESNO:
		{
			render_text YesText =
			LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
							"YES", Renderer->RenderObjects[R_OBJECT_RECT]);
			
			render_text NoText =
			LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
							"NO", Renderer->RenderObjects[R_OBJECT_RECT]);
			
			SetRenderTextPos(&YesText, &MsgBox.Rect, 
							 V2(15, -15), TEXT_POSITION_BOTTOM_LEFT);
			SetRenderTextPos(&NoText, &MsgBox.Rect, 
							 V2(-15, -15), TEXT_POSITION_BOTTOM_RIGHT);
			
			MsgBox.Buttons = (button*)malloc(sizeof(button) * 2);
			MsgBox.Buttons[0] = 
			CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, YesText, Color(0, 0, 0));
			MsgBox.Buttons[1] = 
			CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, NoText, Color(0, 0, 0));
			
		} break;
		
		case UI_MSGBOX_OK:
		{
			render_text OKText =
			LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_LARGE],
							"OK", Renderer->RenderObjects[R_OBJECT_RECT]);
			SetRenderTextPos(&OKText, &MsgBox.Rect, 
							 V2(Dim.x * 0.5f - OKText.Dim.x * 0.5f, 
								-Dim.y * 0.1f), 
							 TEXT_POSITION_BOTTOM_LEFT);
			MsgBox.Buttons = (button*)malloc(sizeof(button));				 
			MsgBox.Buttons[0] = 
			CreateButton(UI_BUTTON_PLAIN_TEXT, 0, 0, OKText, Color(0, 0, 0));
		} break;
	}
	
	return MsgBox;
}

void PromptMessageBox(message_box* MsgBox)
{
	MsgBox->Active = true;
}

ui_msg_box_state ProcessMessageBox(message_box* MsgBox, game_input* Input)
{
	ui_msg_box_state Result = UI_MSGBOX_STATE_NONE;
	if(MsgBox->Active)
	{
		switch(MsgBox->Type)
		{
			case UI_MSGBOX_YESNO:
			{
				HandleButton(&MsgBox->Buttons[0], Input);
				HandleButton(&MsgBox->Buttons[1], Input);
				
				if(MsgBox->Buttons[0].State == UI_BUTTON_STATE_CLICK_L)
				{
					Result = UI_MSGBOX_STATE_YES;
					MsgBox->Active = false;
				}
				if(MsgBox->Buttons[1].State == UI_BUTTON_STATE_CLICK_L)
				{
					Result = UI_MSGBOX_STATE_NO;
					MsgBox->Active = false;
				}
				
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown)
				{
					Result = UI_MSGBOX_STATE_NO;
					MsgBox->Active = false;
				}
			} break;
			case UI_MSGBOX_OK:
			{
				HandleButton(&MsgBox->Buttons[0], Input);
				if(MsgBox->Buttons[0].State == UI_BUTTON_STATE_CLICK_L)
				{
					Result = UI_MSGBOX_STATE_OK;
					MsgBox->Active = false;
				}
				if(Input->Keyboard.Keys[KEY_ESC].EndedDown)
				{
					Result = UI_MSGBOX_STATE_OK;
					MsgBox->Active = false;
				}
			}
		}
	}
	
	return Result;
}

void RenderMessageBox(message_box* MsgBox, 
					  game_renderer* Renderer)
{
	if(MsgBox->Active)
	{
		BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
				  &Renderer->RenderObjects[R_OBJECT_RECT], 
				  V3(MsgBox->Pos), V3(MsgBox->Dim), 
				  Color(), 0, {}, true);
		
		glLineWidth(3);
		BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
				  &Renderer->RenderObjects[R_OBJECT_RECT], 
				  V3(MsgBox->Pos), V3(MsgBox->Dim), 
				  Color(0, 0, 255));
		glLineWidth(1);
		RenderText(Renderer->Shaders[RENDERER_TEXTURED], &MsgBox->Title, 
					Color(0, 0, 0), false);
		
		RenderText(Renderer->Shaders[RENDERER_TEXTURED], &MsgBox->Body, 
					Color(0, 0, 0), false);
					
		RenderButtons(MsgBox->Buttons, 2, Renderer->Shaders[RENDERER_TEXTURED]);
	}
}

struct check_box
{	
	v2 Pos;
	v2 DefaultPos;
	v2 Dim;
	
	rect32 Rect;
	
	bool Active;
};

check_box CreateCheckBox(v2 Pos, v2 Dim, bool Active = false)
{
	check_box CheckBox = {};
	CheckBox.Pos = Pos;
	CheckBox.DefaultPos = Pos;
	CheckBox.Dim = Dim;
	CheckBox.Rect = WinRect32(Pos, Dim);
	CheckBox.Active = Active;
	
	return CheckBox;
}

void HandleCheckBox(check_box* CheckBox, game_input* Input, Mix_Chunk* SFX)
{
	if(IsPointInRect(V2(Input->MouseState), &CheckBox->Rect))
	{
		if(Input->Mouse.Keys[BUTTON_LEFT].EndedDown)
		{
			CheckBox->Active = !CheckBox->Active;
			if(SFX)
			{
				Mix_PlayChannel(-1, SFX, 0);
			}
		}
	}
}

void RenderCheckBox(check_box* CheckBox, game_renderer* Renderer)
{
	BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
			  &Renderer->RenderObjects[R_OBJECT_RECT],
			  V3(CheckBox->Pos), V3(CheckBox->Dim),
			  Color(),
			  0, {},
			  false, true);
			  
	if(CheckBox->Active)
	{
		BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
				  &Renderer->RenderObjects[R_OBJECT_RECT],
				  V3(CheckBox->Pos), V3(CheckBox->Dim * 0.8f),
				  Color(),
				  0, {},
				  true, true);
	}
}

void RenderCheckBoxes(check_box* CheckBox, game_renderer* Renderer, int Count)
{
	for(int i = 0; i < Count; ++i)
	{
		RenderCheckBox(&CheckBox[i], Renderer);
	}
}

struct drop_menu
{
	button Expand;
	render_text ExpandText;
	
	button* Items;
	int ItemCount;
	v2 ItemDim;
	
	rect32 Rect;
	
	scroll_bar ScrollBar;
	
	bool Active;
};

#if 1
drop_menu CreateDropMenu(v2 ItemDim, int ItemCount,
						 v2 ExpandPos, char* ExpandText, 
						 v4 DefColor, v4 HovColor, 
						 game_renderer* Renderer)
{
	drop_menu Menu = {};
	Menu.ItemDim = ItemDim;
	
	Menu.Expand = CreateButton(UI_BUTTON_UNTEXTURED, 0, 0, {},
							   DefColor, HovColor, 
							   ExpandPos, Menu.ItemDim);
	Menu.ExpandText = 
	LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM],
				   ExpandText,
				   Renderer->RenderObjects[R_OBJECT_RECT]);
	SetRenderTextPos(&Menu.ExpandText, 
					 &Menu.Expand.Rect,
					 V2(10), TEXT_POSITION_TOP_LEFT);
	
	Menu.ItemCount = ItemCount;
	Menu.Items = (button*)malloc(sizeof(button) * ItemCount);
	
	return Menu;
}

void InitDropMenuButtonsAndScrollbar(drop_menu* Menu, game_renderer* Renderer, 
									 v4 DefColor, v4 HovColor,
									 real32 ViewHeight,
									 string* ButtonTextData = 0)
{
	rect32 ExpandRect = Menu->Expand.Rect;
	v2 ItemDim = Menu->ItemDim;
	
	v2 ViewDim = V2(ExpandRect.Dim.x, ViewHeight);
	rect32 Rect = {ExpandRect.Pos + V2(0, ItemDim.y), ViewDim};
	Menu->Rect = Rect;
	
	for(int i = 0; i < Menu->ItemCount; ++i)
	{
		render_text ButtonText = 
		LoadRenderText(Renderer->RenderFonts[DEFAULT_FONT_MEDIUM], 
					   (char*)ButtonTextData[i].c_str(), 
					   Renderer->RenderObjects[R_OBJECT_RECT]);
		SetRenderTextPos(&ButtonText, ViewDim, 
						 V2(10, i * ItemDim.y), TEXT_POSITION_TOP_LEFT);
		
		v2 Pos = V2(Menu->Expand.Pos.x, 
					Menu->Expand.Pos.y + ItemDim.y + (i * ItemDim.y));
		v2 RenderPos = V2(ItemDim.x * 0.5f, i * ItemDim.y + (ItemDim.y*0.5f));
		
		Menu->Items[i] = CreateButton(UI_BUTTON_UNTEXTURED, 0, 0, 
									   ButtonText, 
									   DefColor, 
									   HovColor,
									   Pos,
									   ItemDim,
									   RenderPos);
	}
	
	Menu->ScrollBar = 
	CreateScrollBar(V2(ViewDim.x, ItemDim.y * (Menu->ItemCount)), Rect, 20);
}
#endif

bool IsMenuActive(drop_menu* Menu)
{
	return Menu->Active;
}

void HandleDropMenu(drop_menu* Menu, game_input* Input, render* Graphics, 
					Mix_Chunk* SFX = 0)
{
	HandleButton(&Menu->Expand, Input, SFX);
	
	if(Menu->Expand.State == UI_BUTTON_STATE_CLICK_L)
	{
		Input->Mouse.Keys[BUTTON_LEFT].EndedDown = false;
		Menu->Active = !Menu->Active;
	}
	
	if(Menu->Active)
	{
		if(IsPointInRect(V2(Input->MouseState), 
						&Menu->Rect))
		{
			HandleScrollBar(&Menu->ScrollBar, 
							Input,
							Graphics);
			
			for(int i = 0; i < Menu->ItemCount; ++i)
			{
				Menu->Items[i].Pos.y = 
				Menu->Items[i].DefaultPos.y - 
				Menu->ScrollBar.Camera.y;
				Menu->Items[i].Rect = 
				WinRect32(Menu->Items[i].Pos, Menu->Items[i].Dim);

				if(!Menu->ScrollBar.Active)
				HandleButton(&Menu->Items[i], Input);
			}
		}
		else
		{
			if(Input->Mouse.Keys[BUTTON_LEFT].EndedDown)
			{
				Menu->Active = false;
			}
		}
	}
}

void RenderDropMenu(drop_menu* Menu, game_renderer* Renderer, 
					render* Graphics)
{
	v2 WinDim = V2(Graphics->Dim);
	
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
							 WinDim);
	SetScreenSpaceProjection(Renderer->Shaders[RENDERER_PRIMITIVES],
							 WinDim);
	
	RenderButton(&Menu->Expand, Renderer);
	RenderText(Renderer->Shaders[RENDERER_TEXTURED],
			   &Menu->ExpandText,
			   Color(), 
			   false);
	
	if(Menu->Active)
	{
		v2 ViewPos = Menu->Expand.Rect.Pos;
		v2 ViewDim = Menu->Rect.Dim;
		real32 PosY = WinDim.y - ViewPos.y - 
					  Menu->Expand.Dim.y - ViewDim.y;
		glViewport((int)ViewPos.x, 
				   (int)PosY,
				   (int)ViewDim.x,
				   (int)ViewDim.y);
		SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
								 ViewDim);
		SetScreenSpaceProjection(Renderer->Shaders[RENDERER_PRIMITIVES],
								 ViewDim);
								 
		//NOTE: Draw menu background
		//TODO: Add background color in drop_menu struct
		BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES],
					&Renderer->RenderObjects[R_OBJECT_RECT],
					V3(ViewDim*0.5f), V3(ViewDim),
					Color(150, 150, 150, 0.5f),
					0, {}, true, true);
		
		SetScreenSpaceProjection(Renderer->Shaders[RENDERER_TEXTURED],
								 ViewDim,
								 -Menu->ScrollBar.Camera);
		SetScreenSpaceProjection(Renderer->Shaders[RENDERER_PRIMITIVES],
								 ViewDim,
								 -Menu->ScrollBar.Camera);					
		for(int i = 0; i < Menu->ItemCount; ++i)
		{
			RenderButton(&Menu->Items[i], Renderer, false);
		}
		
		glViewport(0, 0, Graphics->Dim.x, Graphics->Dim.y);
	
		SetScreenSpaceProjection(Renderer->Shaders[RENDERER_PRIMITIVES],
								 WinDim);
		RenderScrollBar(&Menu->ScrollBar,
						WinDim, 
						Renderer->Shaders[RENDERER_PRIMITIVES],
						&Renderer->RenderObjects[R_OBJECT_RECT]);
	}
}

struct slide_bar
{
	uint32 Texture;
	uint32 SliderTexture;
	
	rect32 Rect;
	v2 Pos;
	v2 Dim;
	
	v2 RenderDim;
	
	rect32 SliderRect;
	v2 SliderPos;
	v2 SliderDim;
	
	real32 Value;
	real32 MaxValue;
};

slide_bar CreateSlidebar(uint32 Texture, uint32 SliderTexture, 
						 v2 Pos, v2 Dim, real32 Value, real32 MaxValue)
{
	slide_bar SlideBar = {};
	SlideBar.Texture = Texture;
	SlideBar.SliderTexture = SliderTexture;
	SlideBar.Pos = Pos;
	SlideBar.Dim = Dim;
	SlideBar.RenderDim = V2(Dim.x, 4);
	SlideBar.Rect = WinRect32(Pos, Dim);
	SlideBar.Value = Value;
	SlideBar.MaxValue = MaxValue;
	SlideBar.SliderPos = V2(SlideBar.Rect.Pos.x + (Dim.x * (Value/MaxValue)), 
						    Pos.y);
	SlideBar.SliderDim = V2(30, 30);
	SlideBar.SliderRect = WinRect32(SlideBar.SliderPos, SlideBar.SliderDim);
	
	return SlideBar;
};

real32 HandleSlidebar(slide_bar* SlideBar, game_input* Input)
{
	v2 MouseState = V2(Input->MouseState);
	real32 Result = SlideBar->Value;
	if(IsPointInRect(MouseState, &SlideBar->Rect))
	{
		if(Input->Mouse.Keys[BUTTON_LEFT].IsDown)
		{
			if(IsPointInRect(MouseState, &SlideBar->SliderRect))
			{
				SlideBar->SliderPos.x = MouseState.x;
				SlideBar->SliderRect = WinRect32(SlideBar->SliderPos, 
												 SlideBar->SliderDim);
				Result = ((SlideBar->SliderPos.x - SlideBar->Rect.Pos.x) / 
						  SlideBar->Dim.x) * SlideBar->MaxValue;
				SlideBar->Value = Result;
			}
		}
	}
	
	return Result;
}

void RenderSlidebar(slide_bar* SlideBar, game_renderer* Renderer)
{
	DrawObject(Renderer->Shaders[RENDERER_TEXTURED],
			   &Renderer->RenderObjects[R_OBJECT_RECT],
			   V3(SlideBar->Pos), V3(SlideBar->RenderDim),
			   0, {}, Color(), SlideBar->Texture, true);
	
	DrawObject(Renderer->Shaders[RENDERER_TEXTURED],
			   &Renderer->RenderObjects[R_OBJECT_RECT],
			   V3(SlideBar->SliderPos), V3(SlideBar->SliderDim),
			   0, {}, Color(), SlideBar->SliderTexture, true);
}

void RenderSlidebars(slide_bar* SlideBar, game_renderer* Renderer, int Count)
{
	for(int Index = 0; Index < Count; ++Index)
	{
		RenderSlidebar(&SlideBar[Index], Renderer);
	}
}

struct panel
{
	rect32 Rect;
	button* Buttons;
	int ButtonCount;
	
	v4 BGColor;
	v4 ButtonDefColor;
	v4 ButtonHovColor;
	
	scroll_bar ScrollBar;
};

void CalculatePositionOnScroll(button* Button, scroll_bar* ScrollBar)
{
	Button->Pos.y = Button->DefaultPos.y - ScrollBar->Camera.y;
	Button->Rect = WinRect32(Button->Pos, Button->Dim);
}

void CalculatePositionOnScroll(check_box* CheckBox, scroll_bar* ScrollBar)
{
	CheckBox->Pos.y = CheckBox->DefaultPos.y - ScrollBar->Camera.y;
	CheckBox->Rect = WinRect32(CheckBox->Pos, CheckBox->Dim);
}

enum ui_hover_block_state
{
	UI_HOVER_BLOCK_STATE_NONE,
	UI_HOVER_BLOCK_STATE_HOVER,
};

struct text_hover_block
{
	uint32 Texture;
	v2 Pos;
	v2 Dim;
	rect32 Rect;
	
	render_text Text;
	v4 BackgroundColor;
	
	bool Border;
	ui_hover_block_state State;
};

text_hover_block CreateHoverBlock(uint32 Texture, char* Text, 
								  game_renderer* Renderer, 
								  v2 Pos, v2 Dim, 
								  v4 BGColor, bool Border)
{
	TTF_Font* Font = Renderer->RenderFonts[DEFAULT_FONT_SMALL];
	object_data Object = Renderer->RenderObjects[R_OBJECT_RECT];
	
	text_hover_block Result = {};
	Result.Texture = Texture;
	Result.Pos = Pos;
	Result.Dim = Dim;
	Result.Rect = {Result.Pos - (Result.Dim * 0.5f), Result.Dim};
	Result.Text = LoadRenderText(Font, Text, Object);
	Result.BackgroundColor = BGColor;
	Result.Border = Border;
	
	return Result;
}

void ProcessHoverBlock(text_hover_block* HoverBlock, game_input* Input)
{
	v2 MousePos = V2(Input->MouseState);
	
	HoverBlock->State = UI_HOVER_BLOCK_STATE_NONE;
	if(IsPointInRect(MousePos, &HoverBlock->Rect))
	{
		HoverBlock->State = UI_HOVER_BLOCK_STATE_HOVER;
		HoverBlock->Text.Pos = MousePos + V2(HoverBlock->Text.Dim.x * 0.5f, 
											 HoverBlock->Text.Dim.y * -0.5f);
	}
}

void RenderHoverBlock(text_hover_block* HoverBlock, game_renderer* Renderer)
{
	if(HoverBlock->Border)
	{
		glLineWidth(3);
		BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
				  &Renderer->RenderObjects[R_OBJECT_RECT], 
				  V3(HoverBlock->Pos), V3(HoverBlock->Dim), 
				  Color());
		glLineWidth(1);
	}
	switch(HoverBlock->State)
	{
		case UI_HOVER_BLOCK_STATE_NONE:
		{
			DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
					   &Renderer->RenderObjects[R_OBJECT_RECT],
					   V3(HoverBlock->Pos), V3(HoverBlock->Dim),
					   0, {}, Color(), HoverBlock->Texture, true);
		} break;
		case UI_HOVER_BLOCK_STATE_HOVER:
		{
			DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
					   &Renderer->RenderObjects[R_OBJECT_RECT],
					   V3(HoverBlock->Pos), V3(HoverBlock->Dim),
					   0, {}, Color(), HoverBlock->Texture, true);
			BasicDraw(Renderer->Shaders[RENDERER_PRIMITIVES], 
					  &Renderer->RenderObjects[R_OBJECT_RECT], 
					  V3(HoverBlock->Text.Pos), V3(HoverBlock->Text.Dim), 
					  HoverBlock->BackgroundColor, 0, {}, true);
			RenderText(Renderer->Shaders[RENDERER_TEXTURED], &HoverBlock->Text,
					   Color(), false);
		} break;
	}
}

#endif