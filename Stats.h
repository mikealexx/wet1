
// A Class for comparing players by the rules of get_closest_player.
class Stats{
public:
int goals;
int cards;
int playerID;

Stats(int goals, int cards, int playerID);

friend bool operator<(const Stats& l, const Stats& r){
    if (l.goals < r.goals){
        return true;
    }
    else if (l.goals == r.goals){
        if (l.cards < r.cards){
            return true;
        }
        else if (l.cards == r.cards){
            if (l.playerID < r.playerID){
                return true;
            }
        }
    }
    return false;
}

friend bool operator>(const Stats& l, const Stats& r){
    return (r < l);
}

};