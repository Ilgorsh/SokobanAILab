#include <vector>
#include <set>
#include <queue>
#include "Global.h"
#include <algorithm>

using namespace std;

class State
{
private:
	
	bool is_deadlock(const int&, const int&);
public:
	vector<int> matrix_of_connections(const int &);
	vector<char> map_;
	State() {};
	State(const State& stt) :barrels(stt.barrels), PlayerPos(stt.PlayerPos),  prev_map(stt.prev_map), depth(stt.depth), map_(stt.map_) { prev_state = stt.prev_state; };
	State(const vector<int>& BRLS, const int& PP, State* prev_state_) : barrels(BRLS), PlayerPos(PP), prev_map(prev_state_->map_) { prev_state = prev_state_; depth = prev_state_->depth+1;};
	State(const vector<int>& BRLS, const int& PP, const vector<char> prev_map_) : barrels(BRLS), PlayerPos(PP), prev_map(prev_map_) { prev_state = nullptr; depth = 0; };
	vector<int> barrels;
	int PlayerPos;
	State* prev_state;
	vector<char> prev_map;
	int depth;
	bool is_solved(const vector<char>&);
	bool is_deadlock(const int&);
	vector<char> &genmap(const vector<char> &,const int&);
	vector<State> push(const int& );
	State &operator = (const State& state2) = default;
	friend bool operator < (const State& state1, const State& state2) {
		return(state1.depth < state2.depth);
	};
	friend bool operator == (const State& state1,const State& state2) {
		return(state1.map_==state2.map_);
	};
};

