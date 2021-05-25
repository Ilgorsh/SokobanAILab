
#include "State.h"
#include <string>
#include <queue>
#include <unordered_set>
#include "filereader.h"
#include <iostream>

using namespace std;

class solver
{
public:
	vector<State> solve(const string&);
	string findsolution(const vector<State>&,const int&);
	
};

