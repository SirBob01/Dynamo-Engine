#include <Dynamo.hpp>
#include <iostream>

int main() {
    Dynamo::Messenger::log("Hello, world!");
    Dynamo::Messenger::log("This is a logging example!");
    Dynamo::Messenger::error(Dynamo::ErrorCode::CoreFailure);

    // Dead code
    std::cout << "This should not execute!" << std::endl;

    return 0;
}