#include "worldcup23a1.h"
#include "Team.h"
#include "Player.h"

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
	delete this->playersById;
	delete this->playersByStats;
	delete this->kosherTeams;
	delete this->teams;
}


StatusType world_cup_t::add_team(int teamId, int points)
{
	if(teamId <= 0 || points < 0) {
		return StatusType::INVALID_INPUT;
	}
	shared_ptr<Team> team = shared_ptr<Team>(new Team(teamId, points));
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
		shared_ptr<Player> player = shared_ptr<Player>(new Player(playerId, teamId, team, gamesPlayed - team->getGamesPlayed(), goals, cards, goalKeeper));
		
		bool isKosher = team->isKosher();
		Stats stats = player->getStats();
		this->playersById->insert(player, playerId);
		this->playersByStats->insert(player, stats);
		
		TreeNode<Player, Stats>* pred = this->playersByStats->findPredecessor(stats);
		TreeNode<Player, Stats>* succ = this->playersByStats->findSuccessor(stats);
		if(pred != nullptr) {
			player->setPre(pred->data);
			pred->data->setSucc(player);
		}
		else {
			player->setPre(nullptr);
		}
		if(succ != nullptr) {
			player->setSucc(succ->data);
			succ->data->setPre(player);
		}
		else {
			player->setSucc(nullptr);
		}
		
		team->getPlayersById()->insert(player, playerId);
		team->getPlayersByStats()->insert(player, stats);
		team->addTotalCards(cards);
		team->addTotalGoals(goals);
		team->addPlayersNum(1);
		if(goalKeeper) {
			team->addGoalKeepers(1);
		}
		
		if (!isKosher && team->isKosher()){ // Team was not kosher and now is - add to kosher tree
			this->kosherTeams->insert(team, teamId);
			TreeNode<Team, int>* teamPre = this->kosherTeams->findPredecessor(teamId);
			TreeNode<Team, int>* teamSucc = this->kosherTeams->findSuccessor(teamId);
			if(teamPre != nullptr){
				teamPre->data->setNextKosher(team);
			}
			if(teamSucc != nullptr){
				team->setNextKosher(teamSucc->data);
			}
		}
		if(team->getTopScorer() == nullptr || stats > team->getTopScorer()->getStats()) { 
			team->setTopScorer(player);
		}
		if(this->topScorer == nullptr || stats > this->topScorer->getStats()) {
			this->topScorer = player;
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
		bool isKosher = team->isKosher();
		Stats playerStats = player->getStats();
		if (this->topScorer == player){
			this->topScorer = player->getPre();
		}
		if(team->getTopScorer() == player) {
			TreeNode<Player, Stats>* pred = team->getPlayersByStats()->findPredecessor(player->getStats());
			if(pred != nullptr) {
				team->setTopScorer(pred->data);
			}
			else {
				team->setTopScorer(nullptr);
			}
		}
		if (player->getPre() != nullptr)
			player->getPre()->setSucc(player->getSucc());
		if (player->getSucc() != nullptr)
			player->getSucc()->setPre(player->getPre());
		this->playersById->remove(playerId);
		this->playersByStats->remove(playerStats);
		team->getPlayersById()->remove(playerId);
		team->getPlayersByStats()->remove(playerStats);
		if(player->isGoalKeeper()) {
			team->addGoalKeepers(-1);
		}
		team->addTotalCards(-(player->getCards())); //add player's cards to team's total cards count
		team->addTotalGoals(-(player->getGoals())); //add player's goals to team's total goals count
		team->addPlayersNum(-1);
		if(isKosher && !team->isKosher()) { // If was kosher and now not - remove from kosher tree
			TreeNode<Team, int>* teamPre = this->kosherTeams->findPredecessor(team->getID());
			if(teamPre != nullptr){
				teamPre->data->setNextKosher(team->getNextKosher());
			}
			this->kosherTeams->remove(team->getID());
			team->setNextKosher(nullptr);
		}
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
		Stats stats = player->getStats();

		this->playersByStats->remove(stats);
		team->getPlayersByStats()->remove(stats);

		player->updateStats(gamesPlayed, scoredGoals, cardsReceived);
		Stats newStats = player->getStats();
		
		team->addTotalCards(cardsReceived); //add player's cards to team's total cards count
		team->addTotalGoals(scoredGoals); //add player's goals to team's total goals count

		if(team->getTopScorer() == nullptr || newStats > team->getTopScorer()->getStats()) { 
			team->setTopScorer(player);
		}
		if(this->topScorer == nullptr || newStats > this->topScorer->getStats()) {
			this->topScorer = player;
		}

		team->getPlayersByStats()->insert(player, newStats);
		this->playersByStats->insert(player, newStats);

		shared_ptr<Player> pred = player->getPre();
		shared_ptr<Player> succ = player->getSucc();
		TreeNode<Player, Stats>* newPredNode = this->playersByStats->findPredecessor(newStats);
		TreeNode<Player, Stats>* newSuccNode = this->playersByStats->findSuccessor(newStats);
		shared_ptr<Player> newPred = nullptr;
		shared_ptr<Player> newSucc = nullptr; 
		if(newPredNode != nullptr){
			newPred = newPredNode->data;
		}
		if(newSuccNode != nullptr){
			newSucc = newSuccNode->data;
		}

		if (pred != newPred){
			if (pred == nullptr)
				succ->setPre(nullptr);
			else if(succ == nullptr)
				pred->setPre(nullptr);
			else{
				pred->setSucc(succ);
				succ->setPre(pred);
			}
			player->setPre(newPred);
			player->setSucc(newSucc);
			if(newPred != nullptr)
				newPred->setSucc(player);
			if(newSucc != nullptr)
				newSucc->setPre(player);
		}
	}
	catch(const std::exception& e) {
		return StatusType::FAILURE;
	}
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
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
		shared_ptr<Team> newTeam = shared_ptr<Team>(new Team(newTeamId, team1->getPoints() + team2->getPoints()));
		newTeam->setPlayersNum(team1->getPlayersNum() + team2->getPlayersNum());
		newTeam->addGoalKeepers(team1->getGoalKeepers() + team2->getGoalKeepers());
		newTeam->addTotalGoals(team1->getTotalGoals() + team2->getTotalGoals());
		newTeam->addTotalCards(team1->getTotalCards() + team2->getTotalCards());
		if(team1->getTopScorer() == nullptr){
			newTeam->setTopScorer(team2->getTopScorer());
		}
		else if(team2->getTopScorer() == nullptr){
			newTeam->setTopScorer(team2->getTopScorer());
		}
		else if(team1->getTopScorer()->getGoals() > team2->getTopScorer()->getGoals()) {
			newTeam->setTopScorer(team1->getTopScorer());
		}
		else {
			newTeam->setTopScorer(team2->getTopScorer());
		}
		int arr1_size = team1->getPlayersById()->getSize();
        int arr2_size = team2->getPlayersById()->getSize();
        
        TreeNode<Player, int>** arr1 = new TreeNode<Player, int>* [arr1_size]; //first tree sorted array
        TreeNode<Player, int>** arr2 = new TreeNode<Player, int>* [arr2_size]; //second tree sorted array
        
        team1->getPlayersById()->treeToArray(arr1, team1->getPlayersById()->root, 0);
        team1->getPlayersById()->treeToArray(arr2, team2->getPlayersById()->root, 0);
		shared_ptr<Player> currPlayer;
		for (int i=0; i<arr1_size; i++){
			currPlayer = arr1[i]->data;
			currPlayer->addGamesPlayed(currPlayer->getGamesPlayed() - currPlayer->gamesWithoutTeam());
			currPlayer->setTeam(newTeam);
		}
		for (int i=0; i<arr2_size; i++){
			currPlayer = arr2[i]->data;
			currPlayer->addGamesPlayed(currPlayer->getGamesPlayed() - currPlayer->gamesWithoutTeam());
			currPlayer->setTeam(newTeam);
		}
		AVLTree<Player, int>::merge(*team1->getPlayersById(), *team2->getPlayersById(), *newTeam->getPlayersById()); //merge player tree by id
		AVLTree<Player, Stats>::merge(*team1->getPlayersByStats(), *team2->getPlayersByStats(), *newTeam->getPlayersByStats()); //merge players tree by stats
		this->teams->remove(teamId1);
		if(team1->isKosher()){
			this->kosherTeams->remove(teamId1);
		}
		this->teams->remove(teamId2);
		if(team2->isKosher()){
			this->kosherTeams->remove(teamId2);
		}
		this->teams->insert(newTeam, newTeamId);
		if (newTeam->isKosher()){
			this->kosherTeams->insert(newTeam, newTeamId);
		}
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
		shared_ptr<Player> pre = playerNode->getPre();
		shared_ptr<Player> succ = playerNode->getSucc();
		Stats playerStats = playerNode->getStats();
		int closest;
		if(pre == nullptr && succ == nullptr){
			return output_t<int>(StatusType::FAILURE);
		}
		else if(pre == nullptr) {
			closest = succ->getId();
		}
		else if(succ == nullptr) {
			closest = pre->getId();
		}
		else{
			Stats preStats = pre->getStats();
			Stats succStats = succ->getStats();
			closest = playerStats.getClosest(&preStats, &succStats);
		}
		return output_t<int>(closest);
	}
	catch(const std::exception& e){
		return output_t<int>(StatusType::FAILURE);
	}
	return 1006;
}

