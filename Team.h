#ifndef Team_h
#define Team_h

#include "Player.h"
#include "AVLTree.h"
#include <memory>

using std::shared_ptr;

class Team {
    private:
        int id;
        int gamesPlayed;
        int points;
        int playersNum;
        int goalKeepers;
        int totalGoals;
        int totalCards;
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
        int getTotalCards() const;
        int getTotalGoals() const;
        int getGoalKeepers() const;
        void addGoalKeepers(int goalKeepers);
        void addTotalGoals(int goals);
        void addTotalCards(int cards);
        void addPoints(int points);
        void addGamesPlayed(int games);
        Player* getTopScorer() const;
        AVLTree<Player, int>* getPlayersById() const;
        AVLTree<Player, Stats>* getPlayersByStats() const;
};

#endif