#include <iostream>
#include <string.h>

using namespace std;

int main(int argc,char* argv[]){
	if (argc>1){
		if (strcmp(argv[1],"hello")==0){
			cout << "Bonjour tout le monde" << endl;
		}
		else{
			exit(EXIT_FAILURE);
		}
	}	

    return 0;
}
