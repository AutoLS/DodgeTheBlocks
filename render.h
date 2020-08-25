#ifndef RENDER_H
#define RENDER_H

enum enum_render_objects
{
	R_OBJECT_RECT,
	R_OBJECT_SPRITE,
	R_OBJECT_TRI,
	R_OBJECT_MAX,
};

enum enum_game_renderers
{
	RENDERER_PRIMITIVES,
	RENDERER_TEXTURED,
	RENDERER_MAX,
};

enum enum_game_render_font
{
	DEFAULT_FONT_SMALL,
	DEFAULT_FONT_MEDIUM,
	DEFAULT_FONT_LARGE,
	FONT_04B_SMALL,
	FONT_04B_MEDIUM,
	FONT_04B_LARGE,
	FONT_04B_EXL,
	RENDER_FONT_MAX,
};

enum enum_game_assets
{
	ASSET_GREY_SLIDER,
	ASSET_GREY_CIRCLE,
	ASSET_TRASH_CAN,
	ASSET_TRASH_CAN_OPEN,
	ASSET_ENEMY_1,
	ASSET_ENEMY_2,
	ASSET_ENEMY_3,
	ASSET_MAX,
};

enum render_text_position
{
	TEXT_POSITION_CUSTOM,
	TEXT_POSITION_CENTERED,
	TEXT_POSITION_TOP_RIGHT,
	TEXT_POSITION_TOP_LEFT,
	TEXT_POSITION_BOTTOM_RIGHT,
	TEXT_POSITION_BOTTOM_LEFT,
};

struct game_renderer
{
	uint32 Shaders[RENDERER_MAX];
	object_data RenderObjects[R_OBJECT_MAX];
	TTF_Font* RenderFonts[RENDER_FONT_MAX];
	texture_data Assets[ASSET_MAX];
};

struct render_text
{
	char* Text;
	bool DeleteText;
	uint32 Texture;
	v2 Pos;
	v2i Dim;
	object_data RenderObject;
	real32 Angle;
	v3 Axis;
};

struct game_background_scroll
{
	v2 Camera1;
	v2 Camera2;
	v2 Dim;
	real32 Speed;
};

