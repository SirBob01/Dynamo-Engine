#ifndef DYNAMO_ERROR_H_
#define DYNAMO_ERROR_H_

#include <exception>
#include <string>

namespace Dynamo {
	class InvalidKey : public std::exception {
		std::string key_;
		std::string map_name_;
		std::string msg_;

	public:
		InvalidKey(std::string key, std::string map_name);
		virtual const char *what() const throw();
	};
}

#endif