#include "../include/jsonApi.h"

/****************************--[main.c file]--****************************

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


	// Loop over all keys of the root object 
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
        
        
        else if (jsoneq(JSON_STRING, &t[i], stnodes) == 0) {
			for(int in=0;in<=ndoccur;in++){
				nd[in]=nodesExt(JSON_STRING,IP,i,in);
                
                //printf("%f , %f, %d\n",nd[in].x,nd[in].y,nd[in].id);
                
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
    	return EXIT_SUCCESS;
}
/****************************--[main.c end]--****************************

*/

jdata * importData(jdata *data){
	data = malloc(sizeof(jdata));
	data->base=malloc(sizeof(Base));
	data->occur=malloc(sizeof(occur));

	char *eJSON_STRING = NULL;
	long length;
	

	FILE *f = fopen(jsonFileName, "r");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		eJSON_STRING = malloc(length);
		if (eJSON_STRING)
		{
			fread(eJSON_STRING, 1, length, f);
		}
		fclose(f);
	};
	initParser IP;
	IP = getJsonToken(expectNvalues, eJSON_STRING);

	int r = IP.r;
	jsmntok_t *t = IP.t;
	int i = 0;

	data->occur->beacons=objectOccurance(stBeaconid, eJSON_STRING, IP);
	data->occur->nodes=objectOccurance(stNodeId, eJSON_STRING, IP);
	data->occur->legs=objectOccurance(stlegId, eJSON_STRING, IP);
	data->occur->Constraints=objectOccurance(stCid, eJSON_STRING, IP);
	data->occur->waypoints=objectOccurance(stwpid, eJSON_STRING, IP);

	data->base->_nd=malloc(sizeof(Node)*data->occur->nodes);
	data->base->_b=malloc(sizeof(Node)*data->occur->beacons);
	data->base->_lg=malloc(sizeof(Node)*data->occur->legs);
	data->base->_ct=malloc(sizeof(Node)*data->occur->Constraints);
	data->base->_wpt=malloc(sizeof(Node)*data->occur->waypoints);
	


	return data;
}
int expectNvalues=8000;

int len(char *t)
{
    return expectNvalues;
}

struct Base initBase()
{
	char *eJSON_STRING = NULL;
	long length;
	

	FILE *f = fopen(jsonFileName, "r");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		eJSON_STRING = malloc(length);
		if (eJSON_STRING)
		{
			fread(eJSON_STRING, 1, length, f);
		}
		fclose(f);
	};
	initParser IP;
	IP = getJsonToken(expectNvalues, eJSON_STRING);

	int r = IP.r;
	jsmntok_t *t = IP.t;
	int i = 0;

	int Boccur = objectOccurance(stBeaconid, eJSON_STRING, IP); // verifier occurrence
	int Wpoccur = objectOccurance(stwpid, eJSON_STRING, IP);
	int lgoccur = objectOccurance(stlegId, eJSON_STRING, IP);
	int ctoccur = objectOccurance(stCid, eJSON_STRING, IP);
	int ndoccur = objectOccurance(stNodeId, eJSON_STRING, IP);
	struct Beacons B[Boccur];
	struct Node nd[ndoccur];
	//struct Node *nodes[ndoccur];
	struct Waypoints Wp[Wpoccur];
	struct Legs lg[lgoccur];
	struct Constraints ct[ctoccur];
	struct Constants c;
	
	struct Base Bs = {
        nd,
        B,
        Wp,
        lg,
        ct,
		c       
    };
