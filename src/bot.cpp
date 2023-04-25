#include <iostream>
#include <cstring>
#include <chrono>
#include <algorithm>
#include "common.h"
#include "bot.h"
#include "board.h"
#include "display.h"

Bot::Bot() {
	reset();
	level = 7;
	transTable = new HASHENTRY[TTSIZE];
	clearTT();
}

Bot::~Bot() {
	delete [] transTable;
}

void Bot::reset() {
	prinVarLine.count = 0;
	oldPrinVarLine.count = 0;
	nodes = qNodes = r = 0;
	totalTime = 0;
	for (int f = 0; f < 64; f++)
			for (int t = 0; t < 64; t++)
				hh[f][t] = 0;
}

int Bot::think(Board& b, int depth) {
	using std::vector;

	auto beginTime1 = std::chrono::high_resolution_clock::now();
	typedef std::chrono::duration<float> fsec;

	int bestMoveSoFar = 0, bestScore = 0;
	int alpha = -CHECKMATE_VAL-1, beta = CHECKMATE_VAL+1;
	int asp = P_VAL/2;
	vector<int> moveList;

	if (b.getNumMovesMade() < 2)
		reset();	

	b.genOrderedMoveList(b.getSide(), moveList);
	b.checkCheck(b.getSide(), moveList);
	
	for (int i = 1; i <= depth; i++) {
		std::cout << "Search to ply " << i << "...\n";

		for (int f = 0; f < 64; f++)
			for (int t = 0; t < 64; t++)
				hh[f][t] /= 2;

		oldPrinVarLine = prinVarLine;

		auto beginTime2 = std::chrono::high_resolution_clock::now();
		nodes = 0;
		qNodes = 0;

		alpha = bestScore - asp;
		beta = bestScore + asp;

		for (;;) {
			bestScore = alphaBeta(b, alpha, beta, i, 0, &prinVarLine, 1, 0);

			if (bestScore <= alpha)  {
				std::cout << "FAIL LOW DEPTH " << i << "\n";
				alpha = -CHECKMATE_VAL-1;
			}
			else if (bestScore >= beta) { 
				std::cout << "FAIL HIGH DEPTH " << i << "\n";
				beta = CHECKMATE_VAL+1;
			}
			else break;
		}

		bestMoveSoFar = prinVarLine.move[0];
		std::cout << "TRANSTABLE INFO\n----------\n";
		std::cout << "actual zobrist: " << b.getZobrist() << '\n';
		std::cout << "entry at TT zobrist..\nkey: " << transTable[int(b.getZobrist()%TTSIZE)].zKey;
		std::cout << "\nbestMoveAndScore: " << transTable[int(b.getZobrist()%TTSIZE)].bestMoveAndScore;
		std::cout << "\ndepthAndNodeType: " << transTable[int(b.getZobrist()%TTSIZE)].depthAndNodeType;
		std::cout << "\n\n";

		auto endTime3 = std::chrono::high_resolution_clock::now();
		fsec diff3 = endTime3 - beginTime1;

		std::cout << "Main nodes: " << nodes << ", q-nodes: " << qNodes << ", total: " << nodes+qNodes << '\n';
		std::cout << "Took: ";
		auto endTime1 = std::chrono::high_resolution_clock::now();
		fsec diff1 = endTime1 - beginTime2;
		std::cout << diff1.count() << "s, ";
		std::cout << "nodes / sec: ";
		std::cout << (nodes+qNodes) / diff1.count() << '\n';
		std::cout << "Current score: " << b.eval() << ", best score: " << bestScore << ", move: " << bestMoveSoFar << '\n';;
		std::cout << "Total time taken: " << diff3.count() << "\n\n";

		if (bestScore >= MATING_VAL)
			break;

		if (i == depth && bestScore < STALEMATE_VAL && bestScore > -STALEMATE_VAL) {
			if (diff3.count() < 0 && prinVarLine.count > 1) {
				depth++;
				i = depth - 1;
				continue;
			}
		}
	}

	for (int i = 0; i < prinVarLine.count; i++) {
		std::cout << intToSquare(prinVarLine.move[i]/100) << " to ";
		std::cout << intToSquare(prinVarLine.move[i]%100) << ", ";
	}
	std::cout << '\n';
	
	auto endTime2 = std::chrono::high_resolution_clock::now();
	fsec diff2 = endTime2 - beginTime1;
	totalTime += diff2.count();
	std::cout << "Total time elapsed: " << totalTime << '\n';

	clearTT();

	return prinVarLine.move[0];
}

