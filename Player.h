#ifndef Player_h
#define Player_h

#include "Stats.h"
#include "Team.h"

class Player {
    private:
        int id;
        int teamId;
        shared_ptr<Team> myTeam;
        int gamesPlayed;
        int goals;
        int cards;
        bool goalKeeper;

    public:
        Player() = delete;
        ~Player() = default;
        Player(int id, int teamId, Team* myTeam, int gamesPlayed, int goals, int cards, bool goalKeeper);
        Player(const Player& other) = default;
        Player& operator=(const Player& other) = default;

        int getId() const;
        int getTeamId() const;
        shared_ptr<Team> getTeam() const;
        int getGamesPlayed() const;
        int getGoals() const;
        int getCards() const;
        bool isGoalKeeper() const;

        bool setId(const int id);
        bool setTeamId(const int teamId);
        bool setTeam(Team* myTeam);
        bool setGamesPlayed(const int gamesPlayed);
        bool setGoals(const int goals);
        bool setCards(const int cards);
        bool setGoalKeeper(const bool isGoalKeeper);
};

#endif