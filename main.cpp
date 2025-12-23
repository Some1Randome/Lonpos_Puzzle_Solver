#include "Board.h"
#include "Config.h"
#include "ThreadPool.h"

using namespace std;

int main() {
    Board board(
        Config::getInstance()->getBoardHeight(),
        Config::getInstance()->getBoardWidth()
    );

    ThreadPool pool(12);
    board.solveThreaded(pool, Config::getInstance()->getBlocks());
}
