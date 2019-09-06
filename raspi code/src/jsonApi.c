#include "../include/jsonApi.h"
#include "../include/misc.h"


char* stBeacons= "Beacons";
char* stBeaconid= "Beaconid";
char* stBeaconx= "Beaconx";
char* stBeacony= "Beacony";
char* stBeaconz= "Beaconz";
char* stnodes= "nodes";
char* stNodeId= "nodId";
char* stnodx= "nodx";
char* stnody= "nody";
char* stnodeNbA= "nodeNbA";
char* stnodeArc1= "nodeArc1";
char* stnodeArc2= "nodeArc2";
char* stnodeArc3= "nodeArc3";
char* stnodeArc4= "nodeArc4";
char* stConstants= "Constants";
char* stvdef= "VDEF";
char* stadef= "ADEF";
char* stv1= "V1";
char* stv2= "V2";
char* stConstraints= "Constraints";
char* stCid= "Cid";
char* stCvalue= "Cvalue";
char* stCname= "Cname";
char* stCtype= "Ctype";
char* stCx= "Cx";
char* stCy= "Cy";
char* stCz= "Cz";
char* stWaypoints= "Waypoints";
char* stwpid= "wpid";
char* stwptype= "wptype";
char* stwpx= "wpx";
char* stwpy= "wpy";
char* stwpz= "wpz";
char* stLegs= "Legs";
char* stlegId= "legId";
char* stlegLength= "legLength";
char* stlegStartx= "legStartx";
char* stlegStarty= "legStarty";
char* stlegStartz= "legStartz";
char* stlegEndx= "legEndx";
char* stlegEndy= "legEndy";
char* stlegEndz= "legEndz";
long int expectNvalues = 7000; // 6369
// char * jsonFileName = "map.json";
char* jsonFileName = "map.json";

// void * safe_alloc(int size){
//     errno = 0;
//     void * memblock;
//     memblock = malloc(size);
//     if(memblock==NULL){
//         (void)fprintf(stderr,"Impossible d\'allouer l\'espace dans la mémoire. \n
//         %s.",strerror(errno));
//     }
//     else{
//         return memblock;
//     }
// }

