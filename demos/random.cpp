#include <Dynamo.hpp>
#include <chrono>

int main() {
    // Generate and print a random number
    Dynamo::Random::seed(
        std::chrono::system_clock::now().time_since_epoch().count());
    Dynamo::Log::info("Random: {}", Dynamo::Random::random());
}