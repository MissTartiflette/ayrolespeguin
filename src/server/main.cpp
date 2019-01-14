#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>
#include <map>
#include <memory>
#include <unistd.h>
#include <microhttpd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h> 
#define SOCKET_ERROR -1

#include "state.h"
#include "render.h"
#include "engine.h"
#include "ai.h"
#include "server.h"

using namespace std;
using namespace state;
using namespace render;
using namespace engine;
using namespace ai;
using namespace server;

class Request {
public:
    struct MHD_PostProcessor *pp = nullptr;
    string data;
    ~Request() {
        if (pp) MHD_destroy_post_processor (pp);
    }
};

static int post_iterator(void *cls,
        enum MHD_ValueKind kind,
        const char *key,
        const char *filename,
        const char *content_type,
        const char *transfer_encoding,
        const char *data, uint64_t off, size_t size) 
{
    return MHD_NO;
}

static int
main_handler (void *cls,      
          struct MHD_Connection *connection,
          const char *url, // 
          const char *method,
          const char *version,
          const char *upload_data, size_t *upload_data_size, void **ptr) 
{
    // Données pour une requête (en plusieurs appels à cette fonction)
    Request *request = (Request*)*ptr;

    // Premier appel pour cette requête
    if (!request) { 
        request = new Request();
        if (!request) {
            return MHD_NO;
        }
        *ptr = request;
        if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
         || strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
            request->pp = MHD_create_post_processor(connection,1024,&post_iterator,request);
            if (!request->pp) {
                cerr << "Failed to setup post processor for " << url << endl;
                return MHD_NO;
            }
        }
        return MHD_YES;
    }    
    
    // Cas où il faut récupérer les données envoyés par l'utilisateur
    if (strcmp(method, MHD_HTTP_METHOD_POST) == 0
     || strcmp(method, MHD_HTTP_METHOD_PUT) == 0) {
        MHD_post_process(request->pp,upload_data,*upload_data_size);
        if (*upload_data_size != 0) {
            request->data = upload_data;
            *upload_data_size = 0;
            return MHD_YES;
        }    
    }

    HttpStatus status;
    string response;
    try {

        ServicesManager *manager = (ServicesManager*) cls;
        status = manager->queryService(response,request->data,url,method);
    }
    catch(ServiceException& e) {
        status = e.status();
        response = e.what();
        response += "\n";
    }
    catch(exception& e) {
        status = HttpStatus::SERVER_ERROR;
        response = e.what();
        response += "\n";
    }
    catch(...) {
        status = HttpStatus::SERVER_ERROR;
        response = "Unknown exception\n";
    }

    struct MHD_Response *mhd_response;
    mhd_response = MHD_create_response_from_buffer(response.size(),(void *)response.c_str(),MHD_RESPMEM_MUST_COPY);
    if (strcmp(method,MHD_HTTP_METHOD_GET) == 0) {
        MHD_add_response_header(mhd_response,"Content-Type","application/json; charset=utf-8");
    }
    int ret = MHD_queue_response(connection, status, mhd_response);
    MHD_destroy_response(mhd_response);
    return ret;
}

static void request_completed (void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe){
  Request *request = (Request*)*con_cls;
  if (request) {
      delete request;
      *con_cls = nullptr;
  }
}