void importData(jdata* data) {
    debug_msg("jsonApi: init import data");
    data->base = safe_alloc(sizeof(Base));
    data->occur = safe_alloc(sizeof(occur));

    char* JSON_STRING = NULL;
    long length;

    FILE* f = fopen(jsonFileName, "r");
    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        JSON_STRING = safe_alloc(length);
        if (JSON_STRING) {
            fread(JSON_STRING, 1, length, f);
        }
        fclose(f);
        debug_msg("jsonApi.c : Json file imported !");

    } else {
        debug_msg("jsonApi.c : Couldn't open json file.");
    }
    initParser IP;
    IP = getJsonToken(expectNvalues, JSON_STRING);
    debug_msg("jsonApi.c : json parsed and $IP structure populated");
    int r = IP.r;
    jsmntok_t* t = IP.t;
    int i = 0;

    data->occur->beacons = objectOccurance(stBeaconid, JSON_STRING, IP);
    data->occur->nodes = objectOccurance(stNodeId, JSON_STRING, IP);
    data->occur->legs = objectOccurance(stlegId, JSON_STRING, IP);
    data->occur->Constraints = objectOccurance(stCid, JSON_STRING, IP);
    data->occur->waypoints = objectOccurance(stwpid, JSON_STRING, IP);

    debug_msg("jsonApi.c : objects counted");
    data->base->_nd = safe_alloc(sizeof(Node) * data->occur->nodes);
    data->base->_b = safe_alloc(sizeof(Node) * data->occur->beacons);
    data->base->_lg = safe_alloc(sizeof(Node) * data->occur->legs);
    data->base->_ct = safe_alloc(sizeof(Node) * data->occur->Constraints);
    data->base->_wpt = safe_alloc(sizeof(Node) * data->occur->waypoints);
    debug_msg("jsonApi.c : $data structure populated");

    for (i = 1; i < r; i++) {
        // struct Node nodes[ndoccur];
        if (jsoneq(JSON_STRING, &t[i], stLegs) == 0) {
            for (int in = 0; in < data->occur->legs; in++) {
                data->base->_lg[in] = legsExt(JSON_STRING, IP, i, in);
            }
        } else if (jsoneq(JSON_STRING, &t[i], stBeacons) == 0) {
            for (int in = 0; in < data->occur->beacons; in++) {
                data->base->_b[in] = beaconsExt(JSON_STRING, IP, i, in);
            }
        } else if (jsoneq(JSON_STRING, &t[i], stWaypoints) == 0) {
            for (int in = 0; in < data->occur->waypoints; in++) {
                data->base->_wpt[in] = waypointsExt(JSON_STRING, IP, i, in);
                // printf("%f , %f\n",Wp[in].x,Wp[in].y);
            }
        } else if (jsoneq(JSON_STRING, &t[i], stConstraints) == 0) {
            for (int in = 0; in <= data->occur->Constraints; in++) {
                data->base->_ct[in] = ConstrExt(JSON_STRING, IP, i, in);
            }
        } else if (jsoneq(JSON_STRING, &t[i], stnodes) == 0) {
            for (int in = 0; in <= data->occur->nodes; in++) {
                data->base->_nd[in] = nodesExt(JSON_STRING, IP, i, in);

                // printf("%f , %f, %d\n",nd[in].x,nd[in].y,nd[in].id);

                if (in == data->occur->nodes) {
                    for (int k = 0; k < data->occur->nodes; k++) {
                        for (int j = 0; j < data->base->_nd[k].nb_a; j++) {
                            Arc temparc;
                            temparc.dest = &data->base->_nd[data->base->_nd[k].ids[j]];
                            temparc.max_speed = -1;
                            temparc.max_speed_up = -1;
                            data->base->_nd[k].arcs[j] = temparc;
                        }
                    }
                }
            }
        } else if (jsoneq(JSON_STRING, &t[i], stConstants) == 0) {
            data->base->_c = CstExt(JSON_STRING, IP, i);
        } else {
        }
    }
}

int isNewNode(float x, float y, int size, float** base) {
    for (int i = 0; i < size + 1; i++) {
        if (x - base[0][i] < EPSILONodes && y - base[1][i] < EPSILONodes) {
            return 0;
        }
    }
    return 1;
}

int countNodes(Legs* legs, int nlegs) {
    Legs lick;
    int maxNodes = 2 * nlegs;
    // float base[2][2*nlegs];
    float** base = (float**)safe_alloc(sizeof(float*) * 2);
    base[0] = (float*)safe_alloc(sizeof(float) * maxNodes);
    base[1] = (float*)safe_alloc(sizeof(float) * maxNodes);

    float x[maxNodes], y[maxNodes];
    /*
    for (int k=0;k<nlegs;k++){
            lick = legs[k];
            printf("\n %d",lick.id);
    }
    */
    int madnodes = 0;
    int arcs[nlegs];
    for (int i = 0; i < nlegs; i++) {
        lick = legs[i];
        if (isNewNode(lick.startx, lick.starty, madnodes, base)) {
            // printf("\nnew %d",lick.id);
            base[0][madnodes] = lick.startx;
            base[1][madnodes] = lick.starty;
            madnodes++;
        }
        if (isNewNode(lick.endx, lick.endy, madnodes, base)) {
            // printf("\nnew \t %d",lick.id);
            base[0][madnodes] = lick.endx;
            base[1][madnodes] = lick.endy;
            madnodes++;
        }

        // printf("\t %d ,%d",j,lick.id);
    }
    return madnodes;
}

