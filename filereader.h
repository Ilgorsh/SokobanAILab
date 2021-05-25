#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

class filereader
{
public:
	vector<char> readmap(const string &filename) {
		ifstream in(filename);
		vector<char> result;
		char buffer = in.get();
		int k = 0;
		while (buffer != EOF) {
			if (buffer == '\n'){
				size = k;
				k = 0;
				buffer = in.get();
			} else {
				k++;
				result.push_back(buffer);
				buffer = in.get();
			}
		}
		cout << "MAP SIZE " << size << endl;

		return result;
	};
	int getsize() { return size; };
private:
	int size;
};

