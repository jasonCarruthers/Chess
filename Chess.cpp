#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include <windows.h>
#include <fstream>

#include "TopHeaderFile.h"
#include "Player.h"
#include "BottomHeaderFile.h"
using namespace std;

/*USED FOR...WHAT AGAIN?*/
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif /*_WIN32_WINNT*/

//Global variables
Game *mGame;


//Function prototypes
void LoadPlayerProfiles();
void SavePlayerProfile(Name player_name, int write_type);
void SavePlayerPlayHistory(Player current_player, Player opposing_player, int endgamestate_relative);
void SaveNonPlayerGameHistory(Name name1, Name name2);
void InitGame();
void GetPlayerName(Player &player);
PieceMove GetPlayerMove(Player current_player, int current_player_position);
bool IsMoveLegal(PieceMove player_move, Player &current_player, Player &opposing_player, int current_player_position);
bool DoesRectContainPixel(MyRect rect, MyLoc loc);
void TakePlayersTurn(Player &current_player, Player &opposing_player, int current_player_position);
void DrawHeader();
void DrawInitialSetupButtons();
void DrawChessBoard();
void DrawTileBackground(WORD color, MyLoc start_loc);
void DrawPlayerChessPieces(Player player1, Player player2);
void DrawPiecesAndBoard(Player player1, Player player2);
void DrawBoardIndex();
void DrawPlayerProfile(Player player);
void UpdateMouse();
void UpdatePawnStepVectors(Player &current_player, Player opposing_player, int position);
bool UpdateCheckmateStatusOfPlayer(Player current_player, Player opposing_player, int board_position);
//Player.cpp function prototypes (necessary as refereces. Only functions that exist in player.cpp AND exist in this .cpp file need
//to be prototypes in both .cpp files.
void InitPlayer(Player &player, WORD color, int position);


//Global arrays
string mButtonName[Num__GameModes] =
{
	"Player Versus Player",
	"Player Versus CPU (NOT YET FUNCTIONAL)",
};

char mHorizontalTileName[Num__HorizontalTileNames] =
{
	'A',
	'B',
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
};

string mPlayerName[Num__Positions] =
{
	"Player One",
	"Player Two",
};


//Functions
void LoadPlayerProfiles()
{
	//initialization
	ifstream iFile;
	int num_player_profiles = 0;
	string file_text ("\0");
	Name player_name;
	Name opponent_name;
	int num_wins = -1;
	int num_stalemates = -1;
	int num_losses = -1;


	//open file
	iFile.open("C:\\Users\\Jason\\Desktop\\Visual Studio 2008\\Projects\\Chess\\Chess\\player profiles.txt");

	//get how many high scores there are
	if(iFile.is_open())
	{
		while(!iFile.eof())
		{
			bool end_of_opponents_found = false;

			//get past first dividing line
			getline(iFile, file_text, '\n');
			file_text.clear();

			//get the name of player profile
			//first name
			getline(iFile, file_text, ' ');
			player_name.mFirstName = file_text;
			file_text.clear();
			//last name
			getline(iFile, file_text, '\n');
			player_name.mLastName = file_text;
			file_text.clear();
			
			//get past second dividing line
			getline(iFile, file_text, '\n');
			//get past word "Opponent"
			getline(iFile, file_text, '\n');
			
			//create player profile data
			Profile *new_profile = new Profile(player_name);
			mGame->mProfileList.push_back(new_profile);

			//get past third, final dividing line per profile
			getline(iFile, file_text, '-');
			getline(iFile, file_text, '\n');
			file_text.clear();

			//LOOP HERE
			while( !end_of_opponents_found && !iFile.eof() )
			{
				//get past asterisk
				getline(iFile, file_text, ' ');
				file_text.clear();

				//get opponent first name
				getline(iFile, file_text, ' ');
				opponent_name.mFirstName = file_text;
				file_text.clear();
				//get opponent last name
				getline(iFile, file_text, '<');
				opponent_name.mLastName = file_text;
				file_text.clear();

				//read num wins relative to opponent
				getline(iFile, file_text, ',');
				num_wins = atoi(file_text.c_str());
				getline(iFile, file_text, ' ');
				file_text.clear();
				//read num stalemates relative to opponent
				getline(iFile, file_text, ',');
				num_stalemates = atoi(file_text.c_str());
				getline(iFile, file_text, ' ');
				file_text.clear();
				//read num losses relative to opponent
				getline(iFile, file_text, '>');
				num_losses = atoi(file_text.c_str());
				file_text.clear();

				//set more player profile data
				new_profile->mTotalGames = new_profile->mTotalGames + num_wins + num_stalemates + num_losses;
				new_profile->mTotalWins += num_wins;
				new_profile->mTotalStalemates += num_stalemates;
				new_profile->mTotalLosses += num_losses;
				PlayHistory *new_play_history = new PlayHistory(opponent_name.mFirstName, opponent_name.mLastName, num_wins, num_stalemates, num_losses); 
				new_profile->mPlayHistoryList.push_back(new_play_history);

				//determine what the value of end_of_opponents_found is
				//get past the carriage return
				getline(iFile, file_text, '\n');
				file_text.clear();
				char next_char;
				iFile >> next_char;
 				if(next_char != '*')
					end_of_opponents_found = true;
			}

			//read last two spaces
			//getline(iFile, file_text, '\n');
			//getline(iFile, file_text, '\n');
			//file_text.clear();
		}
	}
}

