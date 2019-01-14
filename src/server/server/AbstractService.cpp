#include "server.h"
#include <iostream>
using namespace server;
using namespace std;

AbstractService::AbstractService (const std::string& pattern) {
    setPattern(pattern);
}

AbstractService::~AbstractService() {
    
}

const string& AbstractService::getPattern () const {
	cout<<"truc" <<pattern<<endl;
    return pattern;
}

void AbstractService::setPattern (const std::string& pattern) {
    this->pattern = pattern;
}

HttpStatus AbstractService::get (Json::Value& out, int id) const {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus AbstractService::post (const Json::Value& in, int id) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus AbstractService::put (Json::Value& out, const Json::Value& in) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}

HttpStatus AbstractService::remove (int id) {
    throw ServiceException(HttpStatus::NOT_IMPLEMENTED,"Non implanté");
}
