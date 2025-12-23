#include "Block.h"
#include <iostream>
#include <memory>

Block::Block(const vector<vector<int> > &fields, int rotationCount, int mirrorCount, int color) {
    this->fields = fields;
    this->rotationCount = rotationCount;
    this->mirrorCount = mirrorCount;
    this->color = color;
}

vector<int> Block::findFirstOfBlock() const {
    vector pos = {0, 0};
    for (int index = this->fields.size() - 1; index >= 0; index--) {
        for (int innerIndex = this->fields[index].size() - 1; innerIndex >= 0; innerIndex--) {
            if (this->fields[index][innerIndex] != 0) {
                pos[0] = index;
                pos[1] = innerIndex;
            }
        }
    }
    return pos;
}

vector<vector<int> > Block::rotateFields(const int amount) {
    switch (amount) {
        case 0: {
            return this->getFields();
        }
        case 1: {
            vector output(this->fields[0].size(), vector<int>(this->fields.size()));
            for (int index = 0; index < this->fields.size(); index++) {
                for (int innerIndex = 0; innerIndex < this->fields[0].size(); innerIndex++) {
                    output[innerIndex][this->fields.size() - 1 - index] = this->fields[index][innerIndex];
                }
            }
            return output;
        }
        case 2: {
            vector output(this->fields.size(), vector<int>(this->fields[0].size()));
            for (int index = 0; index < this->fields.size(); index++) {
                for (int innerIndex = 0; innerIndex < this->fields[index].size(); innerIndex++) {
                    output[this->fields.size() - 1 - index][this->fields[index].size() - 1 - innerIndex] = this->fields[index][innerIndex];
                }
            }
            return output;
        }
        case 3: {
            vector output(this->fields[0].size(), vector<int>(this->fields.size()));
            for (int index = 0; index < this->fields.size(); index++) {
                for (int innerIndex = 0; innerIndex < this->fields[0].size(); innerIndex++) {
                    output[this->fields[0].size() - 1 - innerIndex][index] = this->fields[index][innerIndex];
                }
            }
            return output;
        }
        default: {
            cout << "Wrong rotation!" << endl;
            system("pause");
            abort();
        }
    }
}

vector<vector<int> > Block::mirrorFields(int rotationCount) {
    auto rotatedFields = this->rotateFields(rotationCount);
    vector output(rotatedFields.size(), vector(rotatedFields[0].size(), 0));
    for (int row = 0; row < rotatedFields.size(); row++) {
        for (int col = 0; col < rotatedFields[row].size(); col++) {
            output[row][col] = rotatedFields[row][rotatedFields[row].size() - col - 1];
        }
    }
    return output;
}

shared_ptr<Block> Block::getTurnedBlock(int rotationCount, int mirrorCount) {
    return mirrorCount > 0 ? std::make_shared<Block>(this->mirrorFields(rotationCount), this->rotationCount, this->mirrorCount, this->color) : std::make_shared<Block>(this->rotateFields(rotationCount), this->rotationCount, this->mirrorCount, this->color);
}

vector<vector<int> > Block::getFields() {
    return this->fields;
}

int Block::getRotationCount() const {
    return rotationCount;
}

int Block::getMirrorCount() const {
    return this->mirrorCount;
}

int Block::getRowCount() const {
    return this->fields.size();
}

int Block::getColCount() const {
    return this->fields[0].size();
}

void Block::print() const {
    for (int row = 0; row < this->fields.size(); row++) {
        for (int col = 0; col < this->fields[row].size(); col++) {
            if (this->fields[row][col] != 0) {
                cout << this->fields[row][col];
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}