void SavePlayerProfile(Name player_name, int write_type)
{
	ofstream iFile;

	//open file
	if(write_type == WriteType_New)
		iFile.open("C:\\Users\\Jason\\Desktop\\Visual Studio 2008\\Projects\\Chess\\Chess\\player profiles.txt");
	else if(write_type == WriteType_Append)
		iFile.open("C:\\Users\\Jason\\Desktop\\Visual Studio 2008\\Projects\\Chess\\Chess\\player profiles.txt", ios::app);
	
	iFile << "----------------------------------------------------------\n";
	iFile << player_name.mFirstName << " " << player_name.mLastName << "\n";
	iFile << "----------------------------------------------------------\n";
	iFile << "Opponents\n";
	iFile << "---------\n";

	for( std::list<Profile*>::iterator profile_iter = mGame->mProfileList.begin();profile_iter != mGame->mProfileList.end();
		++profile_iter)
	{
		Profile *profile = (Profile*)(*profile_iter);

		if(player_name.mFirstName == profile->mName.mFirstName && player_name.mLastName == profile->mName.mLastName)
		{
			for( std::list<PlayHistory*>::iterator playhistory_iter = profile->mPlayHistoryList.begin(); 
				playhistory_iter != profile->mPlayHistoryList.end(); ++playhistory_iter)
			{
				PlayHistory *play_history = (PlayHistory*)(*playhistory_iter);
			
				iFile << "* " << play_history->mOpponentName.mFirstName << " " << play_history->mOpponentName.mLastName <<
					" <" << play_history->mTotalWins << ", " << play_history->mTotalStalemates << ", " << 
					play_history->mTotalLosses << ">\n";
			}
			break;
		}
	}

	iFile << "\n\n";

	iFile.close();
}

void SavePlayerPlayHistory(Player current_player, Player opposing_player, int endgamestate_relative)
{
	bool play_history_exists = false;

	for( std::list<Profile*>::iterator profile_iter = mGame->mProfileList.begin();profile_iter != mGame->mProfileList.end();
	++profile_iter)
	{
		Profile *profile = (Profile*)(*profile_iter);
		
		if(current_player.mName.mFirstName == profile->mName.mFirstName && current_player.mName.mLastName == profile->mName.mLastName)
		{
			//search for old play history. if none exists, create new
			for( std::list<PlayHistory*>::iterator playhistory_iter = profile->mPlayHistoryList.begin(); 
				playhistory_iter != profile->mPlayHistoryList.end(); ++playhistory_iter)
			{
				PlayHistory *play_history = (PlayHistory*)(*playhistory_iter);
				
				if(opposing_player.mName.mFirstName == play_history->mOpponentName.mFirstName &&
					opposing_player.mName.mLastName == play_history->mOpponentName.mLastName)
				{
					switch(endgamestate_relative)
					{
					case EndState_Win:			play_history->mTotalWins++; break;
					case EndState_Stalemate:	play_history->mTotalStalemates++; break;
					case EndState_Loss:			play_history->mTotalLosses++; break;
					default:					//should never reach this line
						break;
					}

					play_history->mTotalGames++;
					play_history_exists = true;
					
					break;
				}
			}

			if( !play_history_exists )
			{
				PlayHistory *new_play_history = new PlayHistory(opposing_player.mName.mFirstName,
												opposing_player.mName.mLastName, 0, 0, 0);

				switch(endgamestate_relative)
				{
				case EndState_Win:			new_play_history = new PlayHistory(opposing_player.mName.mFirstName,
												opposing_player.mName.mLastName, 1, 0, 0);
												break;
				case EndState_Stalemate:	new_play_history = new PlayHistory(opposing_player.mName.mFirstName,
												opposing_player.mName.mLastName, 0, 1, 0);
												break;
				case EndState_Loss:			new_play_history = new PlayHistory(opposing_player.mName.mFirstName,
												opposing_player.mName.mLastName, 0, 0, 1);
												break;
				default:					//should never reach this line
					break;
				}
				profile->mPlayHistoryList.push_back( new_play_history );
			}
		}
	}
}

