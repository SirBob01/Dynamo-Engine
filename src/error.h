#ifndef DYNAMO_ERROR_H_
#define DYNAMO_ERROR_H_

#include <exception>
#include <string>

namespace Dynamo {
    class SDLError : public std::exception {
        std::string message_;

    public:
        SDLError(const char *str);
        virtual const char *what() const throw();
    };
    
    class PacketOverflow : public std::exception {
        std::string len_;
        std::string max_;
        std::string message_;

    public:
        PacketOverflow(int max, int size);
        virtual const char *what() const throw();
    };

    class InvalidKey : public std::exception {
        std::string message_;

    public:
        InvalidKey(std::string key, std::string map_name);
        virtual const char *what() const throw();
    };

    class ValueExists : public std::exception {
        std::string message_;

    public:
        ValueExists(std::string key, std::string map_name);
        virtual const char *what() const throw();
    };
}

#endif