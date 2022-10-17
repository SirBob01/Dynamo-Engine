#include <Dynamo.hpp>

int main() {
    // Each type should have a unique identifier
    Dynamo::Messenger::log("int = {}", Dynamo::TypeId::get<int>());
    Dynamo::Messenger::log("float = {}", Dynamo::TypeId::get<float>());
    Dynamo::Messenger::log("int = {}", Dynamo::TypeId::get<int>());
    Dynamo::Messenger::log("char = {}", Dynamo::TypeId::get<char>());
    Dynamo::Messenger::log("char = {}", Dynamo::TypeId::get<char>());
    return 0;
}