void SaveNonPlayerGameHistory(Name name1, Name name2)
{
	for( std::list<Profile*>::iterator profile_iter = mGame->mProfileList.begin();profile_iter != mGame->mProfileList.end();
	++profile_iter)
	{
		Profile *profile = (Profile*)(*profile_iter);

		if( !((name1.mFirstName == profile->mName.mFirstName && name1.mLastName == profile->mName.mLastName) ||
			(name2.mFirstName == profile->mName.mFirstName && name2.mLastName == profile->mName.mLastName)) )
		{
			SavePlayerProfile(profile->mName, WriteType_Append);
		}
	}
	
}

void InitGame()
{
	srand(static_cast<int>(time(0)));

	//change window size
	HWND console = GetConsoleWindow();
	SMALL_RECT window_rect;
	window_rect.Left = 0;
	window_rect.Top = 0;
	window_rect.Right = SCREEN_WIDTH;
	window_rect.Bottom = SCREEN_HEIGHT;

	//change font size
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &window_rect);
	PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx = new CONSOLE_FONT_INFOEX;
	lpConsoleCurrentFontEx->cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(GetStdHandle (STD_OUTPUT_HANDLE), NULL, lpConsoleCurrentFontEx); //retrive all console font informations	
	lpConsoleCurrentFontEx->FontWeight = 700;
	swprintf_s(lpConsoleCurrentFontEx->FaceName, L"Lucida Console");
	lpConsoleCurrentFontEx->dwFontSize.X = 7;
	lpConsoleCurrentFontEx->dwFontSize.Y = 14;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, lpConsoleCurrentFontEx);
	delete lpConsoleCurrentFontEx;

	//change window location, redraw window
	MoveWindow(console, window_rect.Left, window_rect.Top, window_rect.Right, window_rect.Bottom, TRUE);

	//change screen buffer size
	//COORD new_screen_size;
	//new_screen_size.X = SCREEN_WIDTH;
	//new_screen_size.Y = SCREEN_HEIGHT;
	//SetConsoleScreenBufferSize( GetStdHandle(STD_OUTPUT_HANDLE), new_screen_size );

	//init game objects
	mGame = new Game(GameMode_MainMenu);
	mGame->mMouse = new Mouse();
	mGame->mButton[GameMode_PlayerVersusPlayer] = new Button(MyRect(0,3,
		strlen(mButtonName[GameMode_PlayerVersusPlayer].c_str()), 1), GameMode_PlayerVersusPlayer);
	mGame->mButton[GameMode_PlayerVersusCPU] = new Button(MyRect(0,4,
		strlen(mButtonName[GameMode_PlayerVersusCPU].c_str()), 1), GameMode_PlayerVersusCPU);
	mGame->mChessBoard = new ChessBoard(MyRect(3, 3, 8, 8), mGame->mTileColor[TileColor_White], mGame->mTileColor[TileColor_Black]);

	//load all previous games' information
	LoadPlayerProfiles();
}

void GetPlayerName(Player &player)
{
	Name full_name;
	bool player_exists = false;

	//get player's first name
	cout << "Please enter your first name.\n";
	cin >> full_name.mFirstName;
	
	//get player's last name
	cout << "Please enter your last name.\n";
	cin >> full_name.mLastName;


	//perform a search of player through existing profiles
	std::list<Profile*>::iterator profile_iter = mGame->mProfileList.begin();
	for( ;profile_iter != mGame->mProfileList.end(); ++profile_iter)
	{
		Profile *profile = (Profile*)(*profile_iter);

		if(full_name.mFirstName == profile->mName.mFirstName && full_name.mLastName == profile->mName.mLastName)
		{
			player_exists = true;
			break;
		}
	}

	//does player exist? if not create a new profile
	if(player_exists)
	{
		Profile *profile = (Profile*)(*profile_iter);

		cout << "Welcome back " << full_name.mFirstName << " " << full_name.mLastName << "!\n\n";
		cout << "Your current track record:\n";
		cout << "Total games:\t" << profile->mTotalGames << "\n";
		cout << "Total wins:\t" << profile->mTotalWins << "\n";
		cout << "Total stalemates:\t" << profile->mTotalStalemates << "\n";
		cout << "Total losses:\t" << profile->mTotalLosses << "\n\n";

		//display complete play history
		int game = 1;
		for(std::list<PlayHistory*>::iterator playhistory_iter = profile->mPlayHistoryList.begin();
			playhistory_iter != profile->mPlayHistoryList.end(); ++playhistory_iter)
		{
			PlayHistory *play_history = (PlayHistory*)(*playhistory_iter);

			cout << "Game " << game << "\n-------------------------------\n";
			cout << "Opponent:\t" << play_history->mOpponentName.mFirstName << " " << play_history->mOpponentName.mLastName << "\n";
			cout << "You had " << play_history->mTotalWins << " wins,\n";
			cout << play_history->mTotalStalemates << " stalemates,\n";
			cout << "and " << play_history->mTotalLosses << " losses.\n";

			game++;
		}
	}
	else
	{
		Profile *new_profile = new Profile(full_name);
		mGame->mProfileList.push_back(new_profile);

		cout << "A profile has been created for you, " << full_name.mFirstName << " " << full_name.mLastName << ".\n";
	}

	system("pause");
	system("cls");

	player.mName.mFirstName = full_name.mFirstName;
	player.mName.mLastName = full_name.mLastName;
}

