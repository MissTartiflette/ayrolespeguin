
#include "server.h"

using namespace server;

VersionService::VersionService () : AbstractService("/version") {
    
}

HttpStatus VersionService::get (Json::Value& out, int id) const {
    out["major"] = 1;
    out["minor"] = 0;
    return HttpStatus::OK;
}
