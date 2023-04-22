//demo

#ifndef ZOBRIST_H
#define ZOBRIST_H

class Zobrist {
	public:
	
		Zobrist();

		unsigned long key;

		unsigned long piece[6][2][64];
		
		unsigned long side;
		
        unsigned long castling[2][2];

		unsigned long enPassant[8];
};

#endif