PieceMove GetPlayerMove(Player current_player, int current_player_position)
{
	//initialization
	COORD cursor_loc;
	cursor_loc.X = 0;
	cursor_loc.Y = (SCREEN_HEIGHT / FONTSIZE_HEIGHT) - 11;

	string player_input = "";

	PieceMove player_move(-1, MyLoc(-1,-1), MyLoc(-1, -1));

	//do while player_input is seen as invalid
	do
	{
		//inner loop initialization
		player_input = "";
		player_move = PieceMove(-1, MyLoc(-1,-1), MyLoc(-1, -1));

		//get user input
		SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
		cout << "Please enter a move " << mPlayerName[current_player_position] << ".\n";
		cin >> player_input;
		cout << "\n";

		//decode input
		for(int i = 0; i < Num__HorizontalTileNames; i++)
		{
			if(static_cast<char>(toupper(player_input[1])) == mHorizontalTileName[i])
				player_move.mStartLoc.mX = i;
		}
		char buf[2];
		for(int i = 0; i < 2; i++)
			buf[i] = '\0';
		buf[0] = player_input[2];
		player_move.mStartLoc.mY = atoi(buf) - 1;

		for(int i = 0; i < Num__HorizontalTileNames; i++)
		{
			if(static_cast<char>(toupper(player_input[3])) == mHorizontalTileName[i])
				player_move.mDestLoc.mX = i;
		}
		for(int i = 0; i < 2; i++)
			buf[i] = '\0';
		buf[0] = player_input[4];
		player_move.mDestLoc.mY = atoi(buf) - 1;

		//get index of specified piece
		int i = 0;
		for(; i < Num__ChessPieces; i++)
		{
			if(current_player.mPiece[i]->mLoc.mX == player_move.mStartLoc.mX && current_player.mPiece[i]->mLoc.mY == player_move.mStartLoc.mY)
			{
				player_move.mPieceType = i;
				break;
			}
		}
		if( i == Num__ChessPieces - 1 ) //this says if the player index wasn't found/set, then...
		{
			cursor_loc.Y++;
			SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
			cursor_loc.Y--;
			cout << "Cannot find that piece.\n";
			system("pause");
		}
		//if( player_piece_index < 0 )
		//{
		//	cout << "That's an invalid move.\n";
		//	system("pause");
		//}
	} while( !(player_move.mPieceType >= 0 && player_move.mPieceType < Num__ChessPieces) );

	return player_move;
}

bool DoesRectContainPixel(MyRect rect, MyLoc loc)
{
	rect.mLoc.mX *= FONTSIZE_WIDTH;
	rect.mLoc.mY *= FONTSIZE_HEIGHT;
	rect.mWidth *= FONTSIZE_WIDTH;
	rect.mHeight *= FONTSIZE_HEIGHT;

	if(rect.mLoc.mX <= loc.mX && rect.mLoc.mX + rect.mWidth >= loc.mX && rect.mLoc.mY <= loc.mY && rect.mLoc.mY + rect.mHeight >= loc.mY)
		return true;
	return false;
}

void Button::SetText(int game_mode)
{
	mText = mButtonName[ game_mode ];
}

