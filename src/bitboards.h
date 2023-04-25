#ifndef BITBOARDS_H
#define BITBOARDS_H

#include "common.h"

class Bitboards {
	friend class Board;

	public:
		Bitboards();
		
		void setBit(unsigned int bit, U64& bb);
		void unsetBit(unsigned int bit, U64& bb);
		void flipBit(unsigned int bit, U64& bb);
		bool queryBit(unsigned int bit, U64& bb) const;
		int bitScanForward(U64& bb) const;
		int bitScanReverse(U64& bb) const;

	private:

		int step[8];
	
		U64 rank[8];

		U64 file[8];
		
		U64 pieces[2][6];
		
		U64 allPieces[2];
		
		U64 sq[64];
		
		U64 rayAttacks[8][64];
		
		U64 hozAttacks[64];
		
		U64 diagAttacks[64];
		
		U64 kingAttacks[64];
	
		U64 knightAttacks[64];
		
		U64 pawnAttacks[2][64];

};

#endif