int jsoneq(const char* json, jsmntok_t* tok, const char* s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

struct Constants CstExt(char* _JSON_STRING, initParser _IP, int _i) {
    struct Constants C;
    char* tempchar;
    jsmntok_t* _t = _IP.t;
    // char * jsonobjectname=stBeaconid";
    int constantsrank = objectRank(stConstants, _JSON_STRING, _IP) - 1;
    int vdefrank = objectRank(stvdef, _JSON_STRING, _IP) - constantsrank;
    int adefrank = objectRank(stadef, _JSON_STRING, _IP) - constantsrank;
    int v1rank = objectRank(stv1, _JSON_STRING, _IP) - constantsrank;
    int v2rank = objectRank(stv2, _JSON_STRING, _IP) - constantsrank;
    //---
    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + vdefrank].start, sizeof(double));
    C.vdef = strtof(tempchar, NULL);
    free(tempchar);
    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + adefrank].start, sizeof(double));
    C.adef = strtof(tempchar, NULL);
    free(tempchar);
    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + v1rank].start, sizeof(double));
    C.v1 = strtof(tempchar, NULL);
    free(tempchar);
    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + v2rank].start, sizeof(double));
    C.v2 = strtof(tempchar, NULL);
    free(tempchar);
    return C;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
struct Node nodesExt(char* _JSON_STRING, initParser _IP, int _i, int objRank) {
    struct Node nd;
    char* tempchar;
    char* namechar;
    char* typechar;

    jsmntok_t* _t = _IP.t;
    // char * jsonobjectname=stBeaconid";

    int ndrank = objectRank(stnodes, _JSON_STRING, _IP) - 1;
    int tab = nextObjectTab(stNodeId, _JSON_STRING, _IP);

    int idrank = objectRank(stNodeId, _JSON_STRING, _IP) - ndrank + objRank * tab;
    int nbarank = objectRank(stnodeNbA, _JSON_STRING, _IP) - ndrank + objRank * tab;
    int typerank = 0;
    int xrank = objectRank(stnodx, _JSON_STRING, _IP) - ndrank + objRank * tab;
    int yrank = objectRank(stnody, _JSON_STRING, _IP) - ndrank + objRank * tab;
    int arc1rank = objectRank(stnodeArc1, _JSON_STRING, _IP) - ndrank + objRank * tab;
    int arc2rank = objectRank(stnodeArc2, _JSON_STRING, _IP) - ndrank + objRank * tab;
    int arc3rank = objectRank(stnodeArc3, _JSON_STRING, _IP) - ndrank + objRank * tab;
    int arc4rank = objectRank(stnodeArc4, _JSON_STRING, _IP) - ndrank + objRank * tab;

    //---
    tempchar = safe_alloc(sizeof(int));
    strncpy(tempchar, _JSON_STRING + _t[_i + idrank].start, sizeof(int));
    nd.id = atoi(tempchar);
    free(tempchar);
    //---
    // typechar = safe_alloc(14*sizeof(char));
    // strncpy(typechar,  _JSON_STRING + _t[_i+ typerank].start, 14*sizeof(char));
    nd.nt = 0;
    // free(typechar);
    //---

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + xrank].start, sizeof(double));
    nd.x = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + yrank].start, sizeof(double));
    nd.y = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(int));
    strncpy(tempchar, _JSON_STRING + _t[_i + nbarank].start, sizeof(int));
    nd.nb_a = atoi(tempchar);
    free(tempchar);

    tempchar = safe_alloc(sizeof(int));
    strncpy(tempchar, _JSON_STRING + _t[_i + arc1rank].start, sizeof(int));
    nd.ids[0] = atoi(tempchar);
    free(tempchar);

    tempchar = safe_alloc(sizeof(int));
    strncpy(tempchar, _JSON_STRING + _t[_i + arc2rank].start, sizeof(int));
    nd.ids[1] = atoi(tempchar);
    free(tempchar);

    tempchar = safe_alloc(sizeof(int));
    strncpy(tempchar, _JSON_STRING + _t[_i + arc3rank].start, sizeof(int));
    nd.ids[2] = atoi(tempchar);
    free(tempchar);

    tempchar = safe_alloc(sizeof(int));
    strncpy(tempchar, _JSON_STRING + _t[_i + arc4rank].start, sizeof(int));
    nd.ids[3] = atoi(tempchar);
    free(tempchar);

    // printf("%d , %d , %d , %d\n",nd.ids[0],nd.ids[1],nd.ids[2],nd.ids[3]);

    return nd;
}
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
struct Constraints ConstrExt(char* _JSON_STRING, initParser _IP, int _i, int objRank) {
    struct Constraints ct;
    char* tempchar;
    char* namechar;
    char* typechar;

