#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;

mutex Config::mtx;

void Config::readJson() {
    ifstream file("config.json");

    nlohmann::json data;
    file >> data;

    boardWidth = data["boardWidth"];
    boardHeight = data["boardHeight"];

    for (const auto &jsonData: data["Blocks"]) {
        auto block = std::make_shared<Block>(
            jsonData["fields"].get<vector<vector<int> > >(),
            jsonData["rotationCount"],
            jsonData["mirrorCount"],
            jsonData["color"]);
        blocks.push_back(block);
    }
    file.close();
}

Config *Config::getInstance() {
    if (instance == nullptr) {
        mtx.lock();
        // ReSharper disable once CppDFAConstantConditions
        if (instance == nullptr) {
            instance = new Config();
            instance->readJson();
        }
        mtx.unlock();
    }
    return instance;
}

int Config::getBoardWidth() const {
    return boardWidth;
}

int Config::getBoardHeight() const {
    return boardHeight;
}

vector<std::shared_ptr<Block> > Config::getBlocks() const {
    return blocks;
}
