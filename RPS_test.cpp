#include <iostream>
#include <random>

std::random_device seed_gen;
std::mt19937_64 engine(seed_gen());

const int NUM_ACTIONS = 3;

int main()
{
    double oppStrategy[NUM_ACTIONS] = { 0.4, 0.3, 0.3 };
    double strategy[NUM_ACTIONS] = { 0.0, 1.0, 0.0 };
    std::discrete_distribution<size_t> oppDist(oppStrategy, oppStrategy + NUM_ACTIONS);
    std::discrete_distribution<size_t> dist(strategy, strategy + NUM_ACTIONS);

    double sumUtility = 0;
    const int PLAY_NUM = 10000000;
    for (int i = 0; i < PLAY_NUM; ++i) {
        auto action = dist(engine);
        auto oppAction = oppDist(engine);

        sumUtility += action == oppAction ? 0 : (action == (oppAction + 1) % NUM_ACTIONS ? 1 : -1);
    }

    std::cout << sumUtility / PLAY_NUM << std::endl;

    return 0;
}
