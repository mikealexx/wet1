#include "Player.h"

Player::Player(int id, int teamId, shared_ptr<Team> team, int gamesPlayed, int goals, int cards, bool goalKeeper):
    id(id), 
    teamId(teamId), 
    team(team),
    gamesPlayed(gamesPlayed),
    goals(goals), 
    cards(cards), 
    goalKeeper(goalKeeper),
    pre(nullptr),
    succ(nullptr)
{}

int Player::getId() const {
    return this->id;
}

int Player::getTeamId() const {
    return this->teamId;
}

shared_ptr<Team> Player::getTeam() const {
    return this->team;
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

Stats* Player::getStats() const{
    return new Stats(this->getGoals(), this-> getCards(), this->getId());
}

bool Player::isGoalKeeper() const {
    return this->goalKeeper;
}

shared_ptr<Player> Player::getPre() const{
    return this->pre;
}

shared_ptr<Player> Player::getSucc() const{
    return this->succ;
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
    this->team = shared_ptr<Team>(myTeam);
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

bool Player::setPre(shared_ptr<Player> player) {
    this->pre = player;
    return true;
}

bool Player::setSucc(shared_ptr<Player> player) {
    this->succ = player;
    return true;
}