int main(int argc,char* argv[]){

	if (argc>1){
		/*	hello : Affichage simple */
		if(strcmp(argv[1],"hello")==0){
			cout<<"Bonjour tout le monde"<<endl;
			Json::Value root;
			root["tabCmd"];
			
			/*
			cout << "Taille root : " << root["tabCmd"].size() << endl;
			root["tabCmd"][0]["id"] =  200;
			cout << root << endl;
			*/
		}
		if(strcmp(argv[1],"test")==0){
			//VersionService versionService;
			//std::unique_ptr<AbstractService> ptr_versionService (new VersionService(versionService));

			ServicesManager servicesManager;
			//servicesManager.registerService(move(ptr_versionService));
			Player new_player("Paul", true);
			std::unique_ptr<Player> ptr_player (new Player(new_player));
			Game game;
			game.addPlayer(move(ptr_player));

			PlayerService playerService(std::ref(game));
			std::unique_ptr<AbstractService> ptr_playerService (new PlayerService(playerService));

			servicesManager.registerService(move(ptr_playerService));

			//const string url="http://localhost:8080/player/1";
			const string url="http://localhost:8080/player/1";
			const string method="GET";
			string out="";
			string in="";
			HttpStatus h=servicesManager.queryService (out, in, url, method);
			cout<<h<<endl;
		}
		else if(strcmp(argv[1], "listen") == 0){
			struct sockaddr_in addr, addr2;
			int sock;
			if(( sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
				printf("error socket\n");
				return -1;
			}
			int j=1;
			setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &j, sizeof(j));
			addr.sin_family = AF_UNIX;
			addr.sin_port = 8086;
			int bi, li;
			if((bi=bind(sock, (struct sockaddr*) &addr, sizeof(struct sockaddr_in)))<0) {
				perror("bind");
			}
			else{cout<<"bi : "<<bi<<endl;}
			if((li=listen(sock, 5)) ==-1){
				perror("listen");
			}
			else{cout<<"li : "<<li <<endl;}
			char buffer[32] = "Bonjour !";
			char buffer2[32]="";
			if(li!=SOCKET_ERROR){
				socklen_t size=sizeof(struct sockaddr_in);
				int csock = accept(sock, (struct sockaddr*)&addr2, &size);
				int sock_err = send(csock, buffer, 32, 0);
				if(sock_err != SOCKET_ERROR){
                        cout<<"Chaine envoyée : "<<buffer<<endl;
						if(recv(csock, buffer2, 32, 0) != SOCKET_ERROR){
                			cout<<"Recu : "<<buffer2<<endl;
							
						}
						else{
		        			cout<<"Impossible de se connecter"<<endl;
		    			}
 				}
                else{
                        cout<<"Erreur de transmission"<<endl;
				}
 
                    /* Il ne faut pas oublier de fermer la connexion (fermée dans les deux sens) */
                shutdown(csock, 2);
			}
			cout<<"Fermeture de la socket"<<endl;
            close(sock);
            cout<<"Fermeture du serveur terminee"<<endl;

			
			/* On attend que l'utilisateur tape sur une touche, puis on ferme */
   			getchar();
			return EXIT_SUCCESS;
			/*while(1){
			socklen_t size=sizeof(struct sockaddr_in);
			int cfd = accept(sock, (struct sockaddr*)&addr2, &size);
			cout<<"truc"<<endl;
			//if(cfd==-1){perror("accept");}
			//else{cout<<"ac : " <<cfd<<endl;}
			
			
			/*char buf[100];
			if(read(sock, (void*)&buf, 100)==-1){
					perror("read");
			}
			cout<<buf<<endl;

			if(write(sock, (void*)&buf, 100)==-1){
					perror("error write");
			}
			cout<<buf<<endl;
			
			if(fork()) { 
				shutdown(cfd, SHUT_RDWR);
				close(cfd); 
				continue;
				
				//shutdown(sock, SHUT_RDWR);
				//close(sock);
			} 
			else{ 
				shutdown(sock, SHUT_RDWR);
				close(sock); 
				
			
			char buffer[1024];
			int n = 0;
			sleep(3);
			if((n = recv(sock, buffer, sizeof buffer - 1, 0)) < 0){
   				perror("recv()");
    			exit(errno);
			}

			buffer[n] = '\0';
			//shutdown(cfd, SHUT_RDWR);
			//close(cfd);
			}
			shutdown(sock, SHUT_RDWR);
			shutdown(cfd, SHUT_RDWR);
			close(sock);
			close(cfd);
			}*/
			
			/*try {

				VersionService versionService;
				std::unique_ptr<AbstractService> ptr_versionService (new VersionService(versionService));

				ServicesManager servicesManager;
				//servicesManager.registerService(make_unique<VersionService>());
				servicesManager.registerService(move(ptr_versionService));

				Player new_player("Paul", true);
				std::unique_ptr<Player> ptr_player (new Player(new_player));
				Game game;
				game.addPlayer(move(ptr_player));

				PlayerService playerService(std::ref(game));
				std::unique_ptr<AbstractService> ptr_playerService (new PlayerService(playerService));

				servicesManager.registerService(move(ptr_playerService));

				struct MHD_Daemon *d;
				if (argc != 2) {
				    printf("%s PORT\n", argv[0]);
				    return 1;
				}
				d = MHD_start_daemon(// MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG | MHD_USE_POLL,
				        MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG,
				        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG | MHD_USE_POLL,
				        // MHD_USE_THREAD_PER_CONNECTION | MHD_USE_DEBUG,
				        8080,
				      	NULL, NULL, 
				        &main_handler, (void*) &servicesManager,
				        MHD_OPTION_NOTIFY_COMPLETED, request_completed, NULL,
				        MHD_OPTION_END);
				if (d == NULL)
					return 1;
				cout << "Pressez <entrée> pour arrêter le serveur" << endl;
				(void) getc(stdin);
				MHD_stop_daemon(d);
   			 }
			catch(exception& e) {
				cerr << "Exception: " << e.what() << endl;
			}*/
    
		}
		
		/*	record : Enregistrement d'une partie jouée par deux IA*/
		else if(strcmp(argv[1], "record") == 0){
			
			unsigned int longueur_map_cases = 25, largeur_map_cases = 25;
			std::string chemin_fichier_map_txt = "res/map1.txt";
			std::string fichier_commandes = "res/replay.txt";
			
			// Creation des tables de correspondances et du moteur
			Correspondances tab_corres = Correspondances();
			Moteur moteur;
			moteur.setEnableRecord(true);
			
			if(	moteur.getEtat().initGrille(chemin_fichier_map_txt, longueur_map_cases, largeur_map_cases, tab_corres)){
				// Initialisations de démarrage et création des IA
				moteur.getEtat().initPersonnages(tab_corres);
				moteur.getEtat().initCurseur();				
				HeuristicIA armeeRouge;
				HeuristicIA armeeBleue;
				armeeBleue.setCamp(true);
				
				cout << "\t\t--- Record ---" << endl;				
				cout << "On enregistre trois tours de jeu dans le fichier " << fichier_commandes << endl;
				cout << "Deux IA heuristiques jouent l'une contre l'autre" << endl;
				sleep(2);
				cout << "--> Début de l'enregistrement <--" << endl;
				
				// On simule 3 tours
				while (moteur.getEtat().getTour() != 4){
					// L'armée bleue joue les tours impairs, la rouge les tours pairs
					if(moteur.getEtat().getTour()%2 != 0){
						armeeBleue.run(moteur);
					}
					else{
						armeeRouge.run(moteur);
					}
					
					// Verifications de fin de tours pour changer le tour
					if(!moteur.getEtat().getFin() && moteur.verificationFinDeTour()){
							moteur.verificationDebutDeTour();
					}
				}
				
				cout << "--> Fin de l'enregistrement <--" << endl;
				
				
				cout << "--> Debut de l'enregistrement dans le fichier <--" << endl;
				// Ouverture du fichier en ecriture en effacant son contenu à l'ouverture
				std::ofstream fichier_ecriture(fichier_commandes, ios::out|ios::trunc);
				if(fichier_ecriture){
									
					Json::Value root = moteur.getRecord();
					cout << root << endl;
					
					// Ecriture dans le fichier du tableau de commandes de cette partie
					fichier_ecriture << root;
					
					// Fermeture du fichier
					fichier_ecriture.close();
					
					cout << "--> Fin de l'enregistrement dans le fichier <--" << endl;
				}
				else{
					cerr << "Impossible d'ouvrir le fichier des commandes enregistrées pour l'ecriture" << endl;
				}
			}
		}
	}
	
    return 0;
}
