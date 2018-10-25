#include "state.h"

using namespace state;

Correspondances::Correspondances(){
	correspondances_TP[0] 	= MAISON ;
	correspondances_TP[1] 	= MAISON;
	correspondances_TP[2] 	= FORTERESSE;
	correspondances_TP[3] 	= FORTERESSE;
	correspondances_TP[10] 	= COLLINE;
	correspondances_TP[11] 	= COLLINE;
	correspondances_TP[12] 	= COLLINE;
	correspondances_TP[13] 	= COLLINE;
	correspondances_TP[16] 	= PONT;
	correspondances_TP[17] 	= PONT;
	correspondances_TP[18] 	= PLAINE;
	correspondances_TP[19] 	= PLAINE;
	correspondances_TP[20] 	= PLAINE;
	correspondances_TP[21] 	= PLAINE;
	correspondances_TP[22] 	= PLAINE;
	correspondances_TP[23] 	= FORET;
	correspondances_TP[24] 	= FORET;
	correspondances_TP[25] 	= FORET;
	correspondances_TP[26] 	= FORET;
	
	correspondances_TNP[4] 	= MUR;
	correspondances_TNP[5] 	= MUR;
	correspondances_TNP[6] 	= MUR;
	correspondances_TNP[7] 	= MUR;
	correspondances_TNP[8] 	= MUR;
	correspondances_TNP[9] 	= MUR;
	correspondances_TNP[14] = MONTAGNE;
	correspondances_TNP[15] = MONTAGNE;
	correspondances_TNP[27] = RIVIERE;
	correspondances_TNP[28] = RIVIERE;
	correspondances_TNP[29] = RIVIERE;
	correspondances_TNP[30] = RIVIERE;
	correspondances_TNP[31] = RIVIERE;
	correspondances_TNP[32] = RIVIERE;
	correspondances_TNP[33] = RIVIERE;
	correspondances_TNP[34] = RIVIERE;
	correspondances_TNP[35] = RIVIERE;
	correspondances_TNP[36] = RIVIERE;
	correspondances_TNP[37] = RIVIERE;
	correspondances_TNP[38] = RIVIERE;
	correspondances_TNP[39] = RIVIERE;
	correspondances_TNP[40] = RIVIERE;
	correspondances_TNP[41] = FALAISE;
	correspondances_TNP[42] = FALAISE;
	correspondances_TNP[43] = FALAISE;
	correspondances_TNP[44] = FALAISE;
	correspondances_TNP[45] = FALAISE;
	correspondances_TNP[46] = FALAISE;
	
	correspondances_personnage[3] 	= CHEVALIER;
	correspondances_personnage[7] 	= CHEVALIER;
	correspondances_personnage[11] 	= CHEVALIER;
	correspondances_personnage[0] 	= ARCHER;
	correspondances_personnage[4] 	= ARCHER;
	correspondances_personnage[8] 	= ARCHER;
	correspondances_personnage[2] 	= GUERRIER;
	correspondances_personnage[6] 	= GUERRIER;
	correspondances_personnage[10] 	= GUERRIER;
	correspondances_personnage[1] 	= BRIGAND;
	correspondances_personnage[5] 	= BRIGAND;
	correspondances_personnage[9] 	= BRIGAND;	

}

std::map<int, TerrainPraticableID>& Correspondances::getCorrespondanceTP (){
	std::map<int, TerrainPraticableID> & refCorrespondancesTP = correspondances_TP;
	return refCorrespondancesTP;
}

std::map<int, TerrainNonPraticableID>& Correspondances::getCorrespondanceTNP (){
	std::map<int, TerrainNonPraticableID> & refCorrepondancesTNP = correspondances_TNP;
	return refCorrepondancesTNP;
}

std::map<int, TypePersonnageID>& Correspondances::getCorrespondancePersonnage (){
	std::map<int, TypePersonnageID> & refCorrepondancesPersonnage = correspondances_personnage;
	return refCorrepondancesPersonnage;
}
