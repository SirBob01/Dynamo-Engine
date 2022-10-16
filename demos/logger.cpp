#include <Dynamo.hpp>
#include <iostream>

int main() {
    Dynamo::Messenger::log("Hello, world!");
    Dynamo::Messenger::warn("This is a warning...");
    Dynamo::Messenger::error("This is an error!");

    // Dead code
    std::cout << "This should not execute!" << std::endl;

    return 0;
}