/*
	Bs._b[]=malloc(Boccur*sizeof(Beacons));
	Bs._nd=nd;
	Bs._wpt=Wp;
	Bs._lg=lg;
	Bs._ct=ct;
*/
	return Bs;


}
void jsonMainExtract(Base _base,char *jsonFile)
{	
	struct Base *base = &_base;
	char *JSON_STRING = NULL;
	long length;
	FILE *f;
	f = fopen(jsonFile, "r");
	
	if (f!=NULL)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		JSON_STRING = malloc(length);
		if (JSON_STRING)
		{
			fread(JSON_STRING, 1, length, f);
		}
		fclose(f);
	};
	initParser IP;
	IP = getJsonToken(expectNvalues, JSON_STRING);

	int r = IP.r;
	jsmntok_t *t = IP.t;
	int i = 0;
	// Loop over all keys of the root object
	int Boccur = objectOccurance(stBeaconid, JSON_STRING, IP); // verifier occurrence
	int Wpoccur = objectOccurance(stwpid, JSON_STRING, IP);
	int lgoccur = objectOccurance(stlegId, JSON_STRING, IP);
	int ctoccur = objectOccurance(stCid, JSON_STRING, IP);
	int ndoccur = objectOccurance(stNodeId, JSON_STRING, IP);

	int fstbrank = objectRank(stBeaconid, JSON_STRING, IP);

	struct Beacons B[Boccur];
	struct Node nd[ndoccur];
	//struct Node *nodes[ndoccur];
	struct Waypoints Wp[Wpoccur];
	struct Legs lg[lgoccur];
	struct Constraints ct[ctoccur];
	struct Constants c;/*
	Base *base=malloc(sizeof(Base)*expectNvalues);
	base->_b=malloc(Boccur*sizeof(Beacons));
	base->_nd=malloc(ndoccur*sizeof(Node));
	base->_wpt=malloc(Wpoccur*sizeof(Waypoints));
	base->_lg=malloc(lgoccur*sizeof(Legs));
	base->_ct=malloc(ctoccur*sizeof(Constraints));
	*/
	for (i = 1; i < r; i++)
	{
		struct Node nodes[ndoccur];
		if (jsoneq(JSON_STRING, &t[i], stLegs) == 0)
		{
			for (int in = 0; in < lgoccur; in++)
			{
				base->_lg[in] = legsExt(JSON_STRING, IP, i, in);
			}
		}
		else if (jsoneq(JSON_STRING, &t[i], stBeacons) == 0)
		{
			for (int in = 0; in < Boccur; in++)
			{
				base->_b[in] = beaconsExt(JSON_STRING, IP, i, in);
			}
		}
		else if (jsoneq(JSON_STRING, &t[i], stWaypoints) == 0)
		{
			for (int in = 0; in < Wpoccur; in++)
			{
				base->_wpt[in] = waypointsExt(JSON_STRING, IP, i, in);
				//printf("%f , %f\n",Wp[in].x,Wp[in].y);
			}
		}
		else if (jsoneq(JSON_STRING, &t[i], stConstraints) == 0)
		{
			for (int in = 0; in <= ctoccur; in++)
			{
				base->_ct[in] = ConstrExt(JSON_STRING, IP, i, in);
			}
		}
		else if (jsoneq(JSON_STRING, &t[i], stnodes) == 0)
		{
			for (int in = 0; in <= ndoccur; in++)
			{
				base->_nd[in] = nodesExt(JSON_STRING, IP, i, in);

				//printf("%f , %f, %d\n",nd[in].x,nd[in].y,nd[in].id);

				if (in == ndoccur)
				{
					for (int k = 0; k < ndoccur; k++)
					{
						for (int j = 0; j < base->_nd[k].nb_a; j++)
						{
							Arc temparc;
							temparc.dest = &base->_nd[nd[k].ids[j]];
							temparc.max_speed = -1;
							temparc.max_speed_up = -1;
							base->_nd[k].arcs[j] = temparc;
						}
					}
				}
			}
		}
		else if (jsoneq(JSON_STRING, &t[i], stConstants) == 0)
		{
			base->_c = CstExt(JSON_STRING, IP, i);
		}
		else
		{
		}
	} /*
	Beacons **_B,
	Node **_nd,
	Waypoints **_wp,
	Legs **_lg,
	Constraints **_ct,
	Constants *_c		
*/

	//_base=base;
}



struct Node * existNode(float x,float y,Node n[],int madeNodes){
	for(int i=0;i<madeNodes;i++){
		if(n[i].x==x && n[i].y==y){
			return &n[i];
		}
	}
	return NULL;
}



