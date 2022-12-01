#include "Player.h"

Player::Player(int id, int teamId, Team* myTeam, int gamesPlayed, int goals, int cards, bool goalKeeper):
    id(id), teamId(teamId), gamesPlayed(gamesPlayed), goals(goals), cards(cards), goalKeeper(goalKeeper)
{
    this->myTeam = shared_ptr<Team>(myTeam);
}

int Player::getId() const {
    return this->id;
}

int Player::getTeamId() const {
    return this->teamId;
}

shared_ptr<Team> Player::getTeam() const {
    return this->myTeam;
}

int Player::getGamesPlayed() const {
    return this->gamesPlayed;
}

int Player::getGoals() const {
    return this->goals;
}

int Player::getCards() const {
    return this->cards;
}

bool Player::isGoalKeeper() const {
    return this->goalKeeper;
}

bool Player::setId(const int id) {
    if(id <= 0) {
        return false;
    }
    this->id = id;
    return true;
}

bool Player::setTeamId(const int teamId) {
    if(teamId <= 0) {
        return false;
    }
    this->teamId = teamId;
    return true;
}

bool Player::setTeam(Team* myTeam) {
    if(myTeam == nullptr) {
        return false;
    }
    this->myTeam = shared_ptr<Team>(myTeam);
    return true;
}

bool Player::setGamesPlayed(const int gamesPlayed) {
    if(gamesPlayed < 0) {
        return false;
    }
    this->gamesPlayed = gamesPlayed;
    return true;
}

bool Player::setGoals(const int goals) {
    if(goals < 0) {
        return false;
    }
    this->goals = goals;
    return true;
}

bool Player::setCards(const int cards) {
    if(cards < 0) {
        return false;
    }
    this->cards = cards;
    return false;
}

bool Player::setGoalKeeper(const bool isGoalKeeper) {
    this->goalKeeper = goalKeeper;
    return true;
}