#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <algorithm>

using namespace std;

class DNA {
public:
	int score;
	vector<int> num;
	int calcResult;
	bool isMutant = false;

	DNA() {
		for (int i = 0; i < 4; i++) {
			int random = rand() % 2;
			if (random == 0) {
				random = 1;
			}
			else {// 50% Ȯ���� ������ �ȴ�.
				random = -1;
			}
			num.push_back((rand() % 10 + 1) * random); // ��� ���� �ּڰ��� 1�̴�.
		}
		score = 0;
		calcResult = 0;
	}
	DNA(int a, int b, int c, int x) {
		num.push_back(a);
		num.push_back(b);
		num.push_back(c);
		num.push_back(x);
		score = 0;
		calcResult = 0;
	}

	void mutant() {
		int random = rand() % 2;

		if (random == 0) {
			random = 1;
		}
		else
			random = -1;
		int index = rand() % 4; // a,b,c,x �� �������� �ϳ� ����
		num[index] = (rand() % 10 + 1) * random; // ���õ� ��Ҹ� ������ ������ ��ü (��������)
		isMutant = true;
	}

	void display() {
		cout << "a : " << num[0] << " b : " << num[1] << " c : " << num[2] << " x : " << num[3] << "\t" << "���յ� : " << score;
		if (isMutant) cout << " << �������� ���";
		cout << "\n";
	}
};

int randDNA(int n1, int n2) {
	return (n1 + n2) / 2;
}

bool cmpScore(DNA* a, DNA* b) {
	return a->score > b->score;
}

class geneticAlgorithm {
public:

	int generation;
	int eliteSize;
	int mutantSize;
	int populationSize;
	bool isAnswer;
	DNA* bestCase;


	vector<DNA*> population; // dna �����Ϳ��� ��ü�� �ٲ���� �� �� ���� ���� �߻�..
	vector<DNA*> newPopulation;

	geneticAlgorithm(int PopulationSize, int EliteSize, int MutantSize) { // ������
		generation = 1;
		eliteSize = EliteSize;
		mutantSize = MutantSize;
		populationSize = PopulationSize;

		isAnswer = false;

		for (int i = 0; i < populationSize; i++) {
			population.push_back(new DNA);
		}

		for (auto i : population) {
			fitness(i);
		}
		/*cout << "=== ���յ� �˻� �Ϸ� ===" << endl;
		for (auto i : population) {
			i->display();
		}
		*/
	}

	int calculate(DNA* dna) { // ���� ���
		// ���� ����
		// ax^2 + bx + c = 0
		int result = dna->num[0] * pow(dna->num[3], 2) + dna->num[1] * dna->num[3] + dna->num[2];
		return result;
	}

	void fitness(DNA* dna) { // ���յ� ���
		int calcResult = dna->calcResult = calculate(dna);
		if (calcResult > 50 || calcResult < -50) {
			dna->score = 0;
		}
		else {
			if (calcResult < 0)
				calcResult *= -1;
			dna->score = (50 - calcResult) * 2;
		}
	}

	void selectSurvivors() { // �θ� ����		
		sort(population.begin(), population.end(), cmpScore);
		bestCase = population[0];
		for (int i = 0; i < eliteSize; i++) {
			newPopulation.push_back(population[i]);
		}

		/*cout << "=== ���յ� ���� ���� ���� 40�� ===" << endl;

		for (auto i : newPopulation) {
			i->display();
		}
		cout << "==================================" << endl;
		*/
	}

	void crossDNA() {
		int index = 0;
		vector<DNA*> child;

		for (int i = 0; i < newPopulation.size(); i = i + 2) {
			for (int j = 0; j < 5; j++) {
				DNA* tmp = new DNA(randDNA(population[i]->num[0], population[i + 1]->num[0]),
					randDNA(population[i]->num[1], population[i + 1]->num[1]),
					randDNA(population[i]->num[2], population[i + 1]->num[2]),
					randDNA(population[i]->num[3], population[i + 1]->num[3]));
				child.push_back(tmp);
			}
		}

		population.clear();
		newPopulation.clear();
		population = child;
		child.clear();
		generation++;
	}

	void mutant() {
		for(int i = 0; i < mutantSize; i++){
			int randIndex = rand() % population.size();
			population[randIndex]->mutant();
		}
	}

	void newGeneration() {
		selectSurvivors();
		crossDNA();
		mutant();
		for (auto i : population) {
			fitness(i); // ���յ� ����
			if (i->score == 100) { 
				bestCase = i;
				isAnswer = true; 
			}
		}
		cout << "===== �� " << generation << "���� =====" << endl;
		/*for (auto i : population) {
			i->display();
		}
		*/
		cout << "�ְ� ���յ� :";
		bestCase->display();
	}


	void run() {
		while (isAnswer == false) {
			newGeneration();
		}
		cout <<"\n" << " �����յ� ���� : ";
		bestCase->display();
	}

};

int main() {
	srand((unsigned)time(0));


	geneticAlgorithm ga = geneticAlgorithm(10, 2, 1);
	ga.run();
	
	return 0;
}