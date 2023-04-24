#ifndef BITBOARDS_H
#define BITBOARDS_H

#include "common.h"

class Bitboards{

    friend class Board;
    public:

        Bitboards();

        ~Bitboards();

        void setBit(unsigned int bit, int64& bb);
        void unsetBit(unsigned int bit, int64& bb);
        void flipBit(unsigned int bit, int64& bb);
        bool queryBit(unsigned int bit, int64& bb) const;
        int bitScanForward(int64& bb) const;
        int bitScanReverse(int64& bb) const;
    private:

        int step[8];

        int64 rank[8];

        int64 file[8];

        int64 pieces[2][6];

        int64 allPieces[2];

        int64 sq[64];

        int64 rayAttack[8][64];

        int64 hozAttack[64];

        int64 diagonalAttack[64];

        int64 kingAttack[64];

        int64 knightAttack[64];

        int64 pawnAttack[2][64];
 
};

#endif