int isNewNode(float x,float y,int size,float **base){
	for(int i=0;i<size+1;i++){
		if(x==base[0][i] && y==base[1][i]){
			return 0;
		}
	}
	return 1;
}
int countNodes(Legs * legs,int nlegs){
	Legs lick;
	int maxNodes=2*nlegs;
	//float base[2][2*nlegs];
	float **base = (float **)malloc(sizeof(float*)*2);
	base[0]=(float*)malloc(sizeof(float)*maxNodes);
	base[1]=(float*)malloc(sizeof(float)*maxNodes);
	 
	float x[maxNodes],y[maxNodes];
	/*
	for (int k=0;k<nlegs;k++){
		lick = legs[k];
		printf("\n %d",lick.id);
	}
	*/
	int madnodes=0;
	int arcs[nlegs];
	for(int i=0;i<nlegs;i++){
		lick = legs[i];
		if(isNewNode(lick.startx,lick.starty,madnodes,base)){
			//printf("\nnew %d",lick.id);
			base[0][madnodes]=lick.startx;
			base[1][madnodes]=lick.starty;
			madnodes++;
		}
		if(isNewNode(lick.endx,lick.endy,madnodes,base)){
			//printf("\nnew \t %d",lick.id);
			base[0][madnodes]=lick.endx;
			base[1][madnodes]=lick.endy;
			madnodes++;
		}

		//printf("\t %d ,%d",j,lick.id);

	}
	return madnodes;
}
int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

struct Constants CstExt(char *_JSON_STRING,initParser _IP,int _i){
		struct Constants C;
		char * tempchar;
		jsmntok_t *_t = _IP.t;
		//char * jsonobjectname=stBeaconid";
		int constantsrank = objectRank(stConstants,_JSON_STRING, _IP)-1;
		int vdefrank=objectRank(stvdef,_JSON_STRING, _IP)-constantsrank;
		int adefrank=objectRank(stadef,_JSON_STRING, _IP)-constantsrank;
		int v1rank=objectRank(stv1,_JSON_STRING, _IP)-constantsrank;
		int v2rank=objectRank(stv2,_JSON_STRING, _IP)-constantsrank;
		//---
		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ vdefrank].start, sizeof(double));
		C.vdef=strtof(tempchar,NULL);
		free(tempchar);
		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ adefrank].start, sizeof(double));
		C.adef=strtof(tempchar,NULL);
		free(tempchar);
		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ v1rank].start, sizeof(double));
		C.v1=strtof(tempchar,NULL);
		free(tempchar);
		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ v2rank].start, sizeof(double));
		C.v2=strtof(tempchar,NULL);
		free(tempchar);
		return C;
  }
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
struct Node nodesExt(char *_JSON_STRING,initParser _IP,int _i,int objRank){
		struct Node nd;
		char * tempchar;
		char * namechar;
		char * typechar;
		
		
		jsmntok_t *_t = _IP.t;
		//char * jsonobjectname=stBeaconid";
		
		int ndrank = objectRank(stnodes,_JSON_STRING, _IP)-1;
		int tab=nextObjectTab(stNodeId,_JSON_STRING, _IP);

