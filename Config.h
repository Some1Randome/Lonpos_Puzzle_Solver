#ifndef CONFIG_H
#define CONFIG_H

#include <memory>
#include <vector>
#include <mutex>
#include "Block.h"

using namespace std;


class Config {
    Config() {
    };
    inline static Config *instance = nullptr;
    static mutex mtx;
    int boardWidth{};
    int boardHeight{};
    vector<shared_ptr<Block> > blocks;

public:
    void readJson();

    static Config *getInstance();

    Config(const Config &obj) = delete;

    int getBoardWidth() const;

    int getBoardHeight() const;

    vector<shared_ptr<Block> > getBlocks() const;
};

#endif
