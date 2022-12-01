#include "Team.h"

Team::Team(int id, int points):
    id(id),
    gamesPlayed(0),
    points(points),
    playersNum(0),
    topScorer(nullptr)
{
    playersById = new AVLTree<Player, int>();
    playersByStats = new AVLTree<Player, Stats>();
}

int Team::getID() const{
    return this->id;
}

int Team::getGamesPlayed() const{
    return this->gamesPlayed;
}

int Team::getPoints() const{
    return this->points;
}

int Team::getPlayersNum() const{
    return this->playersNum;
}

Player* Team::getTopScorer() const{
    return this->topScorer;
}

AVLTree<Player, int>* Team::getPlayersById() const{
    return this->playersById;
}

AVLTree<Player, Stats>* Team::getPlayersByStats() const{
    return this->playersByStats;
}