bool IsMoveLegal(PieceMove player_move, Player &current_player, Player &opposing_player, int current_player_position)
{
	//initialization
	COORD cursor_loc;
	cursor_loc.X = 0;
	cursor_loc.Y = (SCREEN_HEIGHT / FONTSIZE_HEIGHT) - 9;
	bool move_is_legal = true;
	

	//get largest dimension of move_vector
	int num_remaining_steps__x = abs(player_move.mMoveVector.mX);
	int num_remaining_steps__y = abs(player_move.mMoveVector.mY);
	int largest_dimension = 0;
	if( !(player_move.mPieceType == Piece_Knight1 || player_move.mPieceType == Piece_Knight2) )
		largest_dimension = (abs(player_move.mMoveVector.mX) > abs(player_move.mMoveVector.mY)) ? abs(player_move.mMoveVector.mX) : abs(player_move.mMoveVector.mY);
	else
		largest_dimension = 1; //assumed to be a knight, which has one step
	
	//this variable is used for adding truncated_move_vec incrementally to
	MyLoc piece_loc_COPY = player_move.mStartLoc;
	
	//--
	//Check if move is illegal here
	//--
	//if selection is out of bounds
	if(player_move.mStartLoc.mX < 0 || player_move.mStartLoc.mY > 7 || player_move.mStartLoc.mY < 0 || player_move.mStartLoc.mY > 7)
		move_is_legal = false;
	//if selection is an enemy piece
	for(int chess_piece = 0; chess_piece < Num__ChessPieces; chess_piece++)
	{
		if(player_move.mStartLoc.mX == opposing_player.mPiece[chess_piece]->mLoc.mX && player_move.mStartLoc.mY == opposing_player.mPiece[chess_piece]->mLoc.mY &&
			!opposing_player.mPiece[chess_piece]->mIsTaken)
		{
			move_is_legal = false;
			break;
		}
	}
	//if move is out of bounds
	if( (player_move.mDestLoc.mX < 0 || player_move.mDestLoc.mX > 7 || player_move.mDestLoc.mY < 0 || player_move.mDestLoc.mY > 7) &&
		(player_move.mStartLoc.mX + player_move.mTruncatedMoveVector.mX * num_remaining_steps__x >= 0) && (player_move.mStartLoc.mX + player_move.mTruncatedMoveVector.mX * num_remaining_steps__x < 8) &&
		(player_move.mStartLoc.mY + player_move.mTruncatedMoveVector.mY * num_remaining_steps__y >= 0) && (player_move.mStartLoc.mY + player_move.mTruncatedMoveVector.mY * num_remaining_steps__y < 8) )
		move_is_legal = false;
	
	//if move is an illegal vector
	if( move_is_legal )
	{
		for(int move_step_vec = 0; move_step_vec < MAX_STEP_VECTORS_PER_PIECE; move_step_vec++)
		{
			//if a pawn moves, (losing it's (0, 2) vector,) and tries moving (0, 2) then what happens? 
			//why does this section set true for move_is_legal...?

			//check vector as a whole, if pawn
			if(player_move.mPieceType >= Piece_Pawn1 && player_move.mPieceType <= Piece_Pawn8)
			{
				if( abs(player_move.mMoveVector.mX) == abs(current_player.mPiece[player_move.mPieceType]->mStepVector[move_step_vec].mX) &&
					abs(player_move.mMoveVector.mY) == abs(current_player.mPiece[player_move.mPieceType]->mStepVector[move_step_vec].mY) )
				{
					move_is_legal = true;
					break;
				}
				else
					move_is_legal = false;
			}
			else
			{
				//check truncated vector
				if( player_move.mTruncatedMoveVector.mX == current_player.mPiece[player_move.mPieceType]->mStepVector[move_step_vec].mX &&
					player_move.mTruncatedMoveVector.mY == current_player.mPiece[player_move.mPieceType]->mStepVector[move_step_vec].mY )
				{
					move_is_legal = true;
					break;
				}
				else
					move_is_legal = false;
			}
		}
	}

	if(move_is_legal)
	{
		for(int i = 0; i < largest_dimension; i++)
		{
			//increment piece_loc_COPY by truncated step vec for every step there is.
			//num steps is equal to the largest_dimension
			piece_loc_COPY.mX += player_move.mTruncatedMoveVector.mX;
			piece_loc_COPY.mY += player_move.mTruncatedMoveVector.mY;

			for(int chess_piece = 0; chess_piece < Num__ChessPieces; chess_piece++)
			{
				//check to see if a piece of the same color is in the way of the move_vector
				if( !current_player.mPiece[chess_piece]->mIsTaken &&
				piece_loc_COPY.mX == current_player.mPiece[chess_piece]->mLoc.mX && piece_loc_COPY.mY == current_player.mPiece[chess_piece]->mLoc.mY)
				{
					move_is_legal = false;
					break;
				}
				//check to see if an opposing player's piece is in the way of the move_vector
				if( !opposing_player.mPiece[chess_piece]->mIsTaken &&
					piece_loc_COPY.mX == opposing_player.mPiece[chess_piece]->mLoc.mX && piece_loc_COPY.mY == opposing_player.mPiece[chess_piece]->mLoc.mY && i < largest_dimension-1)
				{
					move_is_legal = false;
					break;
				}
			}
			if(!move_is_legal)
				break;
		}
	}

	if( !(piece_loc_COPY.mX == player_move.mDestLoc.mX && piece_loc_COPY.mY == player_move.mDestLoc.mY) )
		move_is_legal = false;
	
	//if complete move is legal, make complete move
	if(move_is_legal)
	{
		current_player.mPiece[player_move.mPieceType]->mLoc = player_move.mDestLoc;
		if(player_move.mPieceType >= Piece_Pawn1 && player_move.mPieceType <= Piece_Pawn8)
			current_player.mPiece[player_move.mPieceType]->mStepVector[1] = MyLoc(0,0);
		
		//check to see if an opposing player's piece gets taken
		for(int chess_piece = 0; chess_piece < Num__ChessPieces; chess_piece++)
		{
			if(player_move.mDestLoc.mX == opposing_player.mPiece[chess_piece]->mLoc.mX && player_move.mDestLoc.mY == opposing_player.mPiece[chess_piece]->mLoc.mY)
			{
				cout << "You took a piece!\n";
				opposing_player.mPiece[chess_piece]->mIsTaken = true;
				system("pause");
				break;
			}
		}
	}
	else
	{
		//system("cls");
		cout << "Sorry. That's not a legal move.\n";
		system("pause");
	}

	return move_is_legal;
}