game_renderer InitRenderer()
{	
	game_renderer Renderer = {};
	Renderer.Shaders[RENDERER_PRIMITIVES] = 
	CreateRenderer(LoadShaderSource("shaders/primitive_vert.glsl"),
				   LoadShaderSource("shaders/primitive_frag.glsl"));
	Renderer.Shaders[RENDERER_TEXTURED] = 
	CreateRenderer(LoadShaderSource("shaders/textured_vert.glsl"),
				   LoadShaderSource("shaders/textured_frag.glsl"));
	
	v3 RectObjectVert[] =
	{
		V3(0.5f, 0.5f),   V3(1, 1),	//Top Right		
		V3(0.5f, -0.5f),  V3(1, 0),	//Bottom Right
		V3(-0.5f, -0.5f), V3(),		//Bottom Left
		V3(-0.5f, -0.5f), V3(),		//Bottom Left
		V3(-0.5f, 0.5f),  V3(0, 1),	//Top Left
		V3(0.5f, 0.5f),   V3(1, 1)	//Top Right	
	};
	
	v3 TriObjectVert[] =
	{
		V3(0, 0.5f),  V3(0.5, 1),	//Top Centre
		V3(-0.5f, -0.5f), V3(),		//Bottom Left
		V3(0.5f, -0.5f),  V3(1, 0),	//Bottom Right
	};
	
	Renderer.RenderObjects[R_OBJECT_RECT] = 
	InitObject(RectObjectVert, 6, ArraySize(RectObjectVert));
	UploadObjectData(&Renderer.RenderObjects[R_OBJECT_RECT]);
	AttribPointer(&Renderer.RenderObjects[R_OBJECT_RECT], 0, 3, 6, 0);
	AttribPointer(&Renderer.RenderObjects[R_OBJECT_RECT], 1, 3, 6, 3);
	
	Renderer.RenderObjects[R_OBJECT_SPRITE] = 
	InitObject(RectObjectVert, 6, ArraySize(RectObjectVert), 0, 0, 
				GL_DYNAMIC_DRAW);
	UploadObjectData(&Renderer.RenderObjects[R_OBJECT_SPRITE]);
	AttribPointer(&Renderer.RenderObjects[R_OBJECT_SPRITE], 0, 3, 6, 0);
	AttribPointer(&Renderer.RenderObjects[R_OBJECT_SPRITE], 1, 3, 6, 3);
	
	Renderer.RenderObjects[R_OBJECT_TRI] = 
	InitObject(TriObjectVert, 3, ArraySize(TriObjectVert));
	UploadObjectData(&Renderer.RenderObjects[R_OBJECT_TRI]);
	AttribPointer(&Renderer.RenderObjects[R_OBJECT_TRI], 0, 3, 6, 0);
	AttribPointer(&Renderer.RenderObjects[R_OBJECT_TRI], 1, 3, 6, 3);
	
	Renderer.RenderFonts[DEFAULT_FONT_SMALL] = 
	TTF_OpenFont("ast/fonts/LibreBaskerville-Regular.ttf", 12);
	
	Renderer.RenderFonts[DEFAULT_FONT_MEDIUM] = 
	TTF_OpenFont("ast/fonts/LibreBaskerville-Regular.ttf", 24);
	
	Renderer.RenderFonts[DEFAULT_FONT_LARGE] = 
	TTF_OpenFont("ast/fonts/LibreBaskerville-Regular.ttf", 36);
	
	Renderer.RenderFonts[FONT_04B_SMALL] = 
	TTF_OpenFont("ast/fonts/font.ttf", 12);
	
	Renderer.RenderFonts[FONT_04B_MEDIUM] = 
	TTF_OpenFont("ast/fonts/font.ttf", 24);
	
	Renderer.RenderFonts[FONT_04B_LARGE] = 
	TTF_OpenFont("ast/fonts/font.ttf", 36);
	
	Renderer.RenderFonts[FONT_04B_EXL] = 
	TTF_OpenFont("ast/fonts/font.ttf", 72);
	
	Renderer.Assets[ASSET_GREY_SLIDER] = 
	LoadTextureGLEX("ast/grey_sliderHorizontal.png", true);
	
	Renderer.Assets[ASSET_GREY_CIRCLE] = 
	LoadTextureGLEX("ast/grey_circle.png", true);
	
	Renderer.Assets[ASSET_TRASH_CAN] = 
	LoadTextureGLEX("ast/trashcan.png", true);
	
	Renderer.Assets[ASSET_TRASH_CAN_OPEN] = 
	LoadTextureGLEX("ast/trashcanOpen.png", true);
	
	Renderer.Assets[ASSET_ENEMY_1] = 
	LoadTextureGLEX("ast/enemy/e1.png", true);
	
	Renderer.Assets[ASSET_ENEMY_2] = 
	LoadTextureGLEX("ast/enemy/e2.png", true);
	
	Renderer.Assets[ASSET_ENEMY_3] = 
	LoadTextureGLEX("ast/enemy/e3.png", true);
	
	return Renderer;
}

void ResetBackgroundScroll(game_background_scroll* BackgroundScroll, 
							texture_data Background, v2 WinDim)
{
	BackgroundScroll->Dim = Background.Dim;
	BackgroundScroll->Camera1 = V2(0, WinDim.y * 0.5f);
	BackgroundScroll->Camera2 = V2(0, BackgroundScroll->Camera1.y + 
									  BackgroundScroll->Dim.y);
	BackgroundScroll->Speed = 150.0f;
}

void ScrollBackground(game_background_scroll* BackgroundScroll, v2 WinDim,
					  real32 t)
{
	BackgroundScroll->Camera1.y -= BackgroundScroll->Speed * t;
	BackgroundScroll->Camera2.y -= BackgroundScroll->Speed * t;
	if(BackgroundScroll->Camera1.y + (BackgroundScroll->Dim.y * 0.5f) < 0)
	{
		BackgroundScroll->Camera1.y = (WinDim.y * 0.5f) + 
									   BackgroundScroll->Dim.y;
	}
	if(BackgroundScroll->Camera2.y + (BackgroundScroll->Dim.y * 0.5f) < 0)
	{
		BackgroundScroll->Camera2.y = (WinDim.y * 0.5f) + 
									   BackgroundScroll->Dim.y;
	}
}