		int idrank= objectRank(stNodeId,_JSON_STRING, _IP)-ndrank+objRank*tab;
		int nbarank=objectRank(stnodeNbA,_JSON_STRING, _IP)-ndrank+objRank*tab;
		int typerank=0;
		int xrank=objectRank(stnodx,_JSON_STRING, _IP)-ndrank+objRank*tab;
		int yrank=objectRank(stnody,_JSON_STRING, _IP)-ndrank+objRank*tab;
		int arc1rank=objectRank(stnodeArc1,_JSON_STRING, _IP)-ndrank+objRank*tab;
		int arc2rank=objectRank(stnodeArc2,_JSON_STRING, _IP)-ndrank+objRank*tab;
		int arc3rank=objectRank(stnodeArc3,_JSON_STRING, _IP)-ndrank+objRank*tab;
		int arc4rank=objectRank(stnodeArc4,_JSON_STRING, _IP)-ndrank+objRank*tab;
		
		
		//---
		tempchar = malloc(sizeof(int));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ idrank].start, sizeof(int));
		nd.id=atoi(tempchar);
		free(tempchar);
		//---
		//typechar = malloc(14*sizeof(char));
		//strncpy(typechar,  _JSON_STRING + _t[_i+ typerank].start, 14*sizeof(char));
		nd.nt=0;
		//free(typechar);
		//---
		

		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ xrank].start, sizeof(double));
		nd.x=strtof(tempchar,NULL);
		free(tempchar);

		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ yrank].start, sizeof(double));
		nd.y=strtof(tempchar,NULL);
		free(tempchar);

		tempchar = malloc(sizeof(int));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ nbarank].start, sizeof(int));
		nd.nb_a=atoi(tempchar);
		free(tempchar);


		tempchar = malloc(sizeof(int));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ arc1rank].start, sizeof(int));
		nd.ids[0]=atoi(tempchar);
		free(tempchar);

		tempchar = malloc(sizeof(int));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ arc2rank].start, sizeof(int));
		nd.ids[1]=atoi(tempchar);
		free(tempchar);

		tempchar = malloc(sizeof(int));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ arc3rank].start, sizeof(int));
		nd.ids[2]=atoi(tempchar);
		free(tempchar);


		tempchar = malloc(sizeof(int));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ arc4rank].start, sizeof(int));
		nd.ids[3]=atoi(tempchar);
		free(tempchar);

		
		
		//printf("%d , %d , %d , %d\n",nd.ids[0],nd.ids[1],nd.ids[2],nd.ids[3]);
		
	return nd;


  }
  //------------------------------------------------------------------------------------------
  //------------------------------------------------------------------------------------------
struct Constraints ConstrExt(char *_JSON_STRING,initParser _IP,int _i,int objRank){
		struct Constraints ct;
		char * tempchar;
		char * namechar;
		char * typechar;
		
		
		jsmntok_t *_t = _IP.t;
		//char * jsonobjectname=stBeaconid";

		int ctrank = objectRank(stConstraints,_JSON_STRING, _IP)-1;
		int tab=nextObjectTab(stCid,_JSON_STRING, _IP);

		int idrank=objectRank(stCid,_JSON_STRING, _IP)-ctrank+objRank*tab;
		int valuerank=objectRank(stCvalue,_JSON_STRING, _IP)-ctrank+objRank*tab;
		int namerank=objectRank(stCname,_JSON_STRING, _IP)-ctrank+objRank*tab;
		int typerank=objectRank(stCtype,_JSON_STRING, _IP)-ctrank+objRank*tab;
		int xrank=objectRank(stCx,_JSON_STRING, _IP)-ctrank+objRank*tab;
		int yrank=objectRank(stCy,_JSON_STRING, _IP)-ctrank+objRank*tab;
		int zrank=objectRank(stCz,_JSON_STRING, _IP)-ctrank+objRank*tab;
		
		//---
		tempchar = malloc(sizeof(int));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ idrank].start, sizeof(int));
		ct.id=atoi(tempchar);
		free(tempchar);
		//---
		typechar = malloc(14*sizeof(char));
		strncpy(typechar,  _JSON_STRING + _t[_i+ typerank].start, 14*sizeof(char));
		ct.type=typechar;
		//free(typechar);
		//---
		namechar = malloc(5*sizeof(char));
		strncpy(namechar,  _JSON_STRING + _t[_i+ namerank].start, 4*sizeof(char));
		ct.name=namechar;
		//free(namechar);

		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ valuerank].start, sizeof(double));
		ct.value=strtof(tempchar,NULL);
		free(tempchar);

		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ xrank].start, sizeof(double));
		ct.x=strtof(tempchar,NULL);
		free(tempchar);

		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ yrank].start, sizeof(double));
		ct.y=strtof(tempchar,NULL);
		free(tempchar);

		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ zrank].start, sizeof(double));
		ct.z=strtof(tempchar,NULL);
		free(tempchar);

		
		
		return ct;
  }

struct Legs legsExt(char *_JSON_STRING,initParser _IP,int _i,int objRank){
		struct Legs lg;
		char * tempchar;
		
