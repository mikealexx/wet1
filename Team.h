#ifndef Team_h
#define Team_h

#include "Player.h"
#include "AVLTree.h"

class Team {
    private:
        int id;
        int gamesPlayed;
        int points;
        int playersNum;
        Player* topScorer;
        AVLTree<Player, int>* playersById;
        AVLTree<Player, Stats>* playersByStats;


    public:
        Team() = delete;
        Team(int id, int points);
        ~Team();

        int getID() const;
        int getGamesPlayed() const;
        int getPoints() const;
        int getPlayersNum() const;
        Player* getTopScorer() const;
        AVLTree<Player, int>* getPlayersById() const;
        AVLTree<Player, Stats>* getPlayersByStats() const;
};

#endif