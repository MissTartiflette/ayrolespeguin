#include "server.h"
#include <iostream>
using namespace std;
using namespace server;

void ServicesManager::registerService (unique_ptr<AbstractService> service) {
    services.push_back(std::move(service));
}

AbstractService* ServicesManager::findService (const string& url) const {
	
    for (auto& service : services) {
        const string& pattern(service->getPattern());
		
        if (url.find(pattern) != 0)
			
            continue;
        if (url.size() > pattern.size() && url[pattern.size()] != '/')
			
            continue;
        return service.get();
    }
    return nullptr;
}

HttpStatus ServicesManager::queryService (string& out, const string& in, const string& url, const string& method) { 
    AbstractService* service = findService(url);
    if (!service)
		
        throw ServiceException(HttpStatus::NOT_FOUND,"Service "+url+" non trouvé");
    // Recherche un éventuel id (ex: /mon/service/<id>)
    const string& pattern(service->getPattern());
    int id = 0;
	
    if (url.size() > pattern.size()) {
        string end = url.substr(pattern.size());
        if (end[0] != '/')
            throw ServiceException(HttpStatus::BAD_REQUEST,"Url malformée (forme attendue: <service>/<nombre>)");
        end = end.substr(1);
        if (end.empty())
            throw ServiceException(HttpStatus::BAD_REQUEST,"Url malformée (forme attendue: <service>/<nombre>)");
        try {
            size_t pos = 0;
            id = stoi(end,&pos);
            if (pos != end.size())
                throw ServiceException(HttpStatus::BAD_REQUEST,"Url malformée: '"+end+"' n'est pas un nombre");
        }
        catch(...) {
            throw ServiceException(HttpStatus::BAD_REQUEST,"Url malformée: '"+end+"' n'est pas un nombre");
        }
    }
    // Traite les différentes méthodes
    if (method == "GET") {
        cerr << "Requête GET " << pattern << " avec id=" << id << endl;
        Json::Value jsonOut;
        HttpStatus status = service->get(jsonOut,id);
        out = jsonOut.toStyledString();
        return status;
    }
    else if (method == "POST" && in[2]!='r' && in[0]=='{') {
        cerr << "Requête POST " << pattern << " avec contenu: " << in << endl;
        Json::Reader jsonReader;
        Json::Value jsonIn;
        if (!jsonReader.parse(in,jsonIn))
            throw ServiceException(HttpStatus::BAD_REQUEST,"Données invalides: "+jsonReader.getFormattedErrorMessages());
        return service->post(jsonIn,id);
    }
    else if ((method == "POST" && in[2]=='r'&& in[0]=='{')) {
		
		Json::Reader jsonReader;
        Json::Value jsonIn;
        if (!jsonReader.parse(in,jsonIn)){
            throw ServiceException(HttpStatus::BAD_REQUEST,"Données invalides: "+jsonReader.getFormattedErrorMessages());
			cerr << "Requête PUT " << pattern << " avec contenu:" << in << endl;
		}
        cerr << "Requête PUT " << pattern << " avec contenu: {\"name\":" << jsonIn["name"].asString()<<" ,\"free\":"<<jsonIn["free"].asBool() <<"}" << endl;
        
        Json::Value jsonOut;
        HttpStatus status = service->put(jsonOut,jsonIn);
        out = jsonOut.toStyledString();
        return status;
    }
	else if (method=="PUT"){
		
        cerr << "Requête PUT " << pattern << " avec contenu: " << in << endl;
        Json::Reader jsonReader;
        Json::Value jsonIn;
        if (!jsonReader.parse(in,jsonIn))
            throw ServiceException(HttpStatus::BAD_REQUEST,"Données invalides: "+jsonReader.getFormattedErrorMessages());
        Json::Value jsonOut;
        HttpStatus status = service->put(jsonOut,jsonIn);
        out = jsonOut.toStyledString();
        return status;
	}
	
    else if (method=="DELETE" || (method == "POST" && in[0]=='D')) {
        cerr << "Requête DELETE" << endl;
        return service->remove(id);
    }
    throw ServiceException(HttpStatus::BAD_REQUEST,"Méthode "+method+" invalide");
}


