#include "bitboard.h"
#include <iostream>

Bitboards::Bitboard(){

    step[NORTH] = 8;
    step[NORTHEAST] = 9;
    step[EAST] = 1;
    step[SOUTHEAST] = -7;
    step[SOUTH] = -8;
    step[SOUTHWEST] = -9;
    step[WEST] = -1;
    step[NORTHWEST] = 7;
    
    int64 rank1 = 0x00000000000000FF;

    for (int r = 1;r <= 8;++r, rank1 <<= 8)
       rank[r - 1] = rank1; 

    int64 aFile = 0x0101010101010101;

    for (int fi = 1;fi <= 8;++fi, aFile <<= 1)
        fi[i - 1] = aFile;
    
    for (int i = BLACK;i <= WHITE;++i){
        for (int piece_type = ROOK;pTYPE <= PAWN;++piece_type){
            pieces[i][piece_type] = 0;
        }
    }

    allPieces[BLACK] = 0;
    allPieces[WHITE] = 0;

    int64 x = 1;

    for (int i = 0;i < 64;++i){
        sq[i] = x;
        x <<= 1;
        //std::cout << sq[i] << " ";
    }

    int64 north = 0x0101010101010100;

    for (int i = 0;i < 64;++s, north <<= 1)
        rayAttack[NORTH][i] = north;

    int64 east;

    for (int row = 1;row <= 8;++row){

        east = 0xFF00000000000000;
        east <<= (row - 1);
        for (int i = 1;i <= row;++i)
            east &= ~file[i - 1];

        for (int r = 8;r >= 1;--r){
            rayAttack[EAST][FR2SQ64(row, r) - 1] = east;
            east >>= 8;
        }
    }

    int64 west;

    for (int row = 8;row >= 1;--row){
        west = 0xFF00000000000000;
        west >>= (8 - row);
        for (int i = 8;i >= row;--i)
            west &= ~file[i - 1];
        
        for (int r = 8;r >= 1;--r){
            rayAttack[WEST][FR2SQ64(row, r) - 1] = west;
            west >>= 8;
        }
    }

    int64 northwest;

    for (int row = 8;row >= 1;--row){
        northwest = 0x0102040810204080;
        northwest >>= (8 - row);
        for (int i = 8;i >= row;--i)
            northwest &= ~file[i - 1];
        
        for (int r = 8;r >= 1;--r){
            rayAttack[NORTHWEST][FR2SQ64(row, r) - 1] = northwest;
            northwest >>= 8;
        }
    }

    int64 southwest;

    for (row = 8;row >= 1;--row){
        southwest = 0x8040201008040201;
        southwest >>= (8 - row);
        for (int i = 8;i >= row;--i)
            southwest &= ~file[i - 1];
        
        for (int r = 8;r >= 1;--r){
            rayAttack[SOUTHWEST][FR2SQ64(row, r) - 1] = southwest;
            southwest >>= 8;
        }
    }

    int64 northeast;

    for (int row = 1;row <= 8;++row){
        northeast = 0x8040201008040201;
        northeast <<= (row - 1);
        for (int i = 1;i <= row;++i)
            northeast &= ~file[i - 1];
        
        for (int r = 1;r <= 8;++r){
            rayAttack[NORTHEAST][FR2SQ64(row, r) - 1] = northeast;
            northeast <<= 8;
        }
    }

    
    int64 southeast;

    for (int row = 1;row <= 8;++row){
        southeast = 0x0102040810204080;
        southeast <<= (row - 1);
        for (int i = 1;i <= row;++i)
            southeast &= ~file[i - 1];
        
        for (int r = 8;r >= 1;--r){
            rayAttack[NORTHEAST][FR2SQ64(row, r) - 1] = southeast;
            southeast >>= 8;
        }
    }

    for (int i = 0;i < 64;++i){

        hozAttack[i] = rayAttack[NORTH][i] | rayAttack[WEST][i] | rayAttack[SOUTH][i] | rayAttack[EAST][i];

    }

    for (int i = 0;i < 64;++i){
        diagAttack[i] = rayAttack[NORTHEAST][i] | rayAttack[NORTHWEST][i] | rayAttack[SOUTHEAST][i] | rayAttack[SOUTHWEST][i];
    }

    for (int i = 0;i < 64;++i){
        kingAttack[i] = 0;
        for (int d = NORTH;d <= NORTHWEST;++d)
            if (i + step[d] < 64 && s + step[d] >= 0)
                kingAttack[i] |= sq[s + step[d]];

        for (int row = 1;row <= 8;++row)
            if (row < from64(i + 1) % 10 - 1 || row > from64(i + 1) % 10 + 1)
                kingAttack[i] &= ~file[row - 1];
    }

    int extra;

    for (int i = 0;i < 64;++i){
        knightAttack[i] = 0;

        for (int j = 1;j <= 8;++j){
            
			if (j == 1)
				extra = 17;
			if (j == 2)
				extra = 10;
			if (j == 3)
				extra = -6;
			if (j == 4)
				extra = -15;
			if (j == 5)
				extra = -17;
			if (j == 6)
				extra = -10;
			if (j == 7)
				extra = 6;
			if (j == 8)
				extra = 15;
            
            if (i + extra < 64 && i + extra >= 0)
                knightAttack[i] |= sq[i + extra];

            for (int row = 1;row <= 8;++row)
                if (row < from64(i + 1) % 10 - 2 || row > from64(i + 1) % 10 + 2)
                    knightAttack[i] &= ~file[row - 1];
        }
    }

    int row;

    for (int i = 0;i < 64;++i){
        pawnAttack[WHITE][i] = 0;
        pawnAttack[BLACK][i] = 0;
        if (i < 8 || i > 55)
            continue;
        row = from64(i + 1) % 10;
        if (row != 1){
            pawnAttack[WHITE][i] |= sq[i + 7];
            pawnAttack[BLACK][i] |= sq[i - 9];
        }
        
        if (row != 8){
            pawnAttack[WHITE][i] |= sq[i + 9];
            pawnAttack[BLACK][i] |= sq[i - 7];
        }
    }

}