    jsmntok_t* _t = _IP.t;
    // char * jsonobjectname=stBeaconid";

    int ctrank = objectRank(stConstraints, _JSON_STRING, _IP) - 1;
    int tab = nextObjectTab(stCid, _JSON_STRING, _IP);

    int idrank = objectRank(stCid, _JSON_STRING, _IP) - ctrank + objRank * tab;
    int valuerank = objectRank(stCvalue, _JSON_STRING, _IP) - ctrank + objRank * tab;
    int namerank = objectRank(stCname, _JSON_STRING, _IP) - ctrank + objRank * tab;
    int typerank = objectRank(stCtype, _JSON_STRING, _IP) - ctrank + objRank * tab;
    int xrank = objectRank(stCx, _JSON_STRING, _IP) - ctrank + objRank * tab;
    int yrank = objectRank(stCy, _JSON_STRING, _IP) - ctrank + objRank * tab;
    int zrank = objectRank(stCz, _JSON_STRING, _IP) - ctrank + objRank * tab;

    //---
    tempchar = safe_alloc(sizeof(int));
    strncpy(tempchar, _JSON_STRING + _t[_i + idrank].start, sizeof(int));
    ct.id = atoi(tempchar);
    free(tempchar);
    //---
    typechar = safe_alloc(14 * sizeof(char));
    strncpy(typechar, _JSON_STRING + _t[_i + typerank].start, 14 * sizeof(char));
    ct.type = typechar;
    // free(typechar);
    //---
    namechar = safe_alloc(5 * sizeof(char));
    strncpy(namechar, _JSON_STRING + _t[_i + namerank].start, 4 * sizeof(char));
    ct.name = namechar;
    // free(namechar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + valuerank].start, sizeof(double));
    ct.value = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + xrank].start, sizeof(double));
    ct.x = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + yrank].start, sizeof(double));
    ct.y = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + zrank].start, sizeof(double));
    ct.z = strtof(tempchar, NULL);
    free(tempchar);

    return ct;
}

struct Legs legsExt(char* _JSON_STRING, initParser _IP, int _i, int objRank) {
    struct Legs lg;
    char* tempchar;

    jsmntok_t* _t = _IP.t;
    // char * jsonobjectname=stBeaconid";

    int lgrank = objectRank(stLegs, _JSON_STRING, _IP) - 1;
    int tab = nextObjectTab(stlegId, _JSON_STRING, _IP);

    int idrank = objectRank(stlegId, _JSON_STRING, _IP) - lgrank + objRank * tab;
    int lengthrank = objectRank(stlegLength, _JSON_STRING, _IP) - lgrank + objRank * tab;
    int sxrank = objectRank(stlegStartx, _JSON_STRING, _IP) - lgrank + objRank * tab;
    int syrank = objectRank(stlegStarty, _JSON_STRING, _IP) - lgrank + objRank * tab;
    int szrank = objectRank(stlegStartz, _JSON_STRING, _IP) - lgrank + objRank * tab;
    int exrank = objectRank(stlegEndx, _JSON_STRING, _IP) - lgrank + objRank * tab;
    int eyrank = objectRank(stlegEndy, _JSON_STRING, _IP) - lgrank + objRank * tab;
    int ezrank = objectRank(stlegEndz, _JSON_STRING, _IP) - lgrank + objRank * tab;

