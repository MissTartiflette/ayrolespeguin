#include "server.h"
#include <iostream>
using namespace server;
using namespace std;

//engine::Moteur& moteur;
  
CommandService::CommandService (engine::Moteur& moteur): AbstractService("/command"), moteur(moteur){
	listeCommand["tailleCmdAdverse"]=0;
	listeCommand["tabCmdAdverse"][0]="";
}

HttpStatus CommandService::get (Json::Value& out, int id) const{

	/*for(size_t i=0; i<listeCommand.size();i++){
		if(id!=listeCommand[i]["joueur"].asInt()){
			out["tabCmdAdverse"][out["tailleCmdAdverse"].asInt()]=listeCommand[i];
			out["tailleCmdAdverse"]=out["tailleCmdAdverse"].asInt()+1;
		}
	}
	*/
	out=listeCommand;
	/*Json::Value new_listeCommand;
	//new_listeCommand["tailleCmdAdverse"]=0;
	//new_listeCommand["tabCmdAdverse"][0]="";
    listeCommand=new_listeCommand;*/
	/*listeCommand["tailleCmdAdverse"]=0;
	listeCommand["tabCmdAdverse"][0]="";*/
	return HttpStatus::OK;

}

HttpStatus CommandService::put (Json::Value& out, const Json::Value& in){
	listeCommand["tabCmdAdverse"][listeCommand["tailleCmdAdverse"].asInt()]=in;
 	listeCommand["tailleCmdAdverse"]=listeCommand["tailleCmdAdverse"].asInt()+1;
	
	return HttpStatus::NO_CONTENT;
	
}

bool CommandService::isVersion(){
	return false;
}