		jsmntok_t *_t = _IP.t;
		//char * jsonobjectname=stBeaconid";

		int lgrank = objectRank(stLegs,_JSON_STRING, _IP)-1;
		int tab=nextObjectTab(stlegId,_JSON_STRING, _IP);

		int idrank=objectRank(stlegId,_JSON_STRING, _IP)-lgrank+objRank*tab;
		int lengthrank=objectRank(stlegLength,_JSON_STRING, _IP)-lgrank+objRank*tab;
		int sxrank=objectRank(stlegStartx,_JSON_STRING, _IP)-lgrank+objRank*tab;
		int syrank=objectRank(stlegStarty,_JSON_STRING, _IP)-lgrank+objRank*tab;
		int szrank=objectRank(stlegStartz,_JSON_STRING, _IP)-lgrank+objRank*tab;
		int exrank=objectRank(stlegEndx,_JSON_STRING, _IP)-lgrank+objRank*tab;
		int eyrank=objectRank(stlegEndy,_JSON_STRING, _IP)-lgrank+objRank*tab;
		int ezrank=objectRank(stlegEndz,_JSON_STRING, _IP)-lgrank+objRank*tab;
				
		//---
		tempchar = malloc(sizeof(int));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ idrank].start, sizeof(int));
		lg.id=atoi(tempchar);
		free(tempchar);

		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ lengthrank].start, sizeof(double));
		lg.length=strtof(tempchar,NULL);
		free(tempchar);
		
		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ sxrank].start, sizeof(double));
		lg.startx=strtof(tempchar,NULL);
		free(tempchar);

		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ syrank].start, sizeof(double));
		lg.starty=strtof(tempchar,NULL);
		free(tempchar);

		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ szrank].start, sizeof(double));
		lg.startz=strtof(tempchar,NULL);
		free(tempchar);
		
		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ exrank].start, sizeof(double));
		lg.endx=strtof(tempchar,NULL);
		free(tempchar);
		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ eyrank].start, sizeof(double));
		lg.endy=strtof(tempchar,NULL);
		free(tempchar);
		tempchar = malloc(sizeof(double));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ ezrank].start, sizeof(double));
		lg.endz=strtof(tempchar,NULL);
		free(tempchar);
		
		
		return lg;
  }

struct Waypoints waypointsExt(char *_JSON_STRING,initParser _IP,int _i,int objRank){
		struct Waypoints wp;
		char * tempchar;
		char * typechar;
		
		jsmntok_t *_t = _IP.t;
		int wprank = objectRank(stWaypoints,_JSON_STRING, _IP)-1;
		int tab=nextObjectTab(stwpid,_JSON_STRING, _IP);
		int idrank=objectRank(stwpid,_JSON_STRING, _IP)-wprank+objRank*tab;
		int xrank=objectRank(stwpx,_JSON_STRING, _IP)-wprank+objRank*tab;
		int yrank=objectRank(stwpy,_JSON_STRING, _IP)-wprank+objRank*tab;
		int zrank=objectRank(stwpz,_JSON_STRING, _IP)-wprank+objRank*tab;
		int typerank=objectRank(stwptype,_JSON_STRING, _IP)-wprank+objRank*tab;
		
		//---
		typechar = malloc(13*sizeof(char));
		strncpy(typechar,  _JSON_STRING + _t[_i+ typerank].start, 13*sizeof(char));
		wp.type=typechar;
		//free(typechar);

		tempchar = malloc(sizeof(int));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ idrank].start, sizeof(int));
		wp.id=atoi(tempchar);
		free(tempchar);
		
		tempchar = malloc(sizeof(float));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ xrank].start, sizeof(float));
		wp.x=strtof(tempchar,NULL);
		free(tempchar);
		
		tempchar = malloc(sizeof(float));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ yrank].start, sizeof(float));
		wp.y=strtof(tempchar,NULL);
		free(tempchar);
		
		tempchar = malloc(sizeof(float));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ zrank].start, sizeof(float));
		wp.z=strtof(tempchar,NULL);
		free(tempchar);
		
		return wp;
  }

