#include <iostream>
#include <random>
#include <memory>

class RPSTrainer
{
public:
    static const int ROCK = 0, PAPER = 1, SCISSORS = 2, NUM_ACTIONS = 3;
    static std::random_device seed_gen;
    static std::mt19937_64 engine;
    double regretSum[NUM_ACTIONS] = { 0 };
    double strategy[NUM_ACTIONS] = { 0 };
    double strategySum[NUM_ACTIONS] = { 0 };

    void getStrategy() {
        double normalizingSum = 0;
        for (int a = 0; a < NUM_ACTIONS; a++) {
            strategy[a] = regretSum[a] > 0 ? regretSum[a] : 0;
            normalizingSum += strategy[a];
        }
        for (int a = 0; a < NUM_ACTIONS; a++) {
            if (normalizingSum > 0)
              strategy[a] /= normalizingSum;
            else
              strategy[a] = 1.0 / NUM_ACTIONS;
            strategySum[a] += strategy[a];
        }
    }
    
    int getAction() {
        std::discrete_distribution<size_t> dist(strategy, strategy + NUM_ACTIONS);
        return dist(engine);
    }

    void updateRegret(int myAction, int otherAction) {
        double actionUtility[NUM_ACTIONS];

        actionUtility[otherAction] = 0;
        actionUtility[otherAction == NUM_ACTIONS - 1 ? 0 : otherAction + 1] = 1;
        actionUtility[otherAction == 0 ? NUM_ACTIONS - 1 : otherAction - 1] = -1;

        for (int a = 0; a < NUM_ACTIONS; a++)
            regretSum[a] += actionUtility[a] - actionUtility[myAction];
    }
    

    static void train(int iterations, RPSTrainer& trainer, RPSTrainer& oppTrainer) {
        for (int i = 0; i < iterations; i++) {
            trainer.getStrategy();
            oppTrainer.getStrategy();
            int myAction = trainer.getAction();
            int otherAction = oppTrainer.getAction();

            trainer.updateRegret(myAction, otherAction);
            oppTrainer.updateRegret(otherAction, myAction);

            if ((i + 1) % 100 == 0)
                trainer.printAverageStrategy();
        }
    }
    

    void printAverageStrategy() {
        double avgStrategy[NUM_ACTIONS];
        double normalizingSum = 0;
        for (int a = 0; a < NUM_ACTIONS; a++)
            normalizingSum += strategySum[a];
        for (int a = 0; a < NUM_ACTIONS; a++) 
            if (normalizingSum > 0)
                avgStrategy[a] = strategySum[a] / normalizingSum;
            else
                avgStrategy[a] = 1.0 / NUM_ACTIONS;
        std::cout << "[" << avgStrategy[0] << ", " << avgStrategy[1] << ", " << avgStrategy[2] << "]" << std::endl;
    }

};

std::random_device RPSTrainer::seed_gen;
std::mt19937_64 RPSTrainer::engine(RPSTrainer::seed_gen());

int main()
{
    RPSTrainer trainer;
    RPSTrainer oppTrainer;
    RPSTrainer::train(10000, trainer, oppTrainer);
    trainer.printAverageStrategy();
    return 0;
}