render_text LoadRenderText(TTF_Font* Font, char* Text,
						   object_data RenderObject, 
						   bool DeleteText = false,
						   real32 Theta = 0, v3 Axis = {},
						   uint8 Alpha = 255,
						   text_render_type Type = TEXT_RENDER_BLENDED)
{
	render_text RenderText = {};
	RenderText.Angle = Theta;
	RenderText.Axis = Axis;
	RenderText.Text = Text;
	//Default color is always white
	RenderText.Texture = 
	LoadTextGL(Font, Text, {255, 255, 255, Alpha}, {}, Type);
	RenderText.RenderObject = RenderObject;
	TTF_SizeText(Font, RenderText.Text, &RenderText.Dim.x, &RenderText.Dim.y);
	
	if(DeleteText) free(Text);
	
	return RenderText;
}

void UpdateRenderText(render_text* RenderText, TTF_Font* Font, char* Text)
{
	UpdateTextGL(RenderText->Texture, Font, Text, {255, 255, 255, 255});
	TTF_SizeText(Font, Text, &RenderText->Dim.x, &RenderText->Dim.y);
}

void CleanUpRenderText(render_text* RenderText)
{
	if(RenderText->Texture)
	{
		glDeleteTextures(1, &RenderText->Texture);
	}
}

void SetRenderTextPos(render* Graphics, render_text* Text, v2 Offset = V2(), 
					  render_text_position Position = TEXT_POSITION_CENTERED)
{
	v2 WinDim = V2(Graphics->Dim);
	switch(Position)
	{
		case TEXT_POSITION_CUSTOM:
		{
			Text->Pos = Offset;
		}
		
		case TEXT_POSITION_CENTERED:
		{
			Text->Pos = WinDim * 0.5f + Offset; 
		} break;
		
		case TEXT_POSITION_TOP_LEFT:
		{
			Text->Pos = (V2(Text->Dim) * 0.5f) + Offset;
		} break;
		
		case TEXT_POSITION_TOP_RIGHT:
		{
			Text->Pos = (V2(WinDim.x - (Text->Dim.x * 0.5f),
							Text->Dim.y * 0.5f)) + Offset;
		} break;
		
		case TEXT_POSITION_BOTTOM_RIGHT:
		{
			Text->Pos = (WinDim - (V2(Text->Dim) * 0.5f)) + Offset;
		} break;
		
		case TEXT_POSITION_BOTTOM_LEFT:
		{
			Text->Pos = (V2(Text->Dim.x * 0.5f, WinDim.y - (Text->Dim.y * 0.5f)))
						+ Offset;
		} break;
	}
}

void SetRenderTextPos(render_text* Text, v2 ViewDim, v2 Offset = V2(),
					  render_text_position Position = TEXT_POSITION_CENTERED)
{
	switch(Position)
	{
		case TEXT_POSITION_CUSTOM:
		{
			Text->Pos = Offset;
		}
		
		case TEXT_POSITION_CENTERED:
		{
			Text->Pos = ViewDim * 0.5f + Offset; 
		} break;
		
		case TEXT_POSITION_TOP_LEFT:
		{
			Text->Pos = (V2(Text->Dim) * 0.5f) + Offset;
		} break;
		
		case TEXT_POSITION_TOP_RIGHT:
		{
			Text->Pos = (V2(ViewDim.x - (Text->Dim.x * 0.5f),
							Text->Dim.y * 0.5f)) + Offset;
		} break;
		
		case TEXT_POSITION_BOTTOM_RIGHT:
		{
			Text->Pos = (ViewDim - (V2(Text->Dim) * 0.5f)) + Offset;
		} break;
		
		case TEXT_POSITION_BOTTOM_LEFT:
		{
			Text->Pos = (V2(Text->Dim.x * 0.5f, 
						 ViewDim.y - (Text->Dim.y * 0.5f))) + Offset;
		} break;
	}
}

void SetRenderTextPos(render_text* Text, rect32* Rect, v2 Offset = V2(),
					  render_text_position Position = TEXT_POSITION_CENTERED)
{
	switch(Position)
	{
		case TEXT_POSITION_CUSTOM:
		{
			Text->Pos = Offset;
		}
		
		case TEXT_POSITION_CENTERED:
		{
			Text->Pos = Rect->Pos + (Rect->Dim * 0.5f) + Offset; 
		} break;
		
		case TEXT_POSITION_TOP_LEFT:
		{
			Text->Pos = Rect->Pos + (V2(Text->Dim) * 0.5f) + Offset;
		} break;
		
		case TEXT_POSITION_TOP_RIGHT:
		{
			Text->Pos = Rect->Pos + (V2(Rect->Dim.x - (Text->Dim.x * 0.5f),
							Text->Dim.y * 0.5f)) + Offset;
		} break;
		
		case TEXT_POSITION_BOTTOM_RIGHT:
		{
			Text->Pos = Rect->Pos + (Rect->Dim - (V2(Text->Dim) * 0.5f)) 
						+ Offset;
		} break;
		
		case TEXT_POSITION_BOTTOM_LEFT:
		{
			Text->Pos = Rect->Pos + (V2(Text->Dim.x * 0.5f, 
						 Rect->Dim.y - (Text->Dim.y * 0.5f))) + Offset;
		} break;
	}
}

