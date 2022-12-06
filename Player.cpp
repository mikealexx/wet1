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
    return this->gamesPlayed + this->team->getGamesPlayed();
}

int Player::getGoals() const {
    return this->goals;
}

int Player::getCards() const {
    return this->cards;
}

Stats* Player::getStats() const{
    return &Stats(this->getGoals(), this->getCards(), this->getId());
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

void Player::setId(const int id) {
    this->id = id;
}

void Player::setTeamId(const int teamId) {
    this->teamId = teamId;
}

void Player::setTeam(Team* myTeam) {
    this->team = shared_ptr<Team>(myTeam);
}

void Player::addGamesPlayed(const int gamesPlayed) {
    this->gamesPlayed += gamesPlayed;
}

void Player::addGoals(const int goals) {
    this->goals += goals;
}

void Player::addCards(const int cards) {
    this->cards += cards;
}

void Player::setGoalKeeper(const bool isGoalKeeper) {
    this->goalKeeper = goalKeeper;
}

void Player::setPre(shared_ptr<Player> player) {
    this->pre = player;
}

void Player::setSucc(shared_ptr<Player> player) {
    this->succ = player;
}

void Player::updateStats(int gamesPlayed, int scoredGoals, int cardsReceived) {
    this->gamesPlayed += gamesPlayed;
    this->goals += scoredGoals;
    this->cards += cardsReceived;
}