//Make this function turn into IsMoveLegal(), so that the AI can call it for a check on its move as well.
void TakePlayersTurn(Player &current_player, Player &opposing_player, int current_player_position)
{
	PieceMove player_move(-1, MyLoc(0,0), MyLoc(0,0));

	do
	{
		player_move = GetPlayerMove(current_player, current_player_position);
		player_move.SetVectors();
	} while( !IsMoveLegal(player_move, current_player, opposing_player, current_player_position) );

	system("cls");
}

void DrawHeader()
{
	COORD cursor_loc;
	cursor_loc.X = 0;
	cursor_loc.Y = 0;
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
	cout << "Welcome to chess!\n";
}

void DrawInitialSetupButtons()
{
	COORD cursor_loc;

	cursor_loc.X = mGame->mButton[GameMode_PlayerVersusPlayer]->mRect.mLoc.mX;
	cursor_loc.Y = mGame->mButton[GameMode_PlayerVersusPlayer]->mRect.mLoc.mY;
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );

	if(mGame->mButton[GameMode_PlayerVersusPlayer]->mIsMousedOver)
		SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
	cout << mGame->mButton[GameMode_PlayerVersusPlayer]->mText.c_str();
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), mGame->mInitialScreenColor );


	cursor_loc.X = mGame->mButton[GameMode_PlayerVersusCPU]->mRect.mLoc.mX;
	cursor_loc.Y = mGame->mButton[GameMode_PlayerVersusCPU]->mRect.mLoc.mY;
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );

	if(mGame->mButton[GameMode_PlayerVersusCPU]->mIsMousedOver)
		SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );
	cout << mGame->mButton[GameMode_PlayerVersusCPU]->mText.c_str();
	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), mGame->mInitialScreenColor );
}

void DrawChessBoard()
{
	COORD cursor_loc;

	for(int x = 0; x < CHESS_BOARD_WIDTH; x++)
	{
		for(int y = 0; y < CHESS_BOARD_HEIGHT; y++)
		{
			cursor_loc.X = mGame->mChessBoard->mTile[x][y].mLoc.mX - 1;
			cursor_loc.Y = mGame->mChessBoard->mTile[x][y].mLoc.mY - 1;
			MyLoc start_loc = MyLoc(static_cast<int>(cursor_loc.X), static_cast<int>(cursor_loc.Y));
			for(int x2 = start_loc.mX; x2 < start_loc.mX + TILE_OFFSET_X; x2++)
			{
				for(int y2 = start_loc.mY; y2 < start_loc.mY + TILE_OFFSET_Y; y2++)
				{
					cursor_loc.X = x2;
					cursor_loc.Y = y2;
					SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
					SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), mGame->mChessBoard->mTile[x][y].mTileColorAttributes );
					cout << " ";
					SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), mGame->mInitialScreenColor );
				}
			}
		}
	}
}

void DrawPlayerChessPieces(Player player)
{
	COORD cursor_loc;

	for(int i = 0; i < Num__ChessPieces; i++)
	{
		if( !player.mPiece[i]->mIsTaken )
		{
			cursor_loc.X = mGame->mChessBoard->mRect.mLoc.mX + player.mPiece[i]->mLoc.mX * TILE_OFFSET_X;
			cursor_loc.Y = mGame->mChessBoard->mRect.mLoc.mY + player.mPiece[i]->mLoc.mY * TILE_OFFSET_Y + 1;
			SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
			SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), player.mColor );
			if( i >= Piece_Pawn1 && i <= Piece_Pawn8 )
				cout << "PAWN";
			else if( i == Piece_Rook1 || i == Piece_Rook2 )
				cout << "ROOK";
			else if( i == Piece_Knight1 || i == Piece_Knight2 )
				cout << "KNIGHT";
			else if( i == Piece_Bishop1 || i == Piece_Bishop2 )
				cout << "BISHOP";
			else if( i == Piece_Queen )
				cout << "QUEEN";
			else if( i == Piece_King )
				cout << "KING";
		}
	}

	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), mGame->mTileColor[TileColor_Black] );
}


void DrawBoardIndex()
{
	COORD cursor_loc;

	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY );

	for(int x = 0; x < CHESS_BOARD_WIDTH; x++)
	{
		cursor_loc.X = mGame->mChessBoard->mTile[x][0].mLoc.mX;
		cursor_loc.Y = mGame->mChessBoard->mTile[x][0].mLoc.mY - 2;
		SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
		cout << static_cast<char>( static_cast<int>('A' + x) );
	}

	for(int y = 0; y < CHESS_BOARD_HEIGHT; y++)
	{
		cursor_loc.X = mGame->mChessBoard->mTile[0][y].mLoc.mX - 2;
		cursor_loc.Y = mGame->mChessBoard->mTile[0][y].mLoc.mY;
		SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), cursor_loc );
		cout << y + 1;
	}

	SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), mGame->mInitialScreenColor );
}

