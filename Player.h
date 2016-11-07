#pragma once
using namespace std;


//Classes
class Player
{
public:
	Piece *mPiece[CHESS_BOARD_WIDTH * 2];
	WORD mColor;
	Name mName;
	int mEndState;
public:
	Player()
	{
	}

	Player(int position, WORD color)
	{
		//set end state
		mEndState = EndState_EndNotReached;

		//set color
		mColor = color;

		//set starting locations here. pawn step vectors also need to be set independently
		if(position == Position_Top)
		{
			for(int i = CHESS_BOARD_WIDTH; i < CHESS_BOARD_WIDTH*2; i++)
				mPiece[i] = new Piece(MyLoc(i%CHESS_BOARD_WIDTH, 0));

			for(int i = 0; i < CHESS_BOARD_WIDTH; i++)
				mPiece[i] = new Piece(MyLoc(i%CHESS_BOARD_WIDTH, 1));

			for(int i = Piece_Pawn1; i <= Piece_Pawn8; i++)
			{
				mPiece[i]->mStepVector[0] = MyLoc(0, 1);
				mPiece[i]->mStepVector[1] = MyLoc(0, 2);
				mPiece[i]->mMovementIsContinuous = false;
			}
		}
		else if(position == Position_Bottom)
		{
			for(int i = 0; i < CHESS_BOARD_WIDTH*2; i++)
				mPiece[i] = new Piece(MyLoc(i%CHESS_BOARD_WIDTH, 6 + i / CHESS_BOARD_WIDTH));

			for(int i = Piece_Pawn1; i <= Piece_Pawn8; i++)
			{
				mPiece[i]->mStepVector[0] = MyLoc(0, -1);
				mPiece[i]->mStepVector[1] = MyLoc(0, -2);
				mPiece[i]->mMovementIsContinuous = false;
			}
		}

		//set step vectors here
		for(int i = Piece_Rook1; i <= Piece_Rook2; i += Piece_Rook2 - Piece_Rook1)
		{
			mPiece[i]->mStepVector[0] = MyLoc(0, -1);
			mPiece[i]->mStepVector[1] = MyLoc(1, 0);
			mPiece[i]->mStepVector[2] = MyLoc(0, 1);
			mPiece[i]->mStepVector[3] = MyLoc(-1, 0);
			mPiece[i]->mMovementIsContinuous = true;
		}

		for(int i = Piece_Knight1; i <= Piece_Knight2; i += Piece_Knight2 - Piece_Knight1)
		{
			mPiece[i]->mStepVector[0] = MyLoc(-2, -1);
			mPiece[i]->mStepVector[1] = MyLoc(2, -1);
			mPiece[i]->mStepVector[2] = MyLoc(1, -2);
			mPiece[i]->mStepVector[3] = MyLoc(1, 2);
			mPiece[i]->mStepVector[4] = MyLoc(2, 1);
			mPiece[i]->mStepVector[5] = MyLoc(-2, 1);
			mPiece[i]->mStepVector[6] = MyLoc(-1, -2);
			mPiece[i]->mStepVector[7] = MyLoc(-1, 2);
			mPiece[i]->mMovementIsContinuous = false;
		}

		for(int i = Piece_Bishop1; i <= Piece_Bishop2; i += Piece_Bishop2 - Piece_Bishop1)
		{
			mPiece[i]->mStepVector[0] = MyLoc(-1, -1);
			mPiece[i]->mStepVector[1] = MyLoc(1, -1);
			mPiece[i]->mStepVector[2] = MyLoc(-1, 1);
			mPiece[i]->mStepVector[3] = MyLoc(1, 1);
			mPiece[i]->mMovementIsContinuous = true;
		}

		mPiece[Piece_Queen]->mStepVector[0] = MyLoc(-1, -1);
		mPiece[Piece_Queen]->mStepVector[1] = MyLoc(-1, 1);
		mPiece[Piece_Queen]->mStepVector[2] = MyLoc(-1, 0);
		mPiece[Piece_Queen]->mStepVector[3] = MyLoc(1, 1);
		mPiece[Piece_Queen]->mStepVector[4] = MyLoc(1, 0);
		mPiece[Piece_Queen]->mStepVector[5] = MyLoc(1, -1);
		mPiece[Piece_Queen]->mStepVector[6] = MyLoc(0, 1);
		mPiece[Piece_Queen]->mStepVector[7] = MyLoc(0, -1);
		mPiece[Piece_Queen]->mMovementIsContinuous = true;

		mPiece[Piece_King]->mStepVector[0] = MyLoc(-1, -1);
		mPiece[Piece_King]->mStepVector[1] = MyLoc(-1, 1);
		mPiece[Piece_King]->mStepVector[2] = MyLoc(-1, 0);
		mPiece[Piece_King]->mStepVector[3] = MyLoc(1, 1);
		mPiece[Piece_King]->mStepVector[4] = MyLoc(1, 0);
		mPiece[Piece_King]->mStepVector[5] = MyLoc(1, -1);
		mPiece[Piece_King]->mStepVector[6] = MyLoc(0, 1);
		mPiece[Piece_King]->mStepVector[7] = MyLoc(0, -1);
		mPiece[Piece_King]->mMovementIsContinuous = false;	
	}
};