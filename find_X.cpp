#include <iostream>
#include <vector>
#include <time.h>
#include <math.h>
#include <algorithm>

using namespace std;


class DNA {
public:
	double score;
	double x;
	double calcResult;
	bool isMutant = false;
	vector<int> nums;

	DNA(vector<int> Nums) : nums(Nums){
		int random = rand() % 2;
		if (random == 0) {
			random = 1;
		}
		else {// 50% Ȯ���� ������ �ȴ�.
			random = -1;
		}
		x = (rand() / (double)RAND_MAX * 100) * random;
		score = 0;
		calcResult = 0;
	}
	DNA(double num, vector<int> Nums) : nums(Nums) {
		x = num;
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
		int index = rand() % 5; // a,b,c,d,x �� �������� �ϳ� ����
		x =  (rand() % 19 + 1) * random; // ���õ� ��Ҹ� ������ ������ ��ü (��������)
		isMutant = true;
	}

	void display() {
		printf("X = %f \t ���յ� : %f", x, score);
		//cout << " X = " << x <<  "\t" << "���յ� : " << score;
		if (isMutant) cout << " << �������� ���";
		cout << "\n";
	}
};

double randDNA(double n1, double n2) {
	return (n1 + n2 / (rand() / (double)RAND_MAX * 2.0) );
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
	vector<int> nums;
	DNA* bestCase;


	vector<DNA*> population; // dna �����Ϳ��� ��ü�� �ٲ���� �� �� ���� ���� �߻�..
	vector<DNA*> newPopulation;

	geneticAlgorithm(int PopulationSize, int EliteSize, int MutantSize, vector<int> Nums) { // ������
		generation = 1;
		eliteSize = EliteSize;
		mutantSize = MutantSize;
		populationSize = PopulationSize;
		nums = Nums;

		isAnswer = false;

		for (int i = 0; i < populationSize; i++) {
			population.push_back(new DNA(nums));
		}

		for (auto i : population) {
			fitness(i);
		}
		//cout << "=== 1����  ===" << endl;
		//for (auto i : population) {
		//	i->display();
		//}
		
	}

	double calculate(DNA* dna) { // ���� ���
		// ���� ����
		// ax^2 + bx + c = 0
		double result = dna->nums[0] * pow(dna->x, 3) + dna->nums[1] * pow(dna->x, 2) + dna->nums[2] * dna->x + dna->nums[3];
		return result;//		   ax^3			   +			 bx^2			   +			cx		   +    d
	}

	void fitness(DNA* dna) { // ���յ� ���
		double calcResult = dna->calcResult = calculate(dna);
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
				DNA* tmp = new DNA(randDNA(population[i]->x, population[i + 1]->x), nums);
				if (abs(tmp->x) >= 100) {
					int random = rand() % 2 == 1 ? 1 : -1;
					tmp->x = rand() / (double)RAND_MAX * 100;
				}
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
		for (int i = 0; i < mutantSize; i++) {
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
			if (i->score >= 99.5) {
				bestCase = i;
				isAnswer = true;
			}
		}

		sort(population.begin(), population.end(), cmpScore);

		cout << "===== �� " << generation << "���� =====" << endl;
		
		cout << "���� 5�� ���յ� \n";
		for (int i = 0; i < 5; i ++) {
			population[i]->display();
		}
		//bestCase->display();
		cout << "\n";
	}


	void run() {
		while (isAnswer == false) {
			newGeneration();
		}
		cout << "\n" << " �����յ� ���� : ";
		bestCase->display();
	}

};

int main() {
	srand((unsigned)time(0));

	vector<int> abcd = { 3,-5,-1,2 };


	geneticAlgorithm ga = geneticAlgorithm(100, 20, 2, abcd);
	ga.run();

	return 0;
}