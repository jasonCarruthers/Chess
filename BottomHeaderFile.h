#pragma once;
using namespace std;

//Constants

//Classes
class Game
{
public:
	int mMode;
	double mElapsedTime;
	Mouse *mMouse;
	Button *mButton[Num__GameModes];
	WORD mInitialScreenColor;
	WORD mTileColor[Num__TileColors];
	ChessBoard *mChessBoard;
	ProfileList mProfileList;
public:
	Game()
	{
		//set default values of everything
		mMode = GameMode_MainMenu;
		mElapsedTime = 0;
	}

	Game(int mode)
	{
		mMode = mode;
		mElapsedTime = 0;

		PCONSOLE_SCREEN_BUFFER_INFO initial_screen_info = static_cast<PCONSOLE_SCREEN_BUFFER_INFO>(malloc(sizeof(PCONSOLE_SCREEN_BUFFER_INFO)));
		GetConsoleScreenBufferInfo( GetStdHandle(STD_OUTPUT_HANDLE), initial_screen_info );
		mInitialScreenColor = initial_screen_info->wAttributes;

		mTileColor[TileColor_White] = 240;
		mTileColor[TileColor_Black] = 264;
	}
};
extern Game *mGame;