void DrawPiecesAndBoard(Player player1, Player player2)
{
	DrawBoardIndex();
	DrawChessBoard();
	DrawPlayerChessPieces(player1);
	DrawPlayerChessPieces(player2);
}

void DrawPlayerProfile(Player player)
{
	int player_profile_num = 1;
	for( std::list<Profile*>::iterator profile_iter = mGame->mProfileList.begin();profile_iter != mGame->mProfileList.end();
		++profile_iter)
	{
		Profile *profile = (Profile*)(*profile_iter);

		if(profile->mName.mFirstName == player.mName.mFirstName && profile->mName.mLastName == player.mName.mLastName)
		{
			cout << "Player profile " << player_profile_num << "\n----------------------------------------------\n";
			cout << profile->mName.mFirstName << " " << profile->mName.mLastName << "\n";
			cout << "Total games: " << profile->mTotalGames << "\n";
			cout << "Total wins: " << profile->mTotalWins << "\n";
			cout << "Total stalemates: " << profile->mTotalStalemates << "\n";
			cout << "Total losses: " << profile->mTotalLosses << "\n";
			cout << "----------------------------------------------\n";

			cout << "List of opponents, with relative wins, stalemates, and losses.\n";
			for( std::list<PlayHistory*>::iterator playhistory_iter = profile->mPlayHistoryList.begin(); 
				playhistory_iter != profile->mPlayHistoryList.end(); ++playhistory_iter)
			{
				PlayHistory *play_history = (PlayHistory*)(*playhistory_iter);
				
				cout << play_history->mOpponentName.mFirstName << " " << play_history->mOpponentName.mLastName << " ";
				cout << "<" << play_history->mTotalWins << ", " << play_history->mTotalStalemates << ", " <<
					play_history->mTotalLosses << ">\n";
			}
		}

		player_profile_num++;
	}

	system("pause");
	system("cls");
}

void UpdateMouse()
{
	GetCursorPos(mGame->mMouse->mLoc);
	//To account for window border...
	mGame->mMouse->mLoc->x -= 7;
	mGame->mMouse->mLoc->y -= 30;

	if( GetAsyncKeyState( VK_LBUTTON ) && mGame->mMouse->mTimeOfLastPress + MIN_MOUSE_CLICK_OFFSET_TIME <= mGame->mElapsedTime )
	{
		mGame->mMouse->mIsPressed = true;
		mGame->mMouse->mTimeOfLastPress = mGame->mElapsedTime;
	}
	else
		mGame->mMouse->mIsPressed = false;
}

void UpdatePawnStepVectors(Player &current_player, Player opposing_player, int position)
{
	//search the two spots where this pawn can attack next turn and if there are any enemy pieces there, then set
	//the mStepVector accordingly
	for(int player_piece_index = Piece_Pawn1; player_piece_index <= Piece_Pawn8; player_piece_index++)
	{	
		current_player.mPiece[player_piece_index]->mStepVector[2] = MyLoc(0,0); //clear the (0,2) step_vecs
		current_player.mPiece[player_piece_index]->mStepVector[3] = MyLoc(0,0); //clear the (0,2) step_vecs

		for(int chess_piece = 0; chess_piece < Num__ChessPieces; chess_piece++)
		{
			if(position == Position_Top &&
				current_player.mPiece[player_piece_index]->mLoc.mX + -1 == opposing_player.mPiece[chess_piece]->mLoc.mX &&
				current_player.mPiece[player_piece_index]->mLoc.mY + 1 == opposing_player.mPiece[chess_piece]->mLoc.mY)
				current_player.mPiece[player_piece_index]->mStepVector[2] = MyLoc(-1, 1);
			if(position == Position_Top &&
				current_player.mPiece[player_piece_index]->mLoc.mX + 1 == opposing_player.mPiece[chess_piece]->mLoc.mX &&
				current_player.mPiece[player_piece_index]->mLoc.mY + 1 == opposing_player.mPiece[chess_piece]->mLoc.mY)
				current_player.mPiece[player_piece_index]->mStepVector[3] = MyLoc(1, 1);

			if(position == Position_Bottom &&
				current_player.mPiece[player_piece_index]->mLoc.mX + -1 == opposing_player.mPiece[chess_piece]->mLoc.mX &&
				current_player.mPiece[player_piece_index]->mLoc.mY + -1 == opposing_player.mPiece[chess_piece]->mLoc.mY)
				current_player.mPiece[player_piece_index]->mStepVector[2] = MyLoc(-1, -1);
			if(position == Position_Bottom &&
				current_player.mPiece[player_piece_index]->mLoc.mX + 1 == opposing_player.mPiece[chess_piece]->mLoc.mX &&
				current_player.mPiece[player_piece_index]->mLoc.mY + -1 == opposing_player.mPiece[chess_piece]->mLoc.mY)
				current_player.mPiece[player_piece_index]->mStepVector[3] = MyLoc(1, -1);
		}
	}
}

