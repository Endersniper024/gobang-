#include "Game.h"
#include <fstream>
#include <iostream>

Game::Game(Player* player, AI* ai, Board* board) {
    this->player = player;
    this->ai = ai;
    this->board = board;
    skipAiTurn = false; // 初始化标志为 false
}

void Game::saveGame(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
        return;
    }

    board->saveToFile(outFile);
    player->savePlayerState(outFile);
    ai->saveGameState(outFile);

    // 保存游戏状态标志
    outFile.write(reinterpret_cast<const char*>(&board->rst), sizeof(board->rst));
    outFile.write(reinterpret_cast<const char*>(&board->sv), sizeof(board->sv));
    outFile.write(reinterpret_cast<const char*>(&board->ld), sizeof(board->ld));

    outFile.write(reinterpret_cast<const char*>(&skipAiTurn), sizeof(skipAiTurn));

    outFile.close();
}

bool Game::loadGame(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
        return false;
    }

    board->loadFromFile(inFile);
    player->loadPlayerState(inFile);
    ai->loadGameState(inFile);

    // 恢复游戏状态标志
    inFile.read(reinterpret_cast<char*>(&board->rst), sizeof(board->rst));
    inFile.read(reinterpret_cast<char*>(&board->sv), sizeof(board->sv));
    inFile.read(reinterpret_cast<char*>(&board->ld), sizeof(board->ld));

    inFile.read(reinterpret_cast<char*>(&skipAiTurn), sizeof(skipAiTurn));

    inFile.close();
    return true;
}

void Game::play(bool resetBoard) {
    board->init(resetBoard);
    player->init(board);
    ai->init(board, resetBoard);

    while (true) {
        // 玩家走棋
        player->go();
        if (board->sv) {
            board->sv = false;
            saveGame("save.txt");
            continue;
        }
        if (board->ld) {
            board->ld = false;
            board->init(true);
            player->init(board);
            ai->init(board, true);
            loadGame("save.txt");
            continue;
        }
        if (board->checkOver()) {
            board->rst = false;
            board->init(true);
            continue;
        }

        // AI 走棋
        if (!skipAiTurn) {
            ai->go();
            if (board->checkOver()) {
                board->rst = false;
                board->init(true);
                continue;
            }
        }
        else {
            skipAiTurn = false; // 重置标志
        }
    }
}