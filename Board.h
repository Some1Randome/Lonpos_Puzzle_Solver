#ifndef BLOCK_PUZZLE_SOLVER_BOARD_H
#define BLOCK_PUZZLE_SOLVER_BOARD_H
#include <vector>
#include "Block.h"
#include "ThreadPool.h"

using namespace std;

struct PlacementAction {
    shared_ptr<Block> block;
    int rotationIndex;
    int mirrorIndex;
    std::vector<std::pair<int, int> > writtenCells;
    std::vector<int> boardCoordinates;
};

class Board {
    vector<vector<int> > fields;

    vector<int> findNextEmpty() const;

    bool placeBlock(const shared_ptr<Board> &boardCopy, const shared_ptr<Block> &block, const vector<int> &coordinates) const;

    bool isFilled() const;

    void actualFillBoard(vector<shared_ptr<Block> > remainingBlocks, int blockIndex, ThreadPool &pool);

    bool placeBlockInPlace(shared_ptr<Block> block, int rotationIndex, int mirrorIndex, vector<int> topLeftCoordinates, PlacementAction &placementRecord);

public:
    void solveThreaded(ThreadPool &pool, const std::vector<shared_ptr<Block> > &blocks);

    void solveFromStartingBlock(const shared_ptr<Block> &startBlock, const std::vector<shared_ptr<Block> > &remainingBlocks);

    explicit Board(const shared_ptr<Board> &toCopy);

    Board(int rows, int columns);

    void printBoard(ostream &ostream) const;
};
#endif //BLOCK_PUZZLE_SOLVER_BOARD_H
