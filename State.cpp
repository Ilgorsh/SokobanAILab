#include "State.h"
#include <iostream>

bool State::is_solved(const vector<char> &map) {
	vector<int> goals;
	for (int i = 0; i != map.size();i++) {
		if (map[i] == GOAL || map[i] == PLR_ON_GOAL || map[i] == BOX_ON_GOAL) goals.push_back(i);
	}
	sort(goals.begin(), goals.end());
	sort(barrels.begin(), barrels.end());
	if (goals == barrels)return true;
	else return false;
}

vector<State> State::push(const int &n) {
	set<int> reachables;
	set<int> visited;
	vector<State> result;
	queue<int> points;
	vector<pair<int,int>> pushes;
	reachables.insert(this->PlayerPos);
	points.push(this->PlayerPos);
	visited.insert(PlayerPos);
	while (!points.empty()) {
		int index = points.front();
		if (index > n && index + n < map_.size()) {
			if ((map_[index + 1] == ' ' || map_[index + 1] == GOAL) && visited.find(index + 1)==visited.end()) { 
				visited.insert(index + 1);
				points.push(index + 1); 
				reachables.insert(index + 1);
			} else visited.insert(index + 1);
			if ((map_[index - 1] == ' ' || map_[index - 1] == GOAL) && visited.find(index - 1) == visited.end()) {
				points.push(index - 1);
				reachables.insert(index - 1);
				visited.insert(index - 1);
			} else visited.insert(index - 1);
			if ((map_[index - n] == ' ' || map_[index - n] == GOAL) && visited.find(index - n) == visited.end()) {
				points.push(index - n); 

				reachables.insert(index - n);
				visited.insert(index - n);
			} else visited.insert(index - n);
			if ((map_[index + n] == ' ' || map_[index + n] == GOAL) && visited.find(index + n) == visited.end()) {
				points.push(index + n);
				reachables.insert(index + n);
				visited.insert(index + n);
			} else visited.insert(index + n);
		}
		points.pop();
	}
	/*
	* 0 = Right
	* 1 = Left
	* 2 = Down
	* 3 = Up
	*/
	for (int i = 0; i != barrels.size(); i++) {
		if (reachables.find(barrels[i] - 1) != reachables.end() && map_[barrels[i] + 1] != WALL)pushes.push_back(pair<int, int>(0, barrels[i]));
		if (reachables.find(barrels[i] + 1) != reachables.end() && map_[barrels[i] - 1] != WALL)pushes.push_back(pair<int, int>(1, barrels[i]));
		if (reachables.find(barrels[i] - n) != reachables.end() && map_[barrels[i] + n] != WALL)pushes.push_back(pair<int, int>(2, barrels[i]));
		if (reachables.find(barrels[i] + n) != reachables.end() && map_[barrels[i] - n] != WALL)pushes.push_back(pair<int, int>(3, barrels[i]));
	}
	for (auto iter = pushes.begin(); iter != pushes.end(); iter++) {
		switch (iter->first) {
		case 0: {
			int j = 0;
			set<int> affected_boxes;
			while (map_[iter->second + j] == BOX || map_[iter->second + j] == BOX_ON_GOAL) {
				affected_boxes.insert(iter->second+j);
				j++;
			}
			if (map_[iter->second + j] != WALL) {
				vector<int> new_box_pos = barrels;
				int newPP = iter->second;
				for (int iter_ = 0; iter_ != new_box_pos.size(); iter_++) {
					if (affected_boxes.find(new_box_pos[iter_]) != affected_boxes.end()) {
						new_box_pos.at(iter_)++;
					}
				}
				result.push_back(State(new_box_pos,newPP,this));
				
			}
			break;
		}
		case 1: {
			int j = 0;
			set<int> affected_boxes;
			while (map_[iter->second - j] == BOX || map_[iter->second - j] == BOX_ON_GOAL) {
				affected_boxes.insert(iter->second - j);
				j++;
			}
			if (map_[iter->second - j] != WALL) {
				vector<int> new_box_pos = barrels;
				int newPP = iter->second;
				for (int iter_ = 0; iter_ != new_box_pos.size(); iter_++) {
					if (affected_boxes.find(new_box_pos[iter_]) != affected_boxes.end()) {
						new_box_pos.at(iter_)--;
					}
				}
				result.push_back(State(new_box_pos, newPP, this));
			}
			break;
		}
		case 2: {
			int j = 0;
			set<int> affected_boxes;
			while (map_[iter->second + j*n] == BOX || map_[iter->second + j*n] == BOX_ON_GOAL) {
				affected_boxes.insert(iter->second + j*n);
				j++;
			}
			if (map_[iter->second + j*n] != WALL) {
				vector<int> new_box_pos = barrels;
				int newPP = iter->second;
				for (int iter_ = 0; iter_ != new_box_pos.size(); iter_++) {
					if (affected_boxes.find(new_box_pos[iter_]) != affected_boxes.end()) {
						new_box_pos.at(iter_)+=n;
					}
				}
				result.push_back(State(new_box_pos, newPP, this));
			}
			break;
		}
		case 3: {
			int j = 0;
			set<int> affected_boxes;
			while (map_[iter->second - j*n] == BOX || map_[iter->second - j*n] == BOX_ON_GOAL) {
				affected_boxes.insert(iter->second - j*n);
				j++;
			}
			if (map_[iter->second - j*n] != WALL) {
				vector<int> new_box_pos = barrels;
				int newPP = iter->second;
				for (int iter_ = 0; iter_ != new_box_pos.size(); iter_++) {
					if (affected_boxes.find(new_box_pos[iter_]) != affected_boxes.end()) {
						new_box_pos.at(iter_)-=n;
					}
				}
				result.push_back(State(new_box_pos, newPP, this));
			}
			break;
		}
		}
	}
	return result;
}

