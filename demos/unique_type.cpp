#include <Dynamo.hpp>

int main() {
    // Each type should have a unique identifier
    Dynamo::TypeId types;

    Dynamo::Log::info("int = {}", types.get<int>());
    Dynamo::Log::info("float = {}", types.get<float>());
    Dynamo::Log::info("int = {}", types.get<int>());
    Dynamo::Log::info("char = {}", types.get<char>());
    Dynamo::Log::info("char = {}", types.get<char>());
    return 0;
}
