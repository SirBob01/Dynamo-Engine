#include <Dynamo.hpp>
#include <iostream>

int main() {
    Dynamo::Messenger::log("Hello, world!");
    Dynamo::Messenger::log("This is a logging example!");
    Dynamo::Messenger::error("This error is silent...", false);
    Dynamo::Messenger::error("This error will crash!");

    // Dead code
    std::cout << "This should not execute!" << std::endl;

    return 0;
}
