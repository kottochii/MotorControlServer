#include "helpers/strings.hpp"
#include <string>
#include <random>

std::string secsys_server::string_helpers::generate_random_string(int n)
{
    // Define the character set
    const std::string characterSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 generator(rd());  // mersenne twister engine (good quality random number generator)
    std::uniform_int_distribution<int> distribution(0, characterSet.size() - 1);

    std::string randomString;
    for (int i = 0; i < n; ++i) {
        int randomIndex = distribution(generator);
        randomString += characterSet[randomIndex];
    }

    return randomString;
}