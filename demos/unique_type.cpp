#include <Dynamo.hpp>

int main() {
    // Each type should have a unique identifier
    Dynamo::Log::info("int = {}", Dynamo::TypeId::get<int>());
    Dynamo::Log::info("float = {}", Dynamo::TypeId::get<float>());
    Dynamo::Log::info("int = {}", Dynamo::TypeId::get<int>());
    Dynamo::Log::info("char = {}", Dynamo::TypeId::get<char>());
    Dynamo::Log::info("char = {}", Dynamo::TypeId::get<char>());
    return 0;
}