void RenderLineOfText(TTF_Font* Font, game_renderer* Renderer, 
					  const char* Text,
					  v2 Pos, v4 RenderColor,
					  uint8 Alpha = 255)
{
	int Length = (int)strlen(Text);
	
	v2 Position = Pos;
	
	for(int i = 0; i < Length; ++i)
	{
		if(Text[i] == '\n')
		{
			Position.y += TTF_FontHeight(Font);
			Position.x = Pos.x;
		}
		else
		{
			glyph_character Character = 
			LoadGlyphGL(Font, Text[i], {255, 255, 255, Alpha});
			v2 Dim = V2((real32)Character.Width, (real32)Character.Height);
			
			DrawObject(Renderer->Shaders[RENDERER_TEXTURED], 
					   &Renderer->RenderObjects[R_OBJECT_RECT], 
					   V3(Position.x + (Character.Width * 0.5f), Position.y), 
					   V3(Dim),
					   0, {}, RenderColor, Character.Texture, true);
			Position.x += Character.Advance - Character.MinX;
			glDeleteTextures(1, &Character.Texture);
		}
	}
}

void RenderText(uint32 Renderer, render_text* Text,
				v4 Color, bool DeleteTexture = true, real32 Scale = 1.0f)
{
	DrawObject(Renderer, 
			   &Text->RenderObject,
			   V3(Text->Pos), V3(Text->Dim) * Scale, Text->Angle, Text->Axis, 
			   Color, Text->Texture,
			   true);
	if(DeleteTexture) 
	{
		glDeleteTextures(1, &Text->Texture);
	}
}

void RenderTextEffectBlinking(uint32 Renderer, render_text* Text, real32 t)
{
	real32 Alpha = (30 + abs(sinf(t)) * 225) / 255;
	RenderText(Renderer, Text, Color(255, 255, 255, Alpha), false);
}

void LoadAndRenderText(TTF_Font* Font, char* Text, v2 ViewDim,
					   uint32 Renderer,
					   object_data RenderObject,
					   render_text_position Position,
					   v2 Offset = {}, 
					   bool DeleteText = true, 
					   uint8 Alpha = 255,
					   real32 Scale = 1.0f, 
					   real32 Theta = 0, v3 Axis = {},
					   text_render_type Type = TEXT_RENDER_BLENDED)
{
	render_text TextToRender = 
	LoadRenderText(Font, 
				   Text, 
				   RenderObject,
				   DeleteText,
				   Theta, Axis, Alpha);
				   
	SetRenderTextPos(&TextToRender, 
					 ViewDim, 
					 Offset, Position);
					 
	RenderText(Renderer,
		   &TextToRender, 
		   Color(), true, 
		   Scale);
}

void SetWorldSpaceProjection(uint32 Renderer, v2 WorldDimension)
{
	mat4 Projection = Mat4();
	Projection = Ortho(-WorldDimension.x * 0.5f, WorldDimension.x*0.5f, 
					    0, WorldDimension.y, 
					   -1, 1);

	mat4 View = Mat4();
	SetGlobalUniforms(Renderer, Projection, View);
}

void SetWorldSpaceProjectionRev(uint32 Renderer, v2 WorldDimension)
{
	mat4 Projection = Mat4();
	Projection = Ortho(-WorldDimension.x * 0.5f, WorldDimension.x*0.5f, 
					    WorldDimension.y, 0,  
					   -1, 1);

	mat4 View = Mat4();
	SetGlobalUniforms(Renderer, Projection, View);
}

void SetScreenSpaceProjection(uint32 Renderer, v2 WinDim, v2 Camera = {})
{
	mat4 Projection = Mat4();
	Projection = Ortho(0, WinDim.x, 
					    WinDim.y, 0, 
					   -1, 1);
	mat4 View = Mat4();
	View = Translate(View, V3(Camera));
	SetGlobalUniforms(Renderer, Projection, View);
}


#endif