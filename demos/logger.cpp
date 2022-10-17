#include <Dynamo.hpp>
#include <iostream>

int main() {
    Dynamo::Log::info("Hello, world!");
    Dynamo::Log::warn("This is a warning...");
    Dynamo::Log::error("This is an error!");

    // Dead code
    std::cout << "This should not execute!" << std::endl;

    return 0;
}