struct TeamSim {
	int teamId;
	int points;
	struct TeamSim* next;
	//struct TeamSim* prev;
};


/*static int getCount(TreeNode<Team, int>* root, int low, int high)
{
    if (root == nullptr) {
		return 0;
	}

    if (root->key == high && root->key == low){
		return 1;
	}
 
    // If current node is in range, then include it in count and
    // recur for left and right children of it
    if (root->key <= high && root->key>= low)
         return 1 + getCount(root->left, low, high) +
                    getCount(root->right, low, high);
 
    // If current node is smaller than low, then do for right
    else if (root->key < low)
         return getCount(root->right, low, high);
 
    // Else do for left child
    else return getCount(root->left, low, high);
}*/

/*static int countKosherTeamsId(TreeNode<Team, int>* node, int minId, int maxId, int count) {
	if(node == nullptr || (node != nullptr && node->left != nullptr && node->left->key < minId) || (node != nullptr && node->right != nullptr && node->right->key > maxId)) {
		return count;
	}
	if(node->left != nullptr && node->left->key <= maxId) {
		count = countKosherTeamsId(node->left, minId, maxId, count);
	}
	if(node->key <= maxId && node->key >= minId) {
		count++;
	}
	if(node->right != nullptr) {
		count = countKosherTeamsId(node->right, minId, maxId, count);
	}
	return count;
}*/