bool UpdateCheckmateStatusOfPlayer(Player current_player, Player opposing_player, int board_position)
{
	if(opposing_player.mPiece[Piece_King]->mIsTaken)
	{
		system("cls");
		cout << opposing_player.mName.mFirstName << " " << opposing_player.mName.mLastName << ", you lost.\n";
		cout << current_player.mName.mFirstName << " " << current_player.mName.mLastName << ", you won! Congradulations.\n";
		cout << "Good game, both of you.\n\n\n";
		system("pause");

		//save data
		SavePlayerPlayHistory(current_player, opposing_player, EndState_Win);
		SavePlayerPlayHistory(opposing_player, current_player, EndState_Loss);

		SavePlayerProfile(current_player.mName, WriteType_New);
		SavePlayerProfile(opposing_player.mName, WriteType_Append);
		//Save all player profiles from the past. iterate through profiles, if name not equal to current || opposing player, save
		SaveNonPlayerGameHistory(current_player.mName, opposing_player.mName);
		
		//main menu
		cout << "The game will now proceed back to the main menu.\n";
		system("pause");
		system("cls");
		mGame->mMode = GameMode_MainMenu;

		return true;
	}

	return false;
}

int main()
{
	/*for(int i = 100; i <= 160; i++)
	{
		SetConsoleTextAttribute( GetStdHandle(STD_OUTPUT_HANDLE), i );
		cout << i << "\t\t\n";
	}
	system("pause");*/
	//END TEMP CODE SECTION
	//highest explored max: 300
	//112: white/gray background, black foreground text
	//271: black background, white foreground text


	//init game
	InitGame();
	//Create player objects
	Player player[2];

	//main loop
	while( true )
	{
		//update mouse before anything else, since other code depends on a current version of the mouse state and coordinates
		UpdateMouse();
		mGame->mElapsedTime += .02;

		//get gamemode from user
		if( mGame->mMode == GameMode_MainMenu )
		{
			//draw
			DrawHeader();
			DrawInitialSetupButtons();

			//update
			if( DoesRectContainPixel(mGame->mButton[GameMode_PlayerVersusPlayer]->mRect, MyLoc(mGame->mMouse->mLoc->x, mGame->mMouse->mLoc->y)) )
				mGame->mButton[GameMode_PlayerVersusPlayer]->mIsMousedOver = true;
			else
				mGame->mButton[GameMode_PlayerVersusPlayer]->mIsMousedOver = false;

			if(mGame->mMouse->mIsPressed && mGame->mButton[GameMode_PlayerVersusPlayer]->mIsMousedOver)
			{
				system("cls");
				mGame->mMode = GameMode_PlayerVersusPlayer;

				//init player
				InitPlayer(player[0], 271, Position_Top);
				InitPlayer(player[1], 160, Position_Bottom);
			}


			if( DoesRectContainPixel(mGame->mButton[GameMode_PlayerVersusCPU]->mRect, MyLoc(mGame->mMouse->mLoc->x, mGame->mMouse->mLoc->y)) )
				mGame->mButton[GameMode_PlayerVersusCPU]->mIsMousedOver = true;
			else
				mGame->mButton[GameMode_PlayerVersusCPU]->mIsMousedOver = false;

			if(mGame->mMouse->mIsPressed && mGame->mButton[GameMode_PlayerVersusCPU]->mIsMousedOver)
			{
				system("cls");
				mGame->mMode = GameMode_PlayerVersusCPU;
			}
		}
		else if(mGame->mMode == GameMode_PlayerVersusPlayer)
		{
			DrawPiecesAndBoard(player[0], player[1]);

			double turn_start_time = mGame->mElapsedTime;

			//player 1's turn
			TakePlayersTurn(player[0], player[1], Position_Top);
			UpdatePawnStepVectors(player[0], player[1], Position_Top);
			UpdatePawnStepVectors(player[1], player[0], Position_Bottom);
			if( UpdateCheckmateStatusOfPlayer(player[0], player[1], Position_Top) == true )
				continue;

			DrawPiecesAndBoard(player[0], player[1]);

			//player 2's turn
			TakePlayersTurn(player[1], player[0], Position_Bottom);
			UpdatePawnStepVectors(player[1], player[0], Position_Bottom);
			UpdatePawnStepVectors(player[0], player[1], Position_Top);
			if( UpdateCheckmateStatusOfPlayer(player[1], player[0], Position_Bottom) == true )
				continue;
		}
		else if(mGame->mMode == GameMode_PlayerVersusCPU)
		{
			//
		}
	}

	system("pause");
	return 0;
}