    //---
    tempchar = safe_alloc(sizeof(int));
    strncpy(tempchar, _JSON_STRING + _t[_i + idrank].start, sizeof(int));
    lg.id = atoi(tempchar);
    free(tempchar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + lengthrank].start, sizeof(double));
    lg.length = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + sxrank].start, sizeof(double));
    lg.startx = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + syrank].start, sizeof(double));
    lg.starty = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + szrank].start, sizeof(double));
    lg.startz = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + exrank].start, sizeof(double));
    lg.endx = strtof(tempchar, NULL);
    free(tempchar);
    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + eyrank].start, sizeof(double));
    lg.endy = strtof(tempchar, NULL);
    free(tempchar);
    tempchar = safe_alloc(sizeof(double));
    strncpy(tempchar, _JSON_STRING + _t[_i + ezrank].start, sizeof(double));
    lg.endz = strtof(tempchar, NULL);
    free(tempchar);

    return lg;
}

struct Waypoints waypointsExt(char* _JSON_STRING, initParser _IP, int _i, int objRank) {
    struct Waypoints wp;
    char* tempchar;
    char* typechar;

    jsmntok_t* _t = _IP.t;
    int wprank = objectRank(stWaypoints, _JSON_STRING, _IP) - 1;
    int tab = nextObjectTab(stwpid, _JSON_STRING, _IP);
    int idrank = objectRank(stwpid, _JSON_STRING, _IP) - wprank + objRank * tab;
    int xrank = objectRank(stwpx, _JSON_STRING, _IP) - wprank + objRank * tab;
    int yrank = objectRank(stwpy, _JSON_STRING, _IP) - wprank + objRank * tab;
    int zrank = objectRank(stwpz, _JSON_STRING, _IP) - wprank + objRank * tab;
    int typerank = objectRank(stwptype, _JSON_STRING, _IP) - wprank + objRank * tab;

    //---
    typechar = safe_alloc(13 * sizeof(char));
    strncpy(typechar, _JSON_STRING + _t[_i + typerank].start, 13 * sizeof(char));
    wp.type = typechar;
    // free(typechar);

    tempchar = safe_alloc(sizeof(int));
    strncpy(tempchar, _JSON_STRING + _t[_i + idrank].start, sizeof(int));
    wp.id = atoi(tempchar);
    free(tempchar);

    tempchar = safe_alloc(sizeof(float));
    strncpy(tempchar, _JSON_STRING + _t[_i + xrank].start, sizeof(float));
    wp.x = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(float));
    strncpy(tempchar, _JSON_STRING + _t[_i + yrank].start, sizeof(float));
    wp.y = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(float));
    strncpy(tempchar, _JSON_STRING + _t[_i + zrank].start, sizeof(float));
    wp.z = strtof(tempchar, NULL);
    free(tempchar);

    return wp;
}

struct Beacons beaconsExt(char* _JSON_STRING, initParser _IP, int _i, int objRank) {
    struct Beacons B;
    char* tempchar;
    jsmntok_t* _t = _IP.t;
    // char * jsonobjectname=stBeaconid";
    int Beaconrank = objectRank(stBeacons, _JSON_STRING, _IP) - 1;
    int tab = nextObjectTab(stBeaconid, _JSON_STRING, _IP);
    int idrank = objectRank(stBeaconid, _JSON_STRING, _IP) - Beaconrank + objRank * tab;
    int xrank = objectRank(stBeaconx, _JSON_STRING, _IP) - Beaconrank + objRank * tab;
    int yrank = objectRank(stBeacony, _JSON_STRING, _IP) - Beaconrank + objRank * tab;
    int zrank = objectRank(stBeaconz, _JSON_STRING, _IP) - Beaconrank + objRank * tab;
    //---
    tempchar = safe_alloc(sizeof(char));
    strncpy(tempchar, _JSON_STRING + _t[_i + idrank].start, sizeof(int));
    B.id = atoi(&tempchar[0]);
    free(tempchar);

