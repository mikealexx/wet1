#include "worldcup23a1.h"

world_cup_t::world_cup_t()
{
	this->teams = new AVLTree<Team, int>();
	this->playersById = new AVLTree<Player, int>();
	this->playersByStats = new AVLTree<Player, Stats>();
}

world_cup_t::~world_cup_t()
{
	delete(this->teams);
	delete(this->playersById);
	delete(this->playersByStats);
}


StatusType world_cup_t::add_team(int teamId, int points)
{
	if(teamId <= 0 || points < 0) {
		return StatusType::INVALID_INPUT;
	}
	Team* team = new Team(teamId, points);
	try{
		this->teams->insert(team, teamId);
	}
	catch(const AVLTree<Team, int>::KeyAlreadyExists& e){
		return StatusType::FAILURE;
	}
	
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
	if (teamId <= 0){
		return StatusType::INVALID_INPUT;
	}
	try{
		TreeNode<Team, int>* teamNode = this->teams->findNode(teamId);
		if (teamNode->data->getPlayersNum() != 0){
			return StatusType::FAILURE;
		}
		this->teams->remove(teamId);
	}
	catch(const AVLTree<Team, int>::NodeNotFound& e){
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	if (playerId <= 0 || teamId <= 0 || gamesPlayed < 0 || goals < 0 || cards < 0 || 
	(gamesPlayed == 0 && (goals > 0 || cards > 0))){
		return StatusType::INVALID_INPUT;
	}
	try{
		shared_ptr<Team> team = this->teams->findNode(teamId)->data;
		Player* player = new Player(playerId, teamId, team, gamesPlayed, goals, cards, goalKeeper);
		if(goalKeeper) {
			team->addGoalKeepers(1);
		}
		Stats stats = *player->getStats();
		this->playersById->insert(player, playerId);
		this->playersByStats->insert(player, stats);
		TreeNode<Player, Stats>* playerNode = this->playersByStats->findNode(stats);
		playerNode->data->setPre(this->playersByStats->findPredecessor(stats)->data);
		playerNode->data->setSucc(this->playersByStats->findSuccessor(stats)->data);
		player->getTeam()->getPlayersById()->insert(player, playerId);
		player->getTeam()->getPlayersByStats()->insert(player, stats);
		team->addTotalCards(player->getCards());
		team->addTotalGoals(player->getGoals());
	}
	catch (const std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	catch(const std::exception& e){
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	if(playerId <= 0) {
		return StatusType::INVALID_INPUT;
	}
	try {
		shared_ptr<Player> player = this->playersById->findNode(playerId)->data;
		shared_ptr<Team> team = player->getTeam();
		int teamId = player->getTeamId();
		Stats* playerStats = player->getStats();
		this->playersById->remove(playerId);
		this->playersByStats->remove(*playerStats);
		if(player->isGoalKeeper()) {
			team->addGoalKeepers(-1);
		}
		team->addTotalCards(-(player->getCards())); //add player's cards to team's total cards count
		team->addTotalGoals(-(player->getGoals())); //add player's goals to team's total goals count
	}
	catch(const std::exception& e) {
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	if(playerId <= 0 || gamesPlayed < 0 || scoredGoals < 0 || cardsReceived < 0) {
		return StatusType::INVALID_INPUT;
	}
	try {
		shared_ptr<Player> player = this->playersById->findNode(playerId)->data;
		shared_ptr<Team> team = player->getTeam();
		player->updateStats(gamesPlayed, scoredGoals, cardsReceived);
		team->addTotalCards(player->getCards()); //remove player's cards from team's total cards count
		team->addTotalGoals(player->getGoals()); //remove player's goals from team's total goals count
	}
	catch(const std::exception& e) {
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <= 0) {
		return StatusType::INVALID_INPUT;
	}
	try {
		shared_ptr<Team> team1 = this->teams->findNode(teamId1)->data;
		shared_ptr<Team> team2 = this->teams->findNode(teamId2)->data;
		if(team1->getPlayersNum() < 12 || team1->getGoalKeepers() < 1 || team2->getPlayersNum() < 12 || team2->getGoalKeepers()) {
			return StatusType::FAILURE;
		}
		int team1GameScore = team1->getPoints() + (team1->getTotalGoals() - team1->getTotalGoals());
		int team2GameScore = team2->getPoints() + (team2->getTotalGoals() - team2->getTotalGoals());
		if(team1GameScore > team2GameScore) { //team1 wins
			team1->addPoints(3);
		}
		else if(team1GameScore < team2GameScore){ //team2 wins
			team2->addPoints(3);
		}
		else { //tie
			team1->addPoints(1);
			team2->addPoints(1);
		}
		team1->addGamesPlayed(1);
		team2->addGamesPlayed(1);
	}
	catch(const std::exception& e) {
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	if(playerId <= 0) {
		return output_t<int>(StatusType::INVALID_INPUT);
	}
	try {
		shared_ptr<Player> player = this->playersById->findNode(playerId)->data;
		return output_t<int>(player->getGamesPlayed());
	}
	catch(const std::exception& e) {
		return output_t<int>(StatusType::FAILURE);
	}
	return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	if(teamId <= 0) {
		return output_t<int>(StatusType::INVALID_INPUT);
	}
	try {
		shared_ptr<Team> team = this->teams->findNode(teamId)->data;
		return output_t<int>(team->getPoints());
	}
	catch(const std::exception& e) {
		return output_t<int>(StatusType::FAILURE);
	}
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	if(teamId1 <= 0 || teamId2 <= 0 || newTeamId <= 0) {
		return StatusType::INVALID_INPUT;
	}
	try {
		Team* team = new Team(teamId, points);

	}
	catch(const std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	catch(const std::exception& e) [
		return StatusType::FAILURE;
	]
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

