/*! \mainpage My Personal Index Page
*
* \section intro_sec Introduction
*
* This is the introduction.
*
* \section install_sec Installation
*
* \subsection step1 Step 1: Opening the box
*
* etc...
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/jsmn.h"
#include "../include/header.h"
#include "../include/dijkstra.h"



char *jsonFileName="base.json";

char *JSON_STRING = NULL;
long length;
long expectNvalues=8000;

int flag=0;

int main(int argc, char const *argv[])
{
    /**
     * @brief Converter
     * quelque chose
     */

    //jsonFileName = argv[1];
    
	FILE *f = fopen(jsonFileName, "r");
	if (f){
		fseek(f,0,SEEK_END);
		length= ftell(f);
		fseek(f,0,SEEK_SET);
		JSON_STRING = malloc(length);
		if (JSON_STRING){
			fread(JSON_STRING,1,length,f);
		}
		fclose(f);
  	};
	initParser IP;
	IP = getJsonToken(expectNvalues,JSON_STRING);

	int r=IP.r;
	jsmntok_t *t=IP.t;
	int i=0;


	/* Loop over all keys of the root object */
	int Boccur 	= objectOccurance(stBeaconid,JSON_STRING, IP); // verifier occurrence
	int Wpoccur	= objectOccurance(stwpid,JSON_STRING, IP);
	int lgoccur = objectOccurance(stlegId,JSON_STRING, IP);
	int ctoccur = objectOccurance(stCid,JSON_STRING, IP);
    int ndoccur = objectOccurance(stNodeId,JSON_STRING, IP);
	
	int fstbrank = objectRank(stBeaconid,JSON_STRING, IP);
	


	struct Beacons B[Boccur];
    struct Node nd[ndoccur];
    struct Node *nodes[ndoccur];
	struct Waypoints Wp[Wpoccur];
	struct Legs lg[lgoccur];
	struct Constraints ct[ctoccur];
	struct Constants c;

	for (i = 1; i < r; i++) {
        struct Node nodes[ndoccur];
		if (jsoneq(JSON_STRING, &t[i], stLegs) == 0) {
			for(int in=0;in<lgoccur;in++){
				lg[in]=legsExt(JSON_STRING,IP,i,in);
			}
		}
		else if (jsoneq(JSON_STRING, &t[i], stBeacons) == 0) {
			for(int in=0;in<Boccur;in++){
				B[in]=beaconsExt(JSON_STRING,IP,i,in);
			}
		}
		else if (jsoneq(JSON_STRING, &t[i], stWaypoints) == 0) {
			for(int in=0;in<Wpoccur;in++){
				Wp[in]=waypointsExt(JSON_STRING,IP,i,in);
                //printf("%f , %f\n",Wp[in].x,Wp[in].y);
			}
		}
		else if (jsoneq(JSON_STRING, &t[i], stConstraints) == 0) {
			for(int in=0;in<=ctoccur;in++){
				ct[in]=ConstrExt(JSON_STRING,IP,i,in);
			}
		}
        //------------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------------
/*        TODO : 
            nodejson : arc1,arc2,arc3,arc4 au lieu de Arcs[]
            use nb_arc to extract
*/
        
        else if (jsoneq(JSON_STRING, &t[i], stnodes) == 0) {
			for(int in=0;in<=ndoccur;in++){
				nd[in]=nodesExt(JSON_STRING,IP,i,in);
                
                //printf("%f , %f, %d\n",nd[in].x,nd[in].y,nd[in].id);
                /*
                TODO: add speed limits to arcs in nodes
                */
                if(in==ndoccur){
                    for(int k=0;k<ndoccur;k++){
                        for(int j=0;j<nd[k].nb_a;j++){
                            Arc temparc;
                            temparc.dest = &nd[nd[k].ids[j]];
                            temparc.max_speed=-1;
                            temparc.max_speed_up=-1;
                            nd[k].arcs[j]=temparc;
                        }
                    }
                }
			}
		}

        //------------------------------------------------------------------------------------------
        //------------------------------------------------------------------------------------------


        
		else if (jsoneq(JSON_STRING, &t[i], stConstants) == 0) {
			c = CstExt(JSON_STRING,IP,i);
		}
		
		else{}
		
		
	}

    //---------------[TEST DIJKSTRA]---------------
    Cartography graphtest;
    Path trajectorytest;
    int src=0;
    int dest=125;

    graphtest.def_max_speed=0.3;
    graphtest.def_max_speed_up=0.5;
    graphtest.nb_nodes=ndoccur;
    graphtest.nb_nodes=ndoccur;
    
    for(int i=0;i<ndoccur;i++){
        graphtest.nodes[i]=nd[i];
    }
    for(int k=0;k<ndoccur;k++){
        graphtest.nb_arcs+=graphtest.nodes[k].nb_a;
        //printf("\nnb_arcs : %d\n",graphtest.nodes[k].nb_a);
    }
    //printf("\nnb_arcs : %d\n",graphtest.nb_arcs);
    

    dijkstra(&graphtest, src, dest, &trajectorytest);

    for(int i=0;i<trajectorytest.size;i++){
        printf("%d\n",trajectorytest.dest[i]->id);
    }
    //---------------[====================]---------------

    /**
     * @brief test functions
     * 
     
    int k = countNodes(lg, lgoccur);
    //printf(" %d\n",k);
    
    struct Node * N;
    N = convertLegs(lg,lgoccur);
    //printf("\nx\t\t y\t\t\t nbArc\n");
    //printf("x, y\n");
    /*
    for(int i =0;i<k;i++){
        //printf("%f\t %f\t\t %d \n",N[i].x,N[i].y,N[i].nb_a);
        printf("%f, %f\n",N[i].x,N[i].y);
	}*/
    

    /*
    printf("\n__________________________________________________________");
    //printf("\n\n(0)\t: Copyrights");
    printf("\n(1)\t: Occurance of objects");
    printf("\n(2)\t: Get Beacons");
    printf("\n(3)\t: Get Waypoints");
    printf("\n(4)\t: Get Legs");
    printf("\n(5)\t: Get Constraints");
    printf("\n(6)\t: Get Constants\n ");
    printf("\nEnter your choice\t: ");
    int user;
    scanf("%d",&user);
    system("clear");
    switch(user){
        case 99:
            printf("\nWelcome to TwIRTee mapManager API interface \n\t\tAuthor\t: S. Ezzerouali\n\t\tTutor\t: E. Jenn\n\t\t");
        break;
        case 1:
            printf("\nNumber of Beacons \t: %d",Boccur);
            printf("\nNumber of waypoints \t: %d",Wpoccur);
            printf("\nNumber of Legs \t\t: %d",lgoccur);
            printf("\nNumber of Constraints \t: %d",ctoccur);
            printf("\nNumber of Constants \t: %d\n",4);
        break;

        case 2:
            for(int ja=0;ja<Boccur;ja++){
                printf("Beacon [%d] : \n id : %d\n x  : %f\n y  : %f\n z  : %f\n",ja,B[ja].id,B[ja].x,B[ja].y,B[ja].z);
            }
        break;
            
        case 3:
            for(int ja=0;ja<Wpoccur;ja++){
                printf("Waypoints [%d] : ",ja);
                printf("\ntype  : %s",Wp[ja].type);		
                printf("\n x  : %f\n y  : %f\n z  : %f\n",Wp[ja].x,Wp[ja].y,Wp[ja].z);
                
            }
        break;
        case 4:
            for(int ja=0;ja<lgoccur;ja++){
                printf("Leg [%d] : \n id : %d\n length : %f\n startx  : %f\n starty  : %f\n startz  : %f \n endx  : %f\n endy  : %f \n endz  : %f\n",
                    ja,lg[ja].id,lg[ja].length,lg[ja].startx,lg[ja].starty,lg[ja].startz,lg[ja].endx,lg[ja].endy,lg[ja].endz);
            }
        break;
        case 5:
            for(int ind=0;ind<ctoccur;ind++){
                printf("\nConstraint [%d]\n value \t:%f \n x \t:%f \n y \t:%f \n z \t:%f",ct[ind].id,ct[ind].value,ct[ind].x,ct[ind].y,ct[ind].z);
                printf("\n name \t: %.*s",4,ct[ind].name);
                printf("\n type \t: %.*s \n",13,ct[ind].type);
            }
        break;
        case 6:
            printf("\nConstants :\n Vdef\t: %.4f\n Adef\t: %.4f\n V1\t: %.4f\n V2\t: %f\n",c.vdef,c.adef,c.v1,c.v2);
        break;
        default:
            printf("\n ! \t Bad input\nPlease enter a number between 1 -- 6\n\n");
        break;
    }
    system("./bin/api.exe");
    */
	return EXIT_SUCCESS;
}