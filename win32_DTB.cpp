#include <glad\glad.h>
#include <glad.c>

#include <AE\AutoEngine.h>
#include "dtb.h"
#include "dtb.cpp"

#if DEBUG_MODE
int wmain()
#else
int WinMain(HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR     lpCmdLine,
			int       nShowCmd)
#endif
{
	InitLib(LIB_OPENGL | LIB_SDL_FULL);
	render Graphics = 
	InitGraphics("DTB", {700, 720}, SDL_WINDOW_OPENGL);
	v2 WinDim = V2(Graphics.Dim);
	
	Mix_AllocateChannels(100);
	
	SDL_Surface* WindowIcon = IMG_Load("DTB.png");
	if(WindowIcon)
	{
		SDL_SetWindowIcon(Graphics.Window, WindowIcon);
		SDL_FreeSurface(WindowIcon);
	}
	
	SYSTEMTIME SystemTime = {};
	GetSystemTime(&SystemTime);
	srand(SystemTime.wSecond);

	if(!gladLoadGL())
	{
		printf("Failed to load glad: Unknown Error\n");
		return -1;
	}

	glViewport(0, 0, Graphics.Dim.x, Graphics.Dim.y);

	SDL_StartTextInput();
	game_core Game = InitGame(STATE_MAIN_MENU, &Graphics);
	InitGameUI(&Game, &Graphics);
	
	real32 SecPerFrame = 1/Graphics.Display.GameUpdateHZ;
	game_input Input = {};
	Input.KeyState = SDL_GetKeyboardState(0);
	Input.TextInput = InitTextInput(0.5f, SecPerFrame, 0.05f);

	if(SDL_GL_SetSwapInterval(0) == -1)
	{
		OutputDebugString(SDL_GetError());
	}
	
	glEnable(GL_MULTISAMPLE);  
	
	MainGame(&Game, &Input, &Graphics);

	SaveOptionConfig(&Game.OptionMenu.Config, &Game.Skin, &Game.Profile);
	SaveKeybindConfig(&Game.OptionMenu.KeyBind);
	SaveProfile(&Game.Profile);
	
	return 0;
}
