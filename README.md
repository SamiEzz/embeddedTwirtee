
# EmebeddedTwirtee
This Repository contain the embedded code to run in the TwIRTee Robot.

# Main programs
The TwIRTee's program contain many scripts in Python, C and Shell code, **./launch.sh** is the band master wich run scripts and compile the project in the right order.

|||||||
| :--- | :--- | :--- | :--- | :--- | :--- |
|| Map2json | jsonApi | dijkstra | kalman ||
|| mission_mgr | localization | traking | motors ||
|||||


## 1) ./Map2json 
This folder contain a python script, to export information from a 3D map edited by Autocad (.dxf to .json). the json file is stored at **./res/map.json** and will be used by the C program to import these information.
Concretely, json file contain :
- Beacons (id,position)
- Nodes (position,neighbours)
- Constants (speed and acceleration limits)
- Legs (start and end nodes)
- Waypoints (type, position)

## 2) jsonApi.c/.h
In order to import json information to the robot, jsonApi.c contain main functions manipulate this information and convert them to C structures wich can be used by the main program of the robot.
### Introduction :octocat:
This program give you several functions to read a json file containing map information.
Below the main functions that you will use :
#### Main functions
| Return type | Function(parameters) |
| :--- | :--- |
| struct Beacons |beaconsExt(char *_JSON_STRING,initParser _IP,int _i,int objRank);|
|struct Waypoints |waypointsExt(char *_JSON_STRING,initParser _IP,int _i,int objRank);|
|struct Legs |legsExt(char *_JSON_STRING,initParser _IP,int _i,int objRank);|
|struct Constraints |ConstrExt(char *_JSON_STRING,initParser _IP,int _i,int objRank);|
|struct Constants |CstExt(char *_JSON_STRING,initParser _IP,int _i);|

#### Parameters


| Parameter | Purpose |
| :--- | :--- |
| char *_JSON_STRING | This is a pointer to a memory bloc that contain the json file. using "fread()" |
| initParser _IP | initiated by "getJsonToken()",  
| | **_IP.r** : number of objects parsed  |
| | **_IP.t** : token pointer to the address of each object in **JSON_STRING**   |
| | **_IP.p** : json parser :: not used for now |
| int _i | represent rank of json object inside the token **t** "t[_i]"  |
| int objRank | used to populate the same structure in a **for** loop |


### How to use :
1. include "**header.h**" and "**jsmn.h**"
2. make sure that you fill this variables :
* int **expectNvalues** 		: more than 6000
* char * **jsonFileName** : can't be more clear 
* char * **JSON_STRING** :  pointer to a memory bloc (Cf. Extra instructions [JSON_STRING](#json_string-))
* initParser **IP** = getJsonToken(expectNvalues,JSON_STRING)  
* int **r**  = IP.r
* jsmntok * **t** = IP.t

3. Compute occurance of objects:
	Cf. Annexe.(Compute Occurance)
4. Create structures
	Cf. Annexe.(Create Structs)
5. loop over all json objects and fill structures Cf. Annexe.(Populate structures loop)



### Annexe

#### JSON_STRING :
 to fill this bloc, use this clasic olgorithme :
  
    FILE *f =  fopen(jsonFileName, "r");
    if (f){
	    fseek(f,0,SEEK_END);
	    length=  ftell(f);
	    fseek(f,0,SEEK_SET);
	    JSON_STRING =  malloc(length);
	    if (JSON_STRING){
		    fread(JSON_STRING,1,length,f);
	    }
	    fclose(f);
    };
#### Compute Occurance
	int Boccur =  objectOccurance(stBeaconid,JSON_STRING, IP);    
	int Wpoccur =  objectOccurance(stwpid,JSON_STRING, IP);
	int lgoccur =  objectOccurance(stlegId,JSON_STRING, IP); 
	int ctoccur =  objectOccurance(stCid,JSON_STRING, IP);
#### Create structs

    struct Beacons B[Boccur];
    struct Waypoints Wp[Wpoccur];
    struct Legs lg[lgoccur];
    struct Constraints ct[ctoccur];
    struct Constants c;
    
#### Populate structures loop :
this code check all objects of the map.json file and fill the corresponding structure   
   
    for (i =  1; i < r; i++) {
	    if (jsoneq(JSON_STRING, &t[i], stLegs) ==  0) {
		    for(int in=0;in<lgoccur;in++){
			    lg[in]=legsExt(JSON_STRING,IP,i,in);
		    }
	    }
	    else  if (jsoneq(JSON_STRING, &t[i], stBeacons) ==  0) {
		    for(int in=0;in<Boccur;in++){
			    B[in]=beaconsExt(JSON_STRING,IP,i,in);
		    }
	    }
	    else  if (jsoneq(JSON_STRING, &t[i], stWaypoints) ==  0) {
		    for(int in=0;in<Wpoccur;in++){
			    Wp[in]=waypointsExt(JSON_STRING,IP,i,in);
		    }
	    }
	    else  if (jsoneq(JSON_STRING, &t[i], stConstraints) ==  0) {
		    for(int in=0;in<=ctoccur;in++){
			    ct[in]=ConstrExt(JSON_STRING,IP,i,in);
		    }
	    }
	    else  if (jsoneq(JSON_STRING, &t[i], stConstants) ==  0) {
		    c =  CstExt(JSON_STRING,IP,i);
	    }
	    else{}
    }




## 3) Dijkstra.c/.h
 
