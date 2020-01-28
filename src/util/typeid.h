#ifndef DYNAMO_TYPEID_H_
#define DYNAMO_TYPEID_H_

namespace Dynamo {
    // Methods for generating unique type IDs
    class TypeID {
        unsigned id_counter_;

    public:
        TypeID();

        template <typename T>
        unsigned get_id() {
            static const unsigned id = id_counter_++;
            return id;
        }
    };
}

#endif