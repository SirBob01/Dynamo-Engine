#include <Dynamo.hpp>
#include <chrono>

int main() {
    // Generate and print a random number
    Dynamo::Random::seed(
        std::chrono::system_clock::now().time_since_epoch().count());
    Dynamo::Messenger::log(std::to_string(Dynamo::Random::random()));
}