struct Beacons beaconsExt(char *_JSON_STRING,initParser _IP,int _i,int objRank){
		struct Beacons B;
		char * tempchar;
		jsmntok_t *_t = _IP.t;
		//char * jsonobjectname=stBeaconid";
		int Beaconrank = objectRank(stBeacons,_JSON_STRING, _IP)-1;
		int tab=nextObjectTab(stBeaconid,_JSON_STRING, _IP);
		int idrank=objectRank(stBeaconid,_JSON_STRING, _IP)-Beaconrank+objRank*tab;
		int xrank=objectRank(stBeaconx,_JSON_STRING, _IP)-Beaconrank+objRank*tab;
		int yrank=objectRank(stBeacony,_JSON_STRING, _IP)-Beaconrank+objRank*tab;
		int zrank=objectRank(stBeaconz,_JSON_STRING, _IP)-Beaconrank+objRank*tab;
		//---
		tempchar = malloc(sizeof(char));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ idrank].start, sizeof(int));
		B.id=atoi(&tempchar[0]);
		free(tempchar);

		tempchar = malloc(sizeof(float));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ xrank].start, sizeof(float));
		B.x=strtof(tempchar,NULL);
		free(tempchar);
		
		tempchar = malloc(sizeof(float));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ yrank].start, sizeof(float));
		B.y=strtof(tempchar,NULL);
		free(tempchar);
		
		tempchar = malloc(sizeof(float));
		strncpy(tempchar,  _JSON_STRING + _t[_i+ zrank].start, sizeof(float));
		B.z=strtof(tempchar,NULL);
		free(tempchar);
		
		return B;
  }


struct initParser getJsonToken(int expectNvalues,char * JSON_STRING){
	initParser IP;
	int r;
	jsmn_parser p;
	jsmntok_t  t[expectNvalues]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, r);
    IP.r=r;
	IP.p=p;
	IP.t=t;
	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		//return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		//return 1;
	}
    
    return IP;
}

int objectRank(char * objectName,char * JSON_STRING, initParser IP){
	int rank;
	jsmntok_t * t = IP.t;
	int r = IP.r;
	int i=0;
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], objectName) == 0) {
			return i;
		}
	}
	return -1;
}

int objectOccurance(char * objectName,char * JSON_STRING, initParser IP){
	int occurance=0;
	jsmntok_t * t = IP.t;
	int r = IP.r;
	for (int i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], objectName) == 0) {
			occurance++;
		}
	}
	return occurance;
}

int nextObjectTab(char * objectName,char * JSON_STRING, initParser IP){
	int tab=0;
	int lasti=-1;
	jsmntok_t * t = IP.t;
	int r = IP.r;
	for (int i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], objectName) == 0) {
			if(lasti==-1){
				lasti=i;
			}
			else{
				tab=i-lasti;
				return tab;
			}
		}
	}
}
//*************************************************************************************************

/**
 * Allocates a fresh unused token from the token pull.
 */
static jsmntok_t *jsmn_alloc_token(jsmn_parser *parser,
		jsmntok_t *tokens, size_t num_tokens) {
	jsmntok_t *tok;
	if (parser->toknext >= num_tokens) {
		return NULL;
	}
	tok = &tokens[parser->toknext++];
	tok->start = tok->end = -1;
	tok->size = 0;
#ifdef JSMN_PARENT_LINKS
	tok->parent = -1;
#endif
	return tok;
}

/**
 * Fills token type and boundaries.
 */
static void jsmn_fill_token(jsmntok_t *token, jsmntype_t type,
                            int start, int end) {
	token->type = type;
	token->start = start;
	token->end = end;
	token->size = 0;
}

/**
 * Fills next available token with JSON primitive.
 */
static int jsmn_parse_primitive(jsmn_parser *parser, const char *js,
		size_t len, jsmntok_t *tokens, size_t num_tokens) {
	jsmntok_t *token;
	int start;

	start = parser->pos;

	for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
		switch (js[parser->pos]) {
#ifndef JSMN_STRICT
			/* In strict mode primitive must be followed by "," or "}" or "]" */
			case ':':
#endif
			case '\t' : case '\r' : case '\n' : case ' ' :
			case ','  : case ']'  : case '}' :
				goto found;
		}
		if (js[parser->pos] < 32 || js[parser->pos] >= 127) {
			parser->pos = start;
			return JSMN_ERROR_INVAL;
		}
	}