    tempchar = safe_alloc(sizeof(float));
    strncpy(tempchar, _JSON_STRING + _t[_i + xrank].start, sizeof(float));
    B.x = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(float));
    strncpy(tempchar, _JSON_STRING + _t[_i + yrank].start, sizeof(float));
    B.y = strtof(tempchar, NULL);
    free(tempchar);

    tempchar = safe_alloc(sizeof(float));
    strncpy(tempchar, _JSON_STRING + _t[_i + zrank].start, sizeof(float));
    B.z = strtof(tempchar, NULL);
    free(tempchar);

    return B;
}

struct initParser getJsonToken(int expectNvalues, char* JSON_STRING) {
    initParser IP;
    int r = 0;
    jsmn_parser p;
    // jsmntok_t  t[expectNvalues];
    /* We expect no more than 128 tokens
    Debugger error : Value requires 128000bytes, more than max-value-size
    */
    IP.t = (jsmntok_t*)safe_alloc(sizeof(jsmntok_t) *
                                  expectNvalues); /* We expect no more than 128 tokens */

    IP.r = 0;
    //	IP.t=t;
    jsmn_init(&IP.p);
    // IP.p=p;
    IP.r = jsmn_parse(&IP.p, JSON_STRING, strlen(JSON_STRING), IP.t, expectNvalues);

    if (IP.r < 0) {
        printf("Failed to parse JSON: %d\n", r);
        // break;
    }

    /* Assume the top-level element is an object */
    if (IP.r < 1 || IP.t[0].type != JSMN_OBJECT) {
        printf("Object expected\n");
        // break;
    }

    return IP;
}

int objectRank(char* objectName, char* JSON_STRING, initParser IP) {
    int rank;
    jsmntok_t* t = IP.t;
    int r = IP.r;
    int i = 0;
    for (i = 1; i < r; i++) {
        if (jsoneq(JSON_STRING, &t[i], objectName) == 0) {
            return i;
        }
    }
    return -1;
}

int objectOccurance(char* objectName, char* JSON_STRING, initParser IP) {
    int occurance = 0;
    jsmntok_t* t = IP.t;
    int r = IP.r;
    for (int i = 1; i < r; i++) {
        if (jsoneq(JSON_STRING, &t[i], objectName) == 0) {
            occurance++;
        }
    }
    return occurance;
}

int nextObjectTab(char* objectName, char* JSON_STRING, initParser IP) {
    int tab = 0;
    int lasti = -1;
    jsmntok_t* t = IP.t;
    int r = IP.r;
    for (int i = 1; i < r; i++) {
        if (jsoneq(JSON_STRING, &t[i], objectName) == 0) {
            if (lasti == -1) {
                lasti = i;
            } else {
                tab = i - lasti;
                return tab;
            }
        }
    }
}
//*************************************************************************************************

/**
 * Allocates a fresh unused token from the token pull.
 */
static jsmntok_t* jsmn_alloc_token(jsmn_parser* parser, jsmntok_t* tokens, size_t num_tokens) {
    jsmntok_t* tok;
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
static void jsmn_fill_token(jsmntok_t* token, jsmntype_t type, int start, int end) {
    token->type = type;
    token->start = start;
    token->end = end;
    token->size = 0;
}

/**
 * Fills next available token with JSON primitive.
 */
static int jsmn_parse_primitive(jsmn_parser* parser, const char* js, size_t len, jsmntok_t* tokens,
                                size_t num_tokens) {
    jsmntok_t* token;
    int start;

    start = parser->pos;

    for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
        switch (js[parser->pos]) {
#ifndef JSMN_STRICT
        /* In strict mode primitive must be followed by "," or "}" or "]" */
        case ':':
#endif
        case '\t':
        case '\r':
        case '\n':
        case ' ':
        case ',':
        case ']':
        case '}':
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
static int jsmn_parse_string(jsmn_parser* parser, const char* js, size_t len, jsmntok_t* tokens,
                             size_t num_tokens) {
    jsmntok_t* token;

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
            jsmn_fill_token(token, JSMN_STRING, start + 1, parser->pos);
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
            case '\"':
            case '/':
            case '\\':
            case 'b':
            case 'f':
            case 'r':
            case 'n':
            case 't':
                break;
            /* Allows escaped symbol \uXXXX */
            case 'u':
                parser->pos++;
                for (i = 0; i < 4 && parser->pos < len && js[parser->pos] != '\0'; i++) {
                    /* If it isn't a hex character we have an error */
                    if (!((js[parser->pos] >= 48 && js[parser->pos] <= 57) ||   /* 0-9 */
                          (js[parser->pos] >= 65 && js[parser->pos] <= 70) ||   /* A-F */
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
int jsmn_parse(jsmn_parser* parser, const char* js, size_t len, jsmntok_t* tokens,
               unsigned int num_tokens) {
    int r;
    int i;
    jsmntok_t* token;
    int count = parser->toknext;

    for (; parser->pos < len && js[parser->pos] != '\0'; parser->pos++) {
        char c;
        jsmntype_t type;

        c = js[parser->pos];
        switch (c) {
        case '{':
        case '[':
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
        case '}':
        case ']':
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
                    if (token->type != type || parser->toksuper == -1) {
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
            if (i == -1)
                return JSMN_ERROR_INVAL;
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
            if (r < 0)
                return r;
            count++;
            if (parser->toksuper != -1 && tokens != NULL)
                tokens[parser->toksuper].size++;
            break;
        case '\t':
        case '\r':
        case '\n':
        case ' ':
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
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 't':
        case 'f':
        case 'n':
            /* And they must not be keys of the object */
            if (tokens != NULL && parser->toksuper != -1) {
                jsmntok_t* t = &tokens[parser->toksuper];
                if (t->type == JSMN_OBJECT || (t->type == JSMN_STRING && t->size != 0)) {
                    return JSMN_ERROR_INVAL;
                }
            }
#else
        /* In non-strict mode every unquoted value is a primitive */
        default:
#endif
            r = jsmn_parse_primitive(parser, js, len, tokens, num_tokens);
            if (r < 0)
                return r;
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
void jsmn_init(jsmn_parser* parser) {
    parser->pos = 0;
    parser->toknext = 0;
    parser->toksuper = -1;
}


Node getnodebyid(jdata* data, int id) {
    return *(data->base->_nd + id);
}

void printnode(Node* n) {
    printf("\t|\n\t|\n--------------------\nid \t %d\nx \t %f\ny \t %f\n--------------------\n",
           n->id, n->x, n->y);
}
void traject_to_file(Node* n, int rank, char* outputFile) {

    // HEAD
    char* mode = "a";
    FILE* fp;
    if (rank == 0) {
        mode = "w";
        fp = fopen(outputFile, mode);

        fprintf(fp, "id, x, y\n");
        fprintf(fp, "%d, %f, %f\n", n->id, n->x, n->y);
    } else {
        // BODY
        fp = fopen(outputFile, mode);
        fprintf(fp, "%d, %f, %f\n", n->id, n->x, n->y);
    }
    // FOOTER
    fclose(fp);

    // // HEAD
    // char * mode="a";
    // FILE * fp;
    // if(rank==0){
    // 	mode="w";
    // 	fp = fopen ("trajectory.json", mode);
    // 	fprintf(fp, "{\"nodes_id\" : [");
    // 	fprintf(fp, "%d",n->id);
    // }
    // else{
    // 	// BODY
    // 	fp = fopen ("trajectory.json", mode);
    // 	fprintf(fp, "%d",n->id);
    // }

    // if(rank<max-1){
    // 	fprintf(fp, ",");
    // }
    // // FOOTER
    // if(rank==max-1){
    // 	fprintf(fp, "]}");
    // 	fclose(fp);

    // }
}

Beacons getbeaconbyid(jdata* data, int id) {
    return *(data->base->_b + id);
}

Waypoints getwaypointbyid(jdata* data, int id) {
    return *(data->base->_wpt + id);
}

Constraints getconstraintbyid(jdata* data, int id) {
    return *(data->base->_ct + id);
}

Constants getconstants(jdata* data) {
    return (data->base->_c);
}