/*static int fillArrayKosher(TreeNode<Team, int>* node, TeamSim* arr, int i, int minId, int maxId) {
	if(node == nullptr || (node != nullptr && node->left != nullptr && node->left->key < minId) || (node != nullptr && node->right != nullptr && node->right->key > maxId)) {
		return i;
	}
	if(node->left != nullptr) {
		i = fillArrayKosher(node->left, arr, i, minId, maxId);
	}
	if(node->key <= maxId && node->key >= minId) {
		arr[i].teamId = node->data->getID();
		arr[i].points = node->data->getPoints() + node->data->getTotalGoals() - node->data->getTotalCards();
		i++;
	}
	if(node->right != nullptr) {
		i = fillArrayKosher(node->right, arr, i, minId, maxId);
	}
	return i;
}*/

static TreeNode<Team, int>* findMinInRange(TreeNode<Team, int>* root, int low, int high){
	TreeNode<Team, int>* curr = root;
	while(curr != nullptr && (curr->key < low || curr->key > high)){ //Get to range
		if(curr->key < low){
			curr = curr->right;
		}
		else {
			curr = curr->left;
		}
	}

	while(curr != nullptr && curr->key >= low){ //Find smallest in range
		if(curr->left == nullptr || curr->left-> key < low){
			return curr;
		}
		curr = curr->left;
	}

	return nullptr;
}

static void playGames(TeamSim* teams){
	TeamSim* curr = teams;
	while(curr->next != nullptr && curr->next->next != nullptr){
		if (curr->next->points > curr->next->next->points){
			curr->next->points += (3 + curr->next->next->points);
			curr->next->next = curr->next->next->next;
		}
		else if (curr->next->points < curr->next->next->points){
			curr->next->next->points += (3 + curr->next->points);
			curr->next = curr->next->next;
		}
		else if (curr->next->teamId > curr->next->next->teamId){
			curr->next->points += (3 + curr->next->next->points);
			curr->next->next = curr->next->next->next;
		}
		else{
			curr->next->next->points += (3 + curr->next->points);
			curr->next = curr->next->next;
		}
		curr = curr->next;
	}
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId){
	if (minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId){
		return output_t<int>(StatusType::INVALID_INPUT);
	}
	
	/*int size = getCount(this->kosherTeams->root, minTeamId, maxTeamId);//countKosherTeamsId(this->kosherTeams->root, minTeamId, maxTeamId, 0);
	if(size == 0){
		return output_t<int>(StatusType::FAILURE);
	}*/
	TreeNode<Team, int>* minTeamNode = findMinInRange(this->kosherTeams->root, minTeamId, maxTeamId);
	if(minTeamNode == nullptr){
		return output_t<int>(StatusType::FAILURE);
	}
	TeamSim* teams = new TeamSim;
	teams->teamId = -1;
	teams->points = -1;
	shared_ptr<Team> curr = minTeamNode->data;
	TeamSim* currTeamSim = teams;
	while(curr != nullptr && curr->getID() <= maxTeamId){
		TeamSim* newTeam = new TeamSim;
		newTeam->teamId = curr->getID();
		newTeam->points = curr->getPoints();
		newTeam->next = nullptr;
		currTeamSim->next = newTeam;
		currTeamSim = currTeamSim->next;
		curr = curr->getNextKosher();
	}

	/*TeamSim* arr = new TeamSim[size];
	TeamSim* teams = new TeamSim;
	teams->teamId = -1;
	teams->points = -1;
	teams->next = nullptr;
	//fillArrayKosher(this->kosherTeams->root, arr, 0, minTeamId, maxTeamId);
	TeamSim* curr = teams;
	for(int i=0; i<size; i++) {
		TeamSim* newTeam = new TeamSim;
		newTeam->teamId = arr[i].teamId;
		newTeam->points = arr[i].points;
		newTeam->next = nullptr;
		curr->next = newTeam;
		curr = curr->next;
	}*/

	while(teams->next->next != nullptr){
		playGames(teams);
	}
	return output_t<int>(teams->next->teamId);
	
}