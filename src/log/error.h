#ifndef DYNAMO_ERROR_H_
#define DYNAMO_ERROR_H_

#include <SDL2/SDL.h>

#include <exception>
#include <string>

namespace Dynamo {
    class SDLError : public std::exception {
    public:
        virtual const char *what() const throw();
    };

    class GenericError : public std::exception {
        std::string message_;

    public:
        GenericError(std::string message);
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
}

#endif