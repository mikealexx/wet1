#ifndef Team_h
#define Team_h

#include "Player.h"

class Team {
    private:
        int playedGames;
        int points;
        Player* topScorer;

    public:
        Team();
        ~Team();
};

#endif