#include "Board.h"
#include <iomanip>
#include <iostream>
#include <stack>
#include <fstream>

using namespace std;
ofstream outputFile;
static mutex solutionMutex;
static int solutions = 0;
static string filename = "C:/Users/bmegyeri/Desktop/Output/output" + to_string(solutions / 100000) + " (" + to_string(solutions) + " - " + to_string(solutions + 100000) + ")" + ".txt";


string getFileName() {
    return filename;
}

void Board::solveThreaded(ThreadPool &pool, const vector<shared_ptr<Block> > &blocks) {
    for (const shared_ptr<Block> &start: blocks) {
        pool.enqueue([this, start, blocks] {
            this->solveFromStartingBlock(start, blocks);
        });
    }
}

//Write real copy constructor
Board::Board(const shared_ptr<Board> &toCopy) {
    vector newVec(toCopy->fields.size(), vector<int>(toCopy->fields[0].size()));
    for (int row = 0; row < toCopy->fields.size(); ++row) {
        for (int col = 0; col < toCopy->fields[0].size(); ++col) {
            newVec[row][col] = toCopy->fields[row][col];
        }
    }
    this->fields = newVec;
}

Board::Board(int rows, int columns) {
    vector newVec(rows, vector<int>(columns));
    this->fields = newVec;
}

void Board::printBoard(ostream &ostream) const {
    for (int index = 0; index < this->fields.size(); index++) {
        for (int innerIndex = 0; innerIndex < this->fields[index].size(); innerIndex++) {
            ostream << setw(2) << this->fields[index][innerIndex] << " ";
        }
        ostream << endl;
    }
    ostream << endl;
}

vector<int> Board::findNextEmpty() const {
    vector pos = {0, 0};
    for (int index = 0; index < this->fields.size(); index++) {
        for (int innerIndex = 0; innerIndex < this->fields[0].size(); innerIndex++) {
            if (this->fields[index][innerIndex] == 0) {
                pos[0] = index;
                pos[1] = innerIndex;
                return pos;
            }
        }
    }
    return {-1, -1};
}

bool Board::isFilled() const {
    for (int index = 0; index < this->fields.size(); index++) {
        for (int innerIndex = 0; innerIndex < this->fields[index].size(); innerIndex++) {
            if (this->fields[index][innerIndex] == 0) {
                return false;
            }
        }
    }
    return true;
}

void Board::solveFromStartingBlock(const shared_ptr<Block> &startBlock, const vector<shared_ptr<Block> > &remainingBlocks) {
    struct State {
        std::shared_ptr<Board> board;
        vector<shared_ptr<Block> > blocks;
    };

    stack<State> stack;

    for (int rotation_count = 0; rotation_count < startBlock->getRotationCount(); ++rotation_count) {
        for (int mirror_count = 0; mirror_count <= startBlock->getMirrorCount(); ++mirror_count) {
            auto turned = startBlock->getTurnedBlock(rotation_count, mirror_count);
            auto startBoard = std::make_shared<Board>(*this);

            auto empty = startBoard->findNextEmpty();
            auto offset = turned->findFirstOfBlock();

            vector pos = {empty[0] - offset[0], empty[1] - offset[1]};

            if (!startBoard->placeBlock(startBoard, turned, pos)) {
                continue;
            }

            vector<shared_ptr<Block> > next;
            for (auto block: remainingBlocks) {
                if (block != startBlock) {
                    next.push_back(block);
                }
            }

            stack.emplace(startBoard, std::move(next));
        }
    }

    while (!stack.empty()) {
        auto state = stack.top();
        stack.pop();

        if (state.board->isFilled()) {
            scoped_lock lock(solutionMutex);
            ++solutions;
            if (outputFile.is_open()) {
                outputFile << solutions << ". Solution Found!" << endl;
                state.board->printBoard(outputFile);
            } else {
                outputFile.open(filename);
                outputFile << solutions << ". Solution Found!" << endl;
                state.board->printBoard(outputFile);
            }
            if (solutions % 100000 == 0) {
                outputFile.close();
                outputFile.open("C:/Users/bmegyeri/Desktop/Output/output" + to_string(solutions / 100000) + " (" + to_string(solutions) + " - " + to_string(solutions + 100000) + ")" + ".txt");
                filename = "C:/Users/bmegyeri/Desktop/Output/output" + to_string(solutions / 100000) + " (" + to_string(solutions) + " - " + to_string(solutions + 100000) + ")" + ".txt";
            }
            if (solutions % 1000 == 0) {
                cout << solutions << " Found!" << endl;
            }
            continue;
        }

        auto empty = state.board->findNextEmpty();

        for (int index = 0; index < state.blocks.size(); ++index) {
            shared_ptr<Block> block = state.blocks[index];

            for (int rotation_count = 0; rotation_count < block->getRotationCount(); ++rotation_count) {
                for (int mirror_count = 0; mirror_count <= block->getMirrorCount(); ++mirror_count) {
                    auto turned = block->getTurnedBlock(rotation_count, mirror_count);
                    auto nextBoard = std::make_shared<Board>(state.board);

                    auto offSet = turned->findFirstOfBlock();
                    vector itterationCoordinate = {empty[0] - offSet[0], empty[1] - offSet[1]};

                    if (!nextBoard->placeBlock(nextBoard, turned, itterationCoordinate)) {
                        continue;
                    }
                    vector<shared_ptr<Block> > nextBlocks;
                    for (int innerIndex = 0; innerIndex < state.blocks.size(); ++innerIndex) {
                        if (innerIndex != index) {
                            nextBlocks.push_back(state.blocks[innerIndex]);
                        }
                    }
                    stack.emplace(std::move(nextBoard), std::move(nextBlocks));
                }
            }
        }
    }
}

bool Board::placeBlock(const shared_ptr<Board> &boardCopy, const shared_ptr<Block> &block, const vector<int> &coordinates) const {
    bool validPlacement = true;
    for (int row = 0; row < block->getRowCount(); row++) {
        for (int col = 0; col < block->getColCount(); col++) {
            if (coordinates[0] + row < this->fields.size() && coordinates[1] + col < boardCopy->fields[row].size()) {
                if (this->fields[row + coordinates[0]][col + coordinates[1]] == 0) {
                    boardCopy->fields[row + coordinates[0]][col + coordinates[1]] = block->getFields()[row][col];
                } else {
                    if (block->getFields()[row][col] == 0) {
                        continue;
                    }
                    validPlacement = false;
                    return validPlacement;
                }
            } else {
                validPlacement = false;
                return validPlacement;
            }
        }
    }
    return validPlacement;
}
