#include "Team.h"

Team::Team(int id, int points):
    id(id),
    gamesPlayed(0),
    points(points),
    topScorer(nullptr)
{
    playersById = new AVLTree<Player, int>();
    PlayersByStats = new AVLTree<Player, Stats>();
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

Player* Team::getTopScorer() const{
    return this->topScorer;
}