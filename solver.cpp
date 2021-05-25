#include "solver.h"

string solver::findsolution(const vector<State>& states,const int &n) {
	vector<State> additional;
	for (auto i : states) {
		additional.push_back(i);
		if (i.prev_map != vector<char>(1, ' ')) {
			
			State between(i);
			int temp = 0;
			int prevPP = 0;
			int moved_box;
			vector<int> prev_barrels;
			for (auto h : i.prev_map) {
				if (h == BOX || h == BOX_ON_GOAL)prev_barrels.push_back(temp);
				temp++;
			}
			vector<int> curr_barrels = i.barrels;
			int next_;
			if (find(i.barrels.begin(), i.barrels.end(), i.PlayerPos + 1) != i.barrels.end()) {
				vector<int> moved;
				next_ = *find(i.barrels.begin(), i.barrels.end(), i.PlayerPos + 1);
				while (i.map_[next_] == BOX || i.map_[next_] == BOX_ON_GOAL) {
					moved.push_back(next_);
					next_++;
				}
				if (i.prev_map[next_ - 1] == ' ' || i.prev_map[next_ - 1] == GOAL || i.prev_map[next_ - 1] == PLR || i.prev_map[next_ - 1] == PLR_ON_GOAL) {
					prevPP = i.PlayerPos - 1;
				}
			}
			else if (find(i.barrels.begin(), i.barrels.end(), i.PlayerPos - 1) != i.barrels.end()) {
				vector<int> moved;
				next_ = *find(i.barrels.begin(), i.barrels.end(), i.PlayerPos - 1);
				while (i.map_[next_] == BOX || i.map_[next_] == BOX_ON_GOAL) {
					moved.push_back(next_);
					next_--;
				}
				if (i.prev_map[next_ + 1] == ' ' || i.prev_map[next_ + 1] == GOAL || i.prev_map[next_ + 1] == PLR || i.prev_map[next_ + 1] == PLR_ON_GOAL) {
					prevPP = i.PlayerPos + 1;
				}
			}
			else if (find(i.barrels.begin(), i.barrels.end(), i.PlayerPos - n) != i.barrels.end()) {
				vector<int> moved;
				next_ = *find(i.barrels.begin(), i.barrels.end(), i.PlayerPos - n);
				while (i.map_[next_] == BOX || i.map_[next_] == BOX_ON_GOAL) {
					moved.push_back(next_);
					next_ -= n;
				}
				if (i.prev_map[next_ + n] == ' ' || i.prev_map[next_ + n] == GOAL || i.prev_map[next_ + n] == PLR || i.prev_map[next_ + n] == PLR_ON_GOAL) {
					prevPP = i.PlayerPos + n;
				}
			}
			else if (find(i.barrels.begin(), i.barrels.end(), i.PlayerPos + n) != i.barrels.end()) {
				vector<int> moved;
				next_ = *find(i.barrels.begin(), i.barrels.end(), i.PlayerPos + n);
				while (i.map_[next_] == BOX || i.map_[next_] == BOX_ON_GOAL) {
					moved.push_back(next_);
					next_ += n;
				}
				if (i.prev_map[next_ - n] == ' ' || i.prev_map[next_ - n] == GOAL || i.prev_map[next_ - n] == PLR || i.prev_map[next_ - n] == PLR_ON_GOAL) {
					prevPP = i.PlayerPos - n;
				}
			}
			State newState(prev_barrels, prevPP, i.prev_map);
			newState.genmap(i.prev_map, n);
			additional.push_back(newState);
		}
	}
	int s = 0;
	for (auto f : additional) {
		for (auto g : f.map_) {
			cout << g << ' ';
			if ((s + 1) % n == 0)cout << endl;
			s++;
		}
	}
	vector<string> results;
	string solution;
	vector<int> PAf;
	for (auto i = additional.begin(); i != additional.end(); i++) {
		if (i->prev_map != vector<char>(1, ' ')) {
			State curr = *i;
			//Djikstra
			auto MATRIX = curr.matrix_of_connections(n);
			vector<int> Distances(curr.map_.size(), 9);
			Distances[curr.PlayerPos] = 0;
			vector<int> visited(curr.map_.size(), 0);
			visited[curr.PlayerPos] = 1;
			vector<int> ancestors(curr.map_.size(), -1);
			ancestors[curr.PlayerPos] == curr.PlayerPos;
			int current_node = curr.PlayerPos;
			while (find(visited.begin(), visited.end(), 0) != visited.end()) {
				for (int j = 0; j < curr.map_.size(); j++) {
					if (MATRIX[current_node * curr.map_.size() + j] != 0 && MATRIX[current_node * curr.map_.size() + j] + Distances[current_node] < Distances[j]) {
						Distances[j] = MATRIX[current_node * curr.map_.size() + j] + Distances[current_node];
						ancestors[j] = current_node;
					}
				}
				
				visited[current_node] = 1;
				int min = 1000;
				int J = 0;
				for (int j = 0; j < curr.map_.size(); j++) {
					if (Distances[j] < min && visited[j] == 0) {
						min = Distances[j];
						J = j;
					}
				}
				/*int b = 0;
				for (auto a : Distances) {
					cout << a << ' ';
					b++;
					if (b % n == 0)cout << endl;
				}
				*/
				current_node = J;
			}
			cout << "Distances:" << endl;
			int b = 0;
			for (auto a : Distances) {
					cout << a << ' ';
					b++;
					if (b % n == 0)cout << endl;
			}
			b = 0;
			cout << "MAP" << endl;
			for (auto a : Distances) {
				cout << b << ' ';
				b++;
				if (b % n == 0)cout << endl;
			}
			cout << "Current" << endl;
			b = 0;
			for (auto a : i->map_) {
				cout << a << ' ';
				b++;
				if (b % n == 0)cout << endl;
			}
			b = 0;
			auto f = i;
			f++;
			cout << "Prev" << endl;
			for (auto a :f->map_) {
				cout << a << ' ';
				b++;
				if (b % n == 0)cout << endl;
			}
			b = 0;
			cout << "Ancestors" << endl;
			for (auto a : ancestors) {
				cout << a << ' ';
				b++;
				if (b % n == 0)cout << endl;
			}
			vector<int> path;
			auto h = i;
			h++;
			cout << h->PlayerPos << endl;
			current_node = h->PlayerPos;
			path.push_back(current_node);
			while (current_node != curr.PlayerPos) {
				path.push_back(ancestors[current_node]);
				cout << current_node << " Ancestor is " << ancestors[current_node] << endl;
				current_node = ancestors[current_node];
			}
			cout << "PAth " << endl;
 			for (int l = 0; l < path.size(); l++) {
				cout << path[l] << " ";
			}
			cout << endl;
			string PATH_TXT;
			reverse(path.begin(), path.end());
			PAf.insert(PAf.end(),path.begin(),path.end());
		}
	}
	for (auto k = PAf.begin(); k != PAf.end();k++) {
		auto a = k;
		a++;
		if (a != PAf.end() && *a == *k)PAf.erase(a);
	}
	for (auto k = PAf.begin(); k != PAf.end(); k++) {
		cout << *k << " ";
		auto a = k;
		a++;
		if (a != PAf.end()) {
			if (*a - *k == -n)solution += "D";
			else if (*a - *k == n)solution += "U";
			else if (*a - *k == 1)solution += "L";
			else if (*a - *k == -1)solution += "R";
		}
	}
	cout << endl;
	reverse(solution.begin(), solution.end());
	
	return solution;
}

