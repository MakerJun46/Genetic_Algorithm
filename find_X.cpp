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
		else {// 50% 확률로 음수가 된다.
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
		int index = rand() % 5; // a,b,c,d,x 중 무작위로 하나 선택
		x =  (rand() % 19 + 1) * random; // 선택된 요소를 무작위 값으로 대체 (돌연변이)
		isMutant = true;
	}

	void display() {
		printf("X = %f \t 적합도 : %f", x, score);
		//cout << " X = " << x <<  "\t" << "적합도 : " << score;
		if (isMutant) cout << " << 돌연변이 요소";
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


	vector<DNA*> population; // dna 포인터에서 객체로 바꿨더니 알 수 없는 오류 발생..
	vector<DNA*> newPopulation;

	geneticAlgorithm(int PopulationSize, int EliteSize, int MutantSize, vector<int> Nums) { // 생성자
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
		//cout << "=== 1세대  ===" << endl;
		//for (auto i : population) {
		//	i->display();
		//}
		
	}

	double calculate(DNA* dna) { // 수식 계산
		// 수식 결정
		// ax^2 + bx + c = 0
		double result = dna->nums[0] * pow(dna->x, 3) + dna->nums[1] * pow(dna->x, 2) + dna->nums[2] * dna->x + dna->nums[3];
		return result;//		   ax^3			   +			 bx^2			   +			cx		   +    d
	}

	void fitness(DNA* dna) { // 적합도 계산
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

	void selectSurvivors() { // 부모 선별		
		sort(population.begin(), population.end(), cmpScore);
		bestCase = population[0];
		for (int i = 0; i < eliteSize; i++) {
			newPopulation.push_back(population[i]);
		}

		/*cout << "=== 적합도 점수 정렬 상위 40개 ===" << endl;

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
			fitness(i); // 적합도 측정
			if (i->score >= 99.5) {
				bestCase = i;
				isAnswer = true;
			}
		}

		sort(population.begin(), population.end(), cmpScore);

		cout << "===== 제 " << generation << "세대 =====" << endl;
		
		cout << "상위 5개 적합도 \n";
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
		cout << "\n" << " 최적합도 정답 : ";
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