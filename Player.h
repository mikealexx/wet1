#ifndef Player_h
#define Player_h

#include "Stats.h"
#include "Team.h"

class Player {
    private:
        int id;
        int teamId;
        shared_ptr<Team> team;
        int gamesPlayed;
        int goals;
        int cards;
        bool goalKeeper;
        shared_ptr<Player> pre;
        shared_ptr<Player> succ;
        

    public:
        Player() = delete;
        ~Player() = default;
        Player(int id, int teamId, shared_ptr<Team> team, int gamesPlayed, int goals, int cards, bool goalKeeper);
        Player(const Player& other) = default;
        Player& operator=(const Player& other) = default;

        int getId() const;
        int getTeamId() const;
        shared_ptr<Team> getTeam() const;
        int getGamesPlayed() const;
        int getGoals() const;
        int getCards() const;
        Stats* getStats() const;
        bool isGoalKeeper() const;
        shared_ptr<Player> getPre() const;
        shared_ptr<Player> getSucc() const;

        bool setId(const int id);
        bool setTeamId(const int teamId);
        bool setTeam(Team* myTeam);
        bool setGamesPlayed(const int gamesPlayed);
        bool setGoals(const int goals);
        bool setCards(const int cards);
        bool setGoalKeeper(const bool isGoalKeeper);
        bool setPre(shared_ptr<Player> player);
        bool setSucc(shared_ptr<Player> player);
};

#endif