#ifdef JSMN_STRICT
	/* In strict mode primitive must be followed by a comma/object/array */
	parser->pos = start;
	return JSMN_ERROR_PART;
#endif

found:
	if (tokens == NULL) {
		parser->pos--;
		return 0;
	}
	token = jsmn_alloc_token(parser, tokens, num_tokens);
	if (token == NULL) {
		parser->pos = start;
		return JSMN_ERROR_NOMEM;
	}
	jsmn_fill_token(token, JSMN_PRIMITIVE, start, parser->pos);
#ifdef JSMN_PARENT_LINKS
	token->parent = parser->toksuper;
#endif
	parser->pos--;
	return 0;
}

/**
 * Fills next token with JSON string.
 */
static int jsmn_parse_string(jsmn_parser *parser, const char *js,
		size_t len, jsmntok_t *tokens, size_t num_tokens) {
	jsmntok_t *token;

	int start = parser->pos;

	parser->pos++;

	/* Skip starting quote */
	for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
		char c = js[parser->pos];

		/* Quote: end of string */
		if (c == '\"') {
			if (tokens == NULL) {
				return 0;
			}
			token = jsmn_alloc_token(parser, tokens, num_tokens);
			if (token == NULL) {
				parser->pos = start;
				return JSMN_ERROR_NOMEM;
			}
			jsmn_fill_token(token, JSMN_STRING, start+1, parser->pos);
#ifdef JSMN_PARENT_LINKS
			token->parent = parser->toksuper;
#endif
			return 0;
		}

		/* Backslash: Quoted symbol expected */
		if (c == '\\' && parser->pos + 1 < len) {
			int i;
			parser->pos++;
			switch (js[parser->pos]) {
				/* Allowed escaped symbols */
				case '\"': case '/' : case '\\' : case 'b' :
				case 'f' : case 'r' : case 'n'  : case 't' :
					break;
				/* Allows escaped symbol \uXXXX */
				case 'u':
					parser->pos++;
					for(i = 0; i < 4 && parser->pos < len && js[parser->pos] != '\0'; i++) {
						/* If it isn't a hex character we have an error */
						if(!((js[parser->pos] >= 48 && js[parser->pos] <= 57) || /* 0-9 */
									(js[parser->pos] >= 65 && js[parser->pos] <= 70) || /* A-F */
									(js[parser->pos] >= 97 && js[parser->pos] <= 102))) { /* a-f */
							parser->pos = start;
							return JSMN_ERROR_INVAL;
						}
						parser->pos++;
					}
					parser->pos--;
					break;
				/* Unexpected symbol */
				default:
					parser->pos = start;
					return JSMN_ERROR_INVAL;
			}
		}
	}
	parser->pos = start;
	return JSMN_ERROR_PART;
}

/**
 * Parse JSON string and fill tokens.
 */