vector<State> solver::solve(const string &levelname) {
	filereader reader;
	const vector<char> levelmap_ = reader.readmap(levelname);
	const int levelsize_ = reader.getsize();
	queue<State> stateQ;
	vector<State> final_states;
	vector<State> State_set;
	vector<int> boxes;
	int a = 0;
	int PP, k = 0;
	for (auto iter = levelmap_.begin(); iter != levelmap_.end(); iter++) {
		if (*iter == BOX || *iter == BOX_ON_GOAL)boxes.push_back(k);
		if (*iter == PLR || *iter == PLR_ON_GOAL)PP = k;
		k++;
	}
	State beg_state(boxes,PP,vector<char>(1,' '));
	beg_state.depth = 0;
	beg_state.genmap(levelmap_,levelsize_);
	stateQ.push(beg_state);
	State_set.push_back(beg_state);
	while (!stateQ.empty()) {
 		State curr_state(stateQ.front());
		if (curr_state.depth < 200) {
			curr_state.genmap(levelmap_, levelsize_);
			auto newStates = curr_state.push(levelsize_);
			for (int iter = 0; iter != newStates.size(); iter++) {
				newStates[iter].genmap(levelmap_, levelsize_);
				newStates[iter].prev_state = &curr_state;
				if (newStates[iter].is_deadlock(levelsize_)) {}
				else if (std::find(State_set.begin(), State_set.end(), newStates[iter]) != State_set.end()) {
					auto jter = find(State_set.begin(), State_set.end(), newStates[iter]);
					if (jter->depth > newStates[iter].depth) {
						State_set.erase(jter);
						State_set.push_back(newStates[iter]);
						if (newStates[iter].is_solved(levelmap_)) {
							final_states.push_back(newStates[iter]);
						}
						else {

							stateQ.push(newStates[iter]);
						}

					}
				}
				else {
					State_set.push_back(newStates[iter]);
					if (newStates[iter].is_solved(levelmap_)) {
						final_states.push_back(newStates[iter]);
					}
					else {
						stateQ.push(newStates[iter]);
					}
				}
			}
		}
		stateQ.pop();
		a++;
	}
	vector<State> answer(1000);
	vector<State> path;
	for (auto h : final_states) {
		path.push_back(h);
		State curr_stt(h);
		while (curr_stt.prev_map != vector<char>(1, ' ')) {
			for (auto j : State_set) {
				if (j.map_ == curr_stt.prev_map)curr_stt = j;
			}
			path.push_back(curr_stt);
		}
		if (path.size() <= answer.size()) {
			answer.clear();
			answer.resize(path.size());
			answer = path;
		}
	}
	return answer;
}
