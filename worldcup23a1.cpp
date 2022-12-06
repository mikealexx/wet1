#include "worldcup23a1.h"
//class Player;
//class Team;

world_cup_t::world_cup_t():
	topScorer(nullptr)
{
	this->teams = new AVLTree<Team, int>();
	this->kosherTeams = new AVLTree<Team, int>();
	this->playersById = new AVLTree<Player, int>();
	this->playersByStats = new AVLTree<Player, Stats>();
}

world_cup_t::~world_cup_t()
{
	delete(this->teams);
	delete(this->kosherTeams);
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
		Stats* stats = player->getStats();
		this->playersById->insert(player, playerId);
		this->playersByStats->insert(player, *stats);
		TreeNode<Player, Stats>* playerNode = this->playersByStats->findNode(*stats);
		playerNode->data->setPre(this->playersByStats->findPredecessor(*stats)->data);
		playerNode->data->setSucc(this->playersByStats->findSuccessor(*stats)->data);
		playerNode->data->getPre()->setSucc(playerNode->data);
		playerNode->data->getSucc()->setPre(playerNode->data);
		player->getTeam()->getPlayersById()->insert(player, playerId);
		player->getTeam()->getPlayersByStats()->insert(player, *stats);
		team->addTotalCards(player->getCards());
		team->addTotalGoals(player->getGoals());
		if(goals > player->getTeam()->getTopScorer()->getGoals()) { 
			player->getTeam()->setTopScorer(playerNode->data);
		}
	}
	catch (const std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	catch(const std::exception& e) {
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
		Stats* playerStats = player->getStats();
		if (team->getTopScorer() == player){
			team->setTopScorer(player->getPre());
		}
		player->getPre()->setSucc(player->getSucc());
		player->getSucc()->setPre(player->getPre());
		this->playersById->remove(playerId);
		this->playersByStats->remove(*playerStats);
		team->getPlayersById()->remove(player->getId());
		team->getPlayersByStats()->remove(*player->getStats());
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
		team->addTotalCards(cardsReceived); //remove player's cards from team's total cards count
		team->addTotalGoals(scoredGoals); //remove player's goals from team's total goals count
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
		if(team1->getPlayersNum() < 11 || team1->getGoalKeepers() < 1 || team2->getPlayersNum() < 11 || team2->getGoalKeepers() < 1) {
			return StatusType::FAILURE;
		}
		int team1GameScore = team1->getPoints() + team1->getTotalGoals() - team1->getTotalCards();
		int team2GameScore = team2->getPoints() + team2->getTotalGoals() - team2->getTotalCards();
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
		return output_t<int>(this->playersById->findNode(playerId)->data->getGamesPlayed());
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
		return output_t<int>(this->teams->findNode(teamId)->data->getPoints());
	}
	catch(const std::exception& e) {
		return output_t<int>(StatusType::FAILURE);
	}
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	if(teamId1 <= 0 || teamId2 <= 0 || newTeamId <= 0 || teamId1 == teamId2) {
		return StatusType::INVALID_INPUT;
	}
	try {
		if(newTeamId != teamId1 && newTeamId != teamId2){
			this->teams->findNode(newTeamId);
			return StatusType::FAILURE; //the key was found and therefore it's a failure
		}
	}
	catch(const AVLTree<Team, int>::NodeNotFound& e){} //catch KeyNotFound exception, and ignore (Ignore = Good)
	try {
		shared_ptr<Team> team1 = this->teams->findNode(teamId1)->data;
		shared_ptr<Team> team2 = this->teams->findNode(teamId2)->data;
		Team* newTeam = new Team(newTeamId, team1->getPoints() + team2->getPoints());
		newTeam->setPlayersNum(team1->getPlayersNum() + team2->getPlayersNum());
		newTeam->addGoalKeepers(team1->getGoalKeepers() + team2->getGoalKeepers());
		newTeam->addTotalGoals(team1->getTotalGoals() + team2->getTotalGoals());
		newTeam->addTotalCards(team1->getTotalCards() + team2->getTotalCards());
		if(team1->getTopScorer()->getGoals() > team2->getTopScorer()->getGoals()) {
			newTeam->setTopScorer(team1->getTopScorer());
		}
		else {
			newTeam->setTopScorer(team2->getTopScorer());
		}
		AVLTree<Player, int>::merge(*team1->getPlayersById(), *team2->getPlayersById(), *newTeam->getPlayersById()); //merge player tree by id
		AVLTree<Player, Stats>::merge(*team1->getPlayersByStats(), *team2->getPlayersByStats(), *newTeam->getPlayersByStats()); //merge players tree by stats
		this->teams->remove(teamId1);
		this->teams->remove(teamId2);
		this->teams->insert(newTeam, newTeamId);
	}
	catch(const std::bad_alloc& e) {
		return StatusType::ALLOCATION_ERROR;
	}
	catch(const std::exception& e) {
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	if(teamId == 0) {
		return output_t<int>(StatusType::INVALID_INPUT);
	}
	if(teamId < 0) {
		return output_t<int>(this->topScorer->getId());
	}
	try {
		shared_ptr<Team> team = this->teams->findNode(teamId)->data;
		return output_t<int>(team->getTopScorer()->getId());
	}
	catch(std::exception& e) {
		return output_t<int>(StatusType::FAILURE);
	}
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	if(teamId == 0) {
		return output_t<int>(StatusType::FAILURE);
	}
	if(teamId < 0) {
		return this->playersById->getSize();
	}
	try{
		return output_t<int>(this->teams->findNode(teamId)->data->getPlayersById()->getSize());
	}
	catch(std::exception& e){
		return output_t<int>(StatusType::FAILURE);
	}
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

static int treeToIdArray(TreeNode<Player, Stats>* root, int *const output, int i) {
	if(root == nullptr) {
		return i;
	}
	if(root->left != nullptr) {
		i = treeToIdArray(root->left, output, i);
	}
	output[i] = root->data->getId();
	i++;
	if(root->right != nullptr) {
		i = treeToIdArray(root->right, output, i);
	}
	return i;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	if(teamId == 0) {
		return StatusType::INVALID_INPUT;
	}
	try {
		AVLTree<Player, Stats>* tree;
		if(teamId < 0) {
			tree = this->playersByStats;
		}
		else {
			tree = this->teams->findNode(teamId)->data->getPlayersByStats();
		}
		if (tree->getSize() != 0 && output == nullptr){
			return StatusType::INVALID_INPUT;
		}
		treeToIdArray(tree->root, output, 0);
		return StatusType::SUCCESS;
	}
	catch(const std::exception& e) {
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	if (playerId <= 0 || teamId <= 0){
		return output_t<int>(StatusType::INVALID_INPUT);
	}
	if (this->playersById->getSize() == 1){ // Only one player in system
		return output_t<int>(StatusType::FAILURE);
	}
	try{
		shared_ptr<Player> playerNode = this->teams->findNode(teamId)->data->getPlayersById()->findNode(playerId)->data;
		Stats* playerStats = playerNode->getStats();
		Stats* preStats = playerNode->getPre()->getStats();
		Stats* succStats = playerNode->getSucc()->getStats();
		return output_t<int>(playerStats->getClosest(preStats, succStats));
	}
	catch(const std::exception& e){
		return output_t<int>(StatusType::FAILURE);
	}
	return 1006;
}

struct TeamSim {
	int teamId;
	int points;
};

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