bool State::is_deadlock(const int &n) {
	auto map = map_;
	for (int i = 0; i != barrels.size(); i++) {
		if (map[barrels[i]] != BOX_ON_GOAL) {
			//Corner deadlocks
			if (map[barrels[i] + 1] == WALL && (map[barrels[i] - n] == WALL || map[barrels[i] + n] == WALL)) return true;
			if (map[barrels[i] - 1] == WALL && (map[barrels[i] - n] == WALL || map[barrels[i] + n] == WALL)) return true;
			if (map[barrels[i] - 1] == BOX_ON_GOAL && this->is_deadlock(n, barrels[i] - 1) && (map[barrels[i] - n] == WALL || map[barrels[i] + n] == WALL)) return true;
			if (map[barrels[i] + 1] == BOX_ON_GOAL && this->is_deadlock(n, barrels[i] + 1) && (map[barrels[i] - n] == WALL || map[barrels[i] + n] == WALL)) return true;
			if (map[barrels[i] - n] == BOX_ON_GOAL && this->is_deadlock(n, barrels[i] - 1) && (map[barrels[i] - 1] == WALL || map[barrels[i] + 1] == WALL)) return true;
			if (map[barrels[i] + n] == BOX_ON_GOAL && this->is_deadlock(n, barrels[i] + 1) && (map[barrels[i] - 1] == WALL || map[barrels[i] + 1] == WALL)) return true;
		}
		//All onter deadlocks will lead to wrong solution anyway
	}
	return false;
}

vector<int> State::matrix_of_connections(const int &n) {
	int N = map_.size();
	vector<int> matrix(map_.size()*map_.size(),0);
	for (int i = 0; i != map_.size(); i++) {
		if (i > n && i + n < map_.size()) {
			switch (map_[i]) {
			case ' ': case '@': case '+': case PLR_ON_GOAL:
				if (map_[i - 1] == ' ' || map_[i - 1] == GOAL || map_[i - 1] == PLR)matrix[N * i + i - 1] = 1;
				if (map_[i + 1] == ' ' || map_[i + 1] == GOAL || map_[i + 1] == PLR)matrix[N * i + i + 1] = 1;
				if (map_[i - n] == ' ' || map_[i - n] == GOAL || map_[i - n] == PLR)matrix[N * i + i - n] = 1;
				if (map_[i + n] == ' ' || map_[i + n] == GOAL || map_[i + n] == PLR)matrix[N * i + i + n] = 1;
				break;
			default: matrix[N * i + i + n] = 0; break;
			}
		}
	}
	return matrix;
}

vector<char> &State::genmap(const vector<char>& map, const int &size) {
	if (map_.empty()) {
		map_ = map;
		
		for (unsigned int i = 0; i < map_.size(); i++) {
			if (map_[i] == BOX) map_[i] = ' ';
			if (map_[i] == BOX_ON_GOAL) map_[i] = GOAL;
			if (map_[i] == PLR) map_[i] = ' ';
			if (map_[i] == PLR_ON_GOAL) map_[i] = GOAL;
		}
		if (map_[PlayerPos] == GOAL || map_[PlayerPos] == PLR_ON_GOAL) map_[PlayerPos] = PLR_ON_GOAL;
		else
		map_[PlayerPos] = PLR;
		for (unsigned int i = 0; i < barrels.size(); i++) {
			if (map_[barrels[i]] == GOAL || map_[barrels[i]] == BOX_ON_GOAL) map_[barrels[i]] = BOX_ON_GOAL;
			else map_[barrels[i]] = BOX;
		}
		return map_;
	}
	else {
		return map_;
	}
}

bool State::is_deadlock( const int& n, const int& pos) {
	if (map_[pos] == WALL) return true;
	if (map_[pos + 1] == WALL && (map_[pos + n] == WALL || map_[pos - n] == WALL)) return true;
	if (map_[pos - 1] == WALL && (map_[pos + n] == WALL || map_[pos - n] == WALL)) return true;
	else return false;
}
