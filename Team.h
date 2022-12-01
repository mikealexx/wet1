#ifndef Team_h
#define Team_h

#include "Player.h"
#include "AVLTree.h"

class Team {
    private:
        int id;
        int gamesPlayed;
        int points;
        Player* topScorer;
        AVLTree<Player, int>* playersById;
        AVLTree<Player, Stats>* PlayersByStats;


    public:
        Team() = delete;
        Team(int id, int points);
        ~Team();

        int getID() const;
        int getGamesPlayed() const;
        int getPoints() const;
        Player* getTopScorer() const;

};

#endif