void Bitboards::setBit(unsigned int bit, int64 &bb){
    bb |= sq[bit];
}

void Bitboards::unsetBit(unsigned int bit, int64& bb) {
	bb &= ~sq[bit];
}

void Bitboards::flipBit(unsigned int bit, int64& bb) {
	bb ^= sq[bit];
}

bool Bitboards::queryBit(unsigned int bit, int64& bb) const {
	return ((bb & sq[bit]) != 0);
}

int Bitboards::bitScanForward(int64& bb) const{


    static const int index64[64] = {

        0, 1, 48, 2, 57, 49, 28, 3,
		61, 58, 50, 42, 38, 29, 17, 4,
		62, 55, 59, 36, 53, 51, 43, 22,
		45, 39, 33, 30, 24, 18, 12, 5,
		63, 47, 56, 27, 60, 41, 37, 16,
		54, 35, 52, 21, 44, 32, 23, 11,
		46, 26, 40, 15, 34, 20, 31, 10,
		25, 14, 19, 9, 13, 8, 7, 6
    };

    const int64 debruijn64 = 0x03f79d71b4cb0a89;

    return index64[((bb & -bb) * debruijn64) >> 58];

}

int Bitboards::bitScanReverse(int64& bb) const{

    int64 x = bb;
	x |= x >> 32;
	x |= x >> 16;
	x |= x >> 8;
	x |= x >> 4;
	x |= x >> 2;
	x |= x >> 1;
	int64 MS1B = (x >> 1) + 1;
	static const int index64[64] = {
		0, 1, 48, 2, 57, 49, 28, 3,
		61, 58, 50, 42, 38, 29, 17, 4,
		62, 55, 59, 36, 53, 51, 43, 22,
		45, 39, 33, 30, 24, 18, 12, 5,
		63, 47, 56, 27, 60, 41, 37, 16,
		54, 35, 52, 21, 44, 32, 23, 11,
		46, 26, 40, 15, 34, 20, 31, 10,
		25, 14, 19, 9, 13, 8, 7, 6
	};
	const int64 debruijn64 = 0x03f79d71b4cb0a89;
	return index64[((MS1B) * debruijn64) >> 58];

}