int Bot::alphaBeta(Board& b, int alpha, int beta, int depthLeft, int depthGone, LINE* pline, bool allowNull, int ext) {
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			exit(0);
	}

	using std::vector;

	bool s = b.getSide();

	if ((s && !b.piece[wK].getAlive()) || (!s && !b.piece[bK].getAlive())) {
		pline->count = 0;
		storeTTEntry(b, 9999, depthLeft, -CHECKMATE_VAL + depthGone - 1, 0);
		return -CHECKMATE_VAL + depthGone - 1;
	}

	if (allowNull && depthGone) {
		if (b.drawCheck()) {
			pline->count = 0;
			return 0;
		}
	}

	LINE line;

	bool inCheck = b.inCheck(s);


	HASHENTRY hashEntry = getTTEntry(b.getZobrist());

	if (hashEntry.zKey == b.getZobrist() && depthGone != 0) {
		int hashMF = abs(hashEntry.bestMoveAndScore / 1000) / 100;
		int hashMT = abs(hashEntry.bestMoveAndScore / 1000) % 100;

		if (hashEntry.depthAndNodeType / 10 >= depthLeft) {
			if (hashEntry.depthAndNodeType % 10 == 0) {
				int score2 = hashEntry.bestMoveAndScore % 10000;
			//	std::cout << "hashEntry.zKey: " << hashEntry.zKey << "\n";
			//	std::cout << "hashEntry.depthAndNodeType: " << hashEntry.depthAndNodeType << "\n";
			//	std::cout << "hashEntry.bestMoveAndScore: " << hashEntry.bestMoveAndScore << "\n";
			//	std::cout << "score2: " << score2 << "\n";
			
				line.count = 0;
				pline->move[0] = hashMF * 100 + hashMT;
				memcpy(pline->move + 1, line.move, line.count * sizeof(int));
				pline->count =  line.count + 1;

				if (score2 >= beta) { 
					return beta;
				}
				else if (score2 < alpha) { 
					return alpha;
				}
				return score2;
			}
			
			else if (hashEntry.depthAndNodeType % 10 == 1) {
				int score2 = hashEntry.bestMoveAndScore % 10000;

				if (score2 >= beta) { 
					return score2;
				}
				if (score2 > alpha) {
					//alpha = score2;
				}
			}
			
			else if (hashEntry.depthAndNodeType % 10 == 2) {
				int score2 = hashEntry.bestMoveAndScore % 10000;

				if (score2 <= alpha) { 
					return score2;
				}
				if (score2 < beta) {
					//beta = score2;
				}
			}
		}
	}


	if (depthLeft <= 0) {
		
		if (allowNull && b.inCheck(s)) {
			return alphaBeta(b, alpha, beta, 1, depthGone, pline, 0, 0);
		}

		pline->count = 0;
		int v = quies(b, alpha, beta, depthGone);
	
		return v;
	}

	int score;

	while (ext >= 100) {
		depthLeft++;
		ext -= 100;
	}
	
	if (allowNull && !inCheck) {
		if ((s && b.getWhiteMaterial() > ENDGAME_VAL) || (!s && b.getBlackMaterial() > ENDGAME_VAL)) {
			r = depthLeft > 6 ? 4 : 3;
			b.changeTurn();
			b.zobristXorSide();
			score = -alphaBeta(b, -beta, -beta+1, depthLeft-r-1, depthGone, pline, 0, 0);
			b.changeTurn();
			b.zobristXorSide();
			if (score >= beta) { 
				pline->count = 0;
				return score;
			}
			
			else if (score < -MATING_VAL)
				ext += 50;
		}
	}

	bool foundPV = false;
	int mF, mT;
	vector<int> moveList;

	b.genOrderedMoveList(s, moveList);
	b.cleanMoveList(s, moveList);
	
	if (inCheck)
		ext += 75;

	if (moveList.size() == 0) {
		pline->count = 0;
		if (inCheck) {
			storeTTEntry(b, 9999, depthLeft, -CHECKMATE_VAL + depthGone - 1, 0);

			return -CHECKMATE_VAL + depthGone - 1;
		}
		else {
			if (b.getWhiteMaterial() >= b.getBlackMaterial()) {
				int v = s ? -STALEMATE_VAL : STALEMATE_VAL;

				storeTTEntry(b, 9999, depthLeft, v, 0);

				return v;
			}
		}
	}
	else if (moveList.size() == 1) {

		if (depthGone == 0) {
			pline->move[0] = moveList[0];
			pline->count = 1;

			storeTTEntry(b, moveList[0], depthLeft, -STALEMATE_VAL, 0);

			return -STALEMATE_VAL;
		}	

		ext += 50;
	}
	else if (moveList.size() == 2)
		ext += 25;

	if (depthLeft == 1) {
		if (!inCheck && !(abs(alpha) > MATING_VAL || abs(beta) > MATING_VAL)) {
			if (b.eval() + B_VAL < alpha && moveList.size()) {
				pline->count = 0;
				return quies(b, alpha, beta, depthGone);
			}
		}
	}

	else if (depthLeft == 2) {
		if (!inCheck && !(abs(alpha) > MATING_VAL || abs(beta) > MATING_VAL)) {
			if (b.eval() + R_VAL < alpha && moveList.size()) {
				depthLeft--;
			}
		}
	}

	else if (depthLeft == 3) {
		if (!inCheck && !(abs(alpha) > MATING_VAL || abs(beta) > MATING_VAL)) {
			if (b.eval() + Q_VAL < alpha && moveList.size()) {
				depthLeft--;
			}
		}
	}

	int temp;
	if (allowNull) { 
		
		std::vector<int>::iterator pvIndex2;
		if (hashEntry.zKey == b.getZobrist()) {
			int pvmove = hashEntry.bestMoveAndScore / 10000;
			if (pvmove != 9999 && pvmove != 0) {
				pvIndex2 = std::find(moveList.begin(), moveList.end(), pvmove);
				if (pvIndex2 != moveList.end()) {
					temp = *pvIndex2;
					moveList.erase(pvIndex2);
					moveList.insert(moveList.begin()+0, temp);
				}
			}
		}
		
	
		std::vector<int>::iterator pvIndex;
		if (depthGone < oldPrinVarLine.count) {
			int pvmove = oldPrinVarLine.move[depthGone];
			if (pvmove != 0 && depthGone < oldPrinVarLine.count) {
				pvIndex = std::find(moveList.begin(), moveList.end(), pvmove);
				if (pvIndex != moveList.end()) {
					temp = *pvIndex;
					moveList.erase(pvIndex);
					moveList.insert(moveList.begin() + 0, temp);
				}
			}
		}
	}
	
	int movesSearched = 0;

	for (size_t i = 0; i < moveList.size(); i++) {
		nodes++;

		mF = moveList[i]/100;
		mT = moveList[i]%100;
		b.setMove(mF, mT);
		b.movePiece();
		b.changeTurn();

		if (b[mF] != none && b.piece[b[mF]].getValue() == P_VAL) {
			if (s && (mT/10 == 80 || mT/10 == 90))
				ext += 50;
			else if (!s && (mT/10 == 30 || mT/10 == 20))
				ext += 50;
		}

		if (foundPV) {
			if (movesSearched >= 4 && b.getPly() > 4 &&
			    b.getPrevOnMoveTo(b.getNumMovesMade()-1) == none && 
			    !b.inCheck(s) && mT != b.getPmSq(b.getNumMovesMade()-1)) {
				score = -alphaBeta(b, -alpha-1, -alpha, depthLeft-2, depthGone+1, &line, 1, ext-100);
			}
			else
				score = alpha+1;
			if (score > alpha) {
				score = -alphaBeta(b, -alpha-1, -alpha, depthLeft-1, depthGone+1, &line, 1, ext-100);
				if ((score > alpha) && (score < beta)) 
					score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1, ext-100);
			}
		}	
		else
			score = -alphaBeta(b, -beta, -alpha, depthLeft-1, depthGone+1, &line, 1, ext-100);

		b.changeTurn();
		b.unmovePiece();

		if (score >= beta) { 
			if (b[mT] == none) {
				hh[to64(mF)-1][to64(mT)-1] += depthGone*depthGone;
			}
		
			storeTTEntry(b, mF * 100 + mT, depthLeft, beta, 1);

			return beta;
		}
		if (score > alpha) { 
			alpha = score;
			foundPV = true;
		
			pline->move[0] = mF*100 + mT;
			memcpy(pline->move + 1, line.move, line.count * sizeof(int));
			pline->count = line.count + 1;

			storeTTEntry(b, mF * 100 + mT, depthLeft, score, 0);
		}
		else {
			if (transTable[int(b.getZobrist() % TTSIZE)].zKey != b.getZobrist()) {
				storeTTEntry(b, mF * 100 + mT, depthLeft, alpha, 2);
			}
		}

		movesSearched++;
	}
	
	return alpha;
}

int Bot::quies(Board& b, int alpha, int beta, int depthGone) {
	using std::vector;

	bool s = b.getSide();

	if ((s && !b.piece[wK].getAlive()) || (!s && !b.piece[bK].getAlive()))
		return -CHECKMATE_VAL + depthGone-1;

	int currEval = b.eval();
	int score, mF, mT;

	if (currEval >= beta) {
		return beta;
	}
	
	if (currEval > alpha)
		alpha = currEval;

	vector<int> nonQuiesList;

	b.generateCaptures(s, nonQuiesList);

	b.sortCaptures(nonQuiesList);

	b.addPromotions(s, nonQuiesList);
	
	for (size_t i = 0; i < nonQuiesList.size(); i++) {
		mF = nonQuiesList[i]/100;
		mT = nonQuiesList[i]%100;

		if (b.getWhiteMaterial() > ENDGAME_VAL && b.getBlackMaterial() > ENDGAME_VAL)
			if (b[mT] != none && b.piece[b[mT]].getValue() + 1.2*P_VAL + currEval <= alpha)
				if (!b.inCheck(s)) 
					continue;

		qNodes++;

		b.setMove(mF, mT);
		b.movePiece();

		if (b.inCheck(s)) {
			b.unmovePiece();
			continue;
		}

		b.changeTurn();
		score = -quies(b, -beta, -alpha, depthGone+1);

		b.unmovePiece();
		b.changeTurn();

		if (score >= beta) {
			return beta;
		}
		if (score > alpha) { 
			alpha = score;
		}
		else {
		}
	}
	
	return alpha;
}

int Bot::getFromHH(int mF, int mT) {
	assert(mF > -1 && mF < 64);
	assert(mT > -1 && mT < 64);
	return hh[mF][mT];
}

void Bot::clearTT() {
	HASHENTRY clear;
	clear.zKey = 0;
	clear.bestMoveAndScore = 0;
	clear.depthAndNodeType = 0;
	for (int i = 0; i < TTSIZE; i++) {
		transTable[i] = clear;
	}
}

void Bot::storeTTEntry(Board& b, int move, int depthLeft, int score, int nodeType) {
	unsigned long zobrist = b.getZobrist();

	if (transTable[int(zobrist%TTSIZE)].depthAndNodeType/10 <= b.getPly()) {
		HASHENTRY storage;
		storage.zKey = zobrist;
		storage.bestMoveAndScore = move * 10000 + abs(score);
		storage.depthAndNodeType = depthLeft * 10 + nodeType;
		transTable[int(zobrist%TTSIZE)] = storage;
	}
}

HASHENTRY Bot::getTTEntry(unsigned long long key) {
	return transTable[int(key%TTSIZE)];
}