int jsmn_parse(jsmn_parser *parser, const char *js, size_t len,jsmntok_t *tokens, unsigned int num_tokens) {
	int r;
	int i;
	jsmntok_t *token;
	int count = parser->toknext;

	for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
		char c;
		jsmntype_t type;

		c = js[parser->pos];
		switch (c) {
			case '{': case '[':
				count++;
				if (tokens == NULL) {
					break;
				}
				token = jsmn_alloc_token(parser, tokens, num_tokens);
				if (token == NULL)
					return JSMN_ERROR_NOMEM;
				if (parser->toksuper != -1) {
					tokens[parser->toksuper].size++;
#ifdef JSMN_PARENT_LINKS
					token->parent = parser->toksuper;
#endif
				}
				token->type = (c == '{' ? JSMN_OBJECT : JSMN_ARRAY);
				token->start = parser->pos;
				parser->toksuper = parser->toknext - 1;
				break;
			case '}': case ']':
				if (tokens == NULL)
					break;
				type = (c == '}' ? JSMN_OBJECT : JSMN_ARRAY);
#ifdef JSMN_PARENT_LINKS
				if (parser->toknext < 1) {
					return JSMN_ERROR_INVAL;
				}
				token = &tokens[parser->toknext - 1];
				for (;;) {
					if (token->start != -1 && token->end == -1) {
						if (token->type != type) {
							return JSMN_ERROR_INVAL;
						}
						token->end = parser->pos + 1;
						parser->toksuper = token->parent;
						break;
					}
					if (token->parent == -1) {
						if(token->type != type || parser->toksuper == -1) {
							return JSMN_ERROR_INVAL;
						}
						break;
					}
					token = &tokens[token->parent];
				}
#else
				for (i = parser->toknext - 1; i >= 0; i--) {
					token = &tokens[i];
					if (token->start != -1 && token->end == -1) {
						if (token->type != type) {
							return JSMN_ERROR_INVAL;
						}
						parser->toksuper = -1;
						token->end = parser->pos + 1;
						break;
					}
				}
				/* Error if unmatched closing bracket */
				if (i == -1) return JSMN_ERROR_INVAL;
				for (; i >= 0; i--) {
					token = &tokens[i];
					if (token->start != -1 && token->end == -1) {
						parser->toksuper = i;
						break;
					}
				}
#endif
				break;
			case '\"':
				r = jsmn_parse_string(parser, js, len, tokens, num_tokens);
				if (r < 0) return r;
				count++;
				if (parser->toksuper != -1 && tokens != NULL)
					tokens[parser->toksuper].size++;
				break;
			case '\t' : case '\r' : case '\n' : case ' ':
				break;
			case ':':
				parser->toksuper = parser->toknext - 1;
				break;
			case ',':
				if (tokens != NULL && parser->toksuper != -1 &&
						tokens[parser->toksuper].type != JSMN_ARRAY &&
						tokens[parser->toksuper].type != JSMN_OBJECT) {
#ifdef JSMN_PARENT_LINKS
					parser->toksuper = tokens[parser->toksuper].parent;
#else
					for (i = parser->toknext - 1; i >= 0; i--) {
						if (tokens[i].type == JSMN_ARRAY || tokens[i].type == JSMN_OBJECT) {
							if (tokens[i].start != -1 && tokens[i].end == -1) {
								parser->toksuper = i;
								break;
							}
						}
					}
#endif
				}
				break;
#ifdef JSMN_STRICT
			/* In strict mode primitives are: numbers and booleans */
			case '-': case '0': case '1' : case '2': case '3' : case '4':
			case '5': case '6': case '7' : case '8': case '9':
			case 't': case 'f': case 'n' :
				/* And they must not be keys of the object */
				if (tokens != NULL && parser->toksuper != -1) {
					jsmntok_t *t = &tokens[parser->toksuper];
					if (t->type == JSMN_OBJECT ||
							(t->type == JSMN_STRING && t->size != 0)) {
						return JSMN_ERROR_INVAL;
					}
				}
#else
			/* In non-strict mode every unquoted value is a primitive */
			default:
#endif
				r = jsmn_parse_primitive(parser, js, len, tokens, num_tokens);
				if (r < 0) return r;
				count++;
				if (parser->toksuper != -1 && tokens != NULL)
					tokens[parser->toksuper].size++;
				break;

#ifdef JSMN_STRICT
			/* Unexpected char in strict mode */
			default:
				return JSMN_ERROR_INVAL;
#endif
		}
	}

	if (tokens != NULL) {
		for (i = parser->toknext - 1; i >= 0; i--) {
			/* Unmatched opened object or array */
			if (tokens[i].start != -1 && tokens[i].end == -1) {
				return JSMN_ERROR_PART;
			}
		}
	}

	return count;
}

/**
 * Creates a new parser based over a given  buffer with an array of tokens
 * available.
 */
void jsmn_init(jsmn_parser *parser) {
	parser->pos = 0;
	parser->toknext = 0;
	parser->toksuper = -1;
}

