#ifndef BLOCK_PUZZLE_SOLVER_BLOCKS_H
#define BLOCK_PUZZLE_SOLVER_BLOCKS_H
#include <memory>
#include <vector>

using namespace std;

class Block {
    vector<vector<int> > fields;
    int rotationCount;
    int mirrorCount;
    int color;

    vector<vector<int> > rotateFields(int amount);

    vector<vector<int> > mirrorFields(int rotationCount);

public:
    Block(const vector<vector<int> > &fields, int rotationCount, int mirrorCount, int color);

    shared_ptr<Block> getTurnedBlock(int rotationCount, int mirrorCount);

    vector<int> findFirstOfBlock() const;

    vector<vector<int> > getFields();

    int getRotationCount() const;

    int getMirrorCount() const;

    int getRowCount() const;

    int getColCount() const;

    void print() const;
};
#endif
