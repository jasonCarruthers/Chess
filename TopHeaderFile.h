#pragma once
using namespace std;



//Constants
const double MIN_MOUSE_CLICK_OFFSET_TIME = .2;
//const int FONTSIZE10_WIDTH = 8;
//const int FONTSIZE10_HEIGHT = 12;
const int FONTSIZE_WIDTH = 12;
const int FONTSIZE_HEIGHT = 16;
//const int SCREEN_WIDTH = 80 * FONTSIZE10_WIDTH;
//const int SCREEN_HEIGHT = 25 * FONTSIZE10_HEIGHT;
const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 740;
const int CHESS_BOARD_WIDTH = 8;
const int CHESS_BOARD_HEIGHT = 8;
const int MAX_STEP_VECTORS_PER_PIECE = 8;
const int TILE_OFFSET_X = 8;
const int TILE_OFFSET_Y = 3;
const double CHECK_FOR_SPACE_PRESS_DURATION = 100.0;

//Enums
enum GameModes
{
	GameMode_MainMenu = -1,

	GameMode_PlayerVersusPlayer,
	GameMode_PlayerVersusCPU,

	Num__GameModes,
};

enum ChessBoardPlayerPositions
{
	Position_Top,
	Position_Bottom,

	Num__Positions,
};

enum TileColors
{
	TileColor_White,
	TileColor_Black,

	Num__TileColors,
};

enum ChessPiecesList
{
	Piece_Pawn1,
	Piece_Pawn2,
	Piece_Pawn3,
	Piece_Pawn4,
	Piece_Pawn5,
	Piece_Pawn6,
	Piece_Pawn7,
	Piece_Pawn8, //1
	Piece_Rook1, //5
	Piece_Knight1, //3
	Piece_Bishop1, //4
	Piece_King, //infinate
	Piece_Queen, //9
	Piece_Bishop2,
	Piece_Knight2,
	Piece_Rook2,

	Num__ChessPieces,
};

enum HorizontalTileNames
{
	HorizonatalTileName_A,
	HorizonatalTileName_B,
	HorizonatalTileName_C,
	HorizonatalTileName_D,
	HorizonatalTileName_E,
	HorizonatalTileName_F,
	HorizonatalTileName_G,
	HorizonatalTileName_H,

	Num__HorizontalTileNames,
};

enum EndStatesOfTheGame
{
	EndState_EndNotReached = -1,

	EndState_Win,
	EndState_Stalemate,
	EndState_Loss,

	Num__EndStates,
};

enum WriteTypes
{
	WriteType_New,
	WriteType_Append,

	Num__WriteTypes,
};

//Classes
class MyLoc
{
public:
	int mX;
	int mY;
public:
	MyLoc()
	{
	}

	MyLoc(int x, int y)
	{
		mX = x;
		mY = y;
	}
};

class MyRect
{
public:
	MyLoc mLoc;
	int mWidth;
	int mHeight;
public:
	MyRect()
	{
	}

	MyRect(int x, int y, int width, int height)
	{
		mLoc.mX = x;
		mLoc.mY = y;
		mWidth = width;
		mHeight = height;
	}

	MyRect(MyLoc loc, int width, int height)
	{
		mLoc = loc;
		mWidth = width;
		mHeight = height;
	}
};

class Mouse
{
public:
	LPPOINT mLoc;
	bool mIsPressed;
	double mTimeOfLastPress;
public:
	Mouse()
	{
		mLoc = new POINT();

		mLoc->x = 0;
		mLoc->y = 0;
		mIsPressed = false;
	}
};

class Button
{
public:
	MyRect mRect;
	string mText;
	bool mIsMousedOver;
public:
	void SetText(int game_mode);
public:
	Button()
	{
		mRect = MyRect(0,0,0,0);
	}

	Button(MyRect rect, int game_mode)
	{
		mRect = rect;
		SetText(game_mode);
		mIsMousedOver = false;
	}
};

class Piece
{
public:
	MyLoc mLoc;
	MyLoc mStepVector[MAX_STEP_VECTORS_PER_PIECE];
	bool mMovementIsContinuous; //if false it's solitary, like a pawn's movement
	bool mIsTaken;
public:
	Piece()
	{
	}

	Piece(MyLoc start_loc)
	{
		mLoc = start_loc;
		mIsTaken = false;
	}
};

class ChessBoardTile
{
public:
	MyLoc mLoc;
	WORD mTileColorAttributes;
};

class ChessBoard
{
public:
	MyRect mRect;
	ChessBoardTile mTile[CHESS_BOARD_WIDTH][CHESS_BOARD_HEIGHT];
public:
	ChessBoard()
	{
	}

	ChessBoard(MyRect rect, WORD color_white, WORD color_black)
	{
		mRect = rect;

		for(int x = 0; x < CHESS_BOARD_WIDTH; x++)
		{
			for(int y = 0; y < CHESS_BOARD_HEIGHT; y++)
			{
				if( (x + y) % 2 == 0 )
					mTile[x][y].mTileColorAttributes = color_white;
				else
					mTile[x][y].mTileColorAttributes = color_black;

				mTile[x][y].mLoc.mX = rect.mLoc.mX + x * TILE_OFFSET_X;
				mTile[x][y].mLoc.mY = rect.mLoc.mY + y * TILE_OFFSET_Y;
			}
		}
	}
};

class Name
{
public:
	string mFirstName;
	string mLastName;
public:
	Name()
	{
	}

	Name(string first_name, string last_name)
	{
		mFirstName = first_name;
		mLastName = last_name;
	}
};

class PlayHistory
{
public:
	Name mOpponentName;
	int mTotalGames;
	int mTotalWins;
	int mTotalStalemates;
	int mTotalLosses;
public:
	PlayHistory()
	{
	}

	PlayHistory(string opponent_first_name, string opponent_last_name, int num_wins, int num_stalemates, int num_losses)
	{
		mOpponentName.mFirstName = opponent_first_name;
		mOpponentName.mLastName = opponent_last_name;
		mTotalGames = num_wins + num_stalemates + num_losses;
		mTotalWins = num_wins;
		mTotalStalemates = num_stalemates;
		mTotalLosses = num_losses;
	}
};
typedef std::list<PlayHistory*> PlayHistoryList;


class Profile
{
public:
	Name mName;
	PlayHistoryList mPlayHistoryList;
	int mTotalGames;
	int mTotalWins;
	int mTotalStalemates;
	int mTotalLosses;
public:
	Profile()
	{
		mTotalGames = 0;
		mTotalWins = 0;
		mTotalStalemates = 0;
		mTotalLosses = 0;
	}

	Profile(Name player_name)
	{
		mName.mFirstName = player_name.mFirstName;
		mName.mLastName = player_name.mLastName;

		mTotalGames = 0;
		mTotalWins = 0;
		mTotalStalemates = 0;
		mTotalLosses = 0;
	}
};
typedef std::list<Profile*> ProfileList;

class PieceMove
{
public:
	char mPieceType;
	MyLoc mStartLoc;
	MyLoc mDestLoc;
	MyLoc mMoveVector;
	MyLoc mTruncatedMoveVector;
public:
	void SetVectors();
public:
	PieceMove()
	{
	}

	PieceMove(int piece_type, MyLoc start_loc, MyLoc dest_loc)
	{
		mPieceType = piece_type;
		mStartLoc = start_loc;
		mDestLoc = dest_loc;
	}
};