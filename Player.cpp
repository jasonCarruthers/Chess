#include <iostream>
#include <string>
#include <list>
#include <ctime>
#include <windows.h>

#include "TopHeaderFile.h"
#include "Player.h"
#include "BottomHeaderFile.h"
using namespace std;


//Function prototypes
void InitPlayer(Player &player, WORD color, int position);
//Other .cpp file function prototypes
void GetPlayerName(Player &player);


//Functions
void InitPlayer(Player &player, WORD color, int position)
{
	player = Player(position, color);
	//GetPlayerName(player);
}

void PieceMove::SetVectors()
{
	mMoveVector = MyLoc(mDestLoc.mX - mStartLoc.mX, mDestLoc.mY - mStartLoc.mY);
	mTruncatedMoveVector = mMoveVector;

	if(!(mPieceType == Piece_Knight1 || mPieceType == Piece_Knight2) && mTruncatedMoveVector.mX > 1)
		mTruncatedMoveVector.mX = 1;
	else if(!(mPieceType == Piece_Knight1 || mPieceType == Piece_Knight2) && mTruncatedMoveVector.mX < -1)
		mTruncatedMoveVector.mX = -1;
	if(!(mPieceType == Piece_Knight1 || mPieceType == Piece_Knight2) && mTruncatedMoveVector.mY > 1)
		mTruncatedMoveVector.mY = 1;
	else if(!(mPieceType == Piece_Knight1 || mPieceType == Piece_Knight2) && mTruncatedMoveVector.mY < -1)
		mTruncatedMoveVector.mY = -1;
}