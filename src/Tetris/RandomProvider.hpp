#pragma once
#include <random>


class RandomProvider {
public:
    RandomProvider() {
        rng.seed(std::random_device()());
    }

    unsigned RandomProvider::get(unsigned min, unsigned max) {
        return std::uniform_int_distribution<std::mt19937::result_type>{min, max}(rng);
    }

private:
    std::mt19937 rng;
};
