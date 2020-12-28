#include <iostream>
#include <stack>
#include <time.h>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>

using namespace std;

string password = "J9U8n0406";
int min_length = 2;
int max_length = 10; // maximum, minimun of password

string chars = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

string generate_word(int length) { // generate random word 
	string result = "";
	for (int i = 0; i < length; i++) {
		result += chars[rand() % chars.size()];
	}
	return result;
}

double fitness(string test_word) { // examine fitness of word
	double score = 0;
	if (password.length() != test_word.length())
		return score;
	
	score += 0.5; // if test_word is right length, plus 0.5 score

	for (int i = 0; i < password.length(); i++) {
		if (password[i] == test_word[i]) {
			score += 1; // if right char in right position, plus 1 score
		}
	}
	return score / (password.length() + 0.5) * 100;
}


bool cmpPair_second(pair<string, int>& left, pair<string, int>& right)
{
	return left.second > right.second;
}

vector<pair<string, int>> generate_population(int size, int min, int max) {
	vector<pair<string, int>> population;

	int length;
	string tmp;

	for (int i = 0; i < size; i++) {
		length = i % (max - min + 1) + min;
		tmp = generate_word(length);
		population.push_back(make_pair(tmp, fitness(tmp)));
	}
	sort(population.begin(), population.end(), cmpPair_second);

	return population;
}

void selectSurvivors(vector<pair<string, int>> population) {
	vector<pair<string, int>> cmpTmp = population; // 벡터 복사본

	vector<pair<string, int>>::iterator iter = cmpTmp.begin();
	
	for (int i = 0; i < population.size(); i++) {
		if (population[i].second == 0) {
			population.erase(iter + i, population.end());
		}
	}
	
	for (auto i : population) {
		cout << i.first << " 적합도 : ";
		cout << i.second << endl;
	}
}


int main() {
	srand((unsigned int)time(0));
	vector<pair<string, int>> test = generate_population(100, min_length, max_length);
	selectSurvivors(test);

	/*
	auto i = [](int left, int right) {
		printf("%d\n", left + right);
	};

	auto j = [](int left, int right) {
		int i = left - right;
		printf("%d\n", left - right);
		return i;
	};
	*/

	return 0;
}