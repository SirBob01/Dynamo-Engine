#include <Dynamo.hpp>

int main() {
    // Each type should have a unique identifier
    Dynamo::Messenger::log("int = " +
                           std::to_string(Dynamo::TypeId::get<int>()));
    Dynamo::Messenger::log("float = " +
                           std::to_string(Dynamo::TypeId::get<float>()));
    Dynamo::Messenger::log("int = " +
                           std::to_string(Dynamo::TypeId::get<int>()));
    Dynamo::Messenger::log("char = " +
                           std::to_string(Dynamo::TypeId::get<char>()));
    Dynamo::Messenger::log("char = " +
                           std::to_string(Dynamo::TypeId::get<char>()));
    return 0;
}
