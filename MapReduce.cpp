#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;
using std::string;

struct word {
	string key;
	int value;
	bool operator< (const word& a) const{ //for string sort
		return key < a.key;
	}
};

struct new_word {
	string key;
	vector <int> value;

};

struct final_word { //for result
	string key;
	int value;
};

int main(){
	ifstream in("new.txt");
	string ch;
	word buffer;
	new_word buffer2;
	final_word buffer3;
	vector<word> victor;
	vector<new_word> mini_victor;
	vector<final_word> final_victor;

	while (!in.eof()) { // not the end of our file
		in >> ch;
		buffer.key = ch;
		buffer.value = 1;
		victor.push_back(buffer);
	}

	sort(victor.begin(), victor.end());
	cout << endl << " >>> sort of result <<< " << endl << endl;
	for (int i = 0; i < victor.size(); i++) {
		cout << victor[i].key << " ; " << victor[i].value << endl;
	}
	buffer.key = "";
	buffer.value = 0;


	victor.push_back(buffer);
	for (int i = 0; i < victor.size(); i++) {
		buffer2.key = victor[i].key;
		buffer2.value.push_back(1);
	
		while (victor[i + 1].key == buffer2.key){
			i++;
			buffer2.value.push_back(1);
		}
		mini_victor.push_back(buffer2);
		
		buffer2.value.clear();
	}
	cout << endl << endl;

	cout << "MapReduse: " << endl;
	for (int i = 0; i < mini_victor.size()-1; i++) {
		buffer3.key = mini_victor[i].key;
		buffer3.value = mini_victor[i].value.size();
		final_victor.push_back(buffer3);
		cout << final_victor[i].key << " , " << final_victor[i].value << endl;
	}

	system("pause");
}
