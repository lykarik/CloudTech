#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include<thread>
#include<mutex> 
#include <cctype>
#include <ctime>
#include<cstdlib> 
#include<windows.h>

using namespace std;


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

struct final_word {
	string key;
	int value;
};

void push(ifstream &in, vector<string> &arr, mutex& m_arr, int &control) {
	string str;
	double count=0;
	while (!in.eof()) {
		in >> str;
		count++;
		if (str.size() < 2) { continue; }
		m_arr.lock();
		arr.push_back(str);
		m_arr.unlock();
	}
	control = 1;
	cout << "count - " << count << endl; 
}

void pop(vector<string> &arr, vector<word> &victor, mutex& m_arr, mutex& m_victor, int &control) {
	string ch;
	word buffer;
	buffer.value = 1;
	while (control != 1){
		while (arr.size() > 2){
			m_arr.lock();
			ch = arr.back();
			arr.pop_back();
			m_arr.unlock();
			for (std::string::iterator it = ch.begin(); it <= ch.end(); it++)
			{
				if (ispunct(*it)) { ch.erase(it); } // if symbol ==  " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ ] ^ _ ` { | } ~ then delete it
				if (isupper(*it)) { *it = tolower(*it); } // if symbol if uppercase -> lowercase
			}
			for (std::string::iterator it = ch.begin(); it <= ch.end(); it++)  //for clear result?????
			{
				if (ispunct(*it)) { ch.erase(it); }
			}
			buffer.key = ch;
			m_victor.lock();
			victor.push_back(buffer);
			m_victor.unlock();
		}
	}
}

int main(){
	cout << "Starting threads!" << endl;
	ifstream in("new.txt");
	ofstream out("result.txt");
	string ch;
	vector<string> string_victor;
	mutex m_arr;
	mutex m_victor;
	int control = 0;

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int numCPU = sysinfo.dwNumberOfProcessors;
	const int length = 3;
	thread thread_array[length];
	word buffer;
	new_word buffer2;
	final_word buffer3;
	vector<word> victor;
	vector<new_word> mini_victor;
	vector<final_word> final_victor;

	srand(time(0));

	thread push_thread(push, ref(in), ref(string_victor), ref(m_arr), ref(control));


	for (int i = 0; i < length; i++) {
		thread_array[i] = thread(pop, ref(string_victor), ref(victor), ref(m_arr), ref(m_victor), ref(control));
	}
	for (int i = 0; i < length; i++) {
		if (thread_array[i].joinable()) {
			thread_array[i].join();
		}
	}
	if (push_thread.joinable()) push_thread.join();

	cout << "Strating sort" << endl;
	double s_time = clock() / 1000.0;
	sort(victor.begin(), victor.end());
	cout << "MAP + Sort time: " << clock() / 1000.0 << endl;

	buffer.key = "";
	buffer.value = 0;
	victor.push_back(buffer);


	double V_size = victor.size();
	for (double i = 0; i < V_size; i++) {

		buffer2.key = victor[i].key;
		buffer2.value.push_back(1);

		while (victor[i + 1].key == buffer2.key){
			buffer2.value.push_back(1);
			i++;
			if (i == V_size) { break; }
		}
		mini_victor.push_back(buffer2);
		buffer2.value.clear();
	}

	for (int i = 0; i < mini_victor.size() - 1; i++) {
		buffer3.key = mini_victor[i].key;
		buffer3.value = mini_victor[i].value.size();
		final_victor.push_back(buffer3);
		out << final_victor[i].key << " , " << final_victor[i].value << endl;
	}
	cout << "runtime - " << clock() / 1000.0 << endl;

	in.close();
	out.close();
	system("pause");

}
