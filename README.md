
# EmebeddedTwirtee
This Repository contain the embedded code to run in the TwIRTee Robot.

# Main programs
The TwIRTee's program contain many scripts in Python, C and Shell code, **./launch.sh** is the band master wich run scripts and compile the project in the right order.

|||||||
| :--- | :--- | :--- | :--- | :--- | :--- |
|| Map2json | jsonApi | dijkstra | kalman ||
|| mission_mgr | localization | traking | motors ||
|||||


## ./Map2json 
This folder contain a python script, to export information from a 3D map edited by Autocad (.dxf to .json). the json file is stored at **./res/map.json** and will be used by the C program to import these information.
Concretely, json file contain :
- Beacons (id,position)
- Nodes (position,neighbours)
- Constants (speed and acceleration limits)
- Legs (start and end nodes)
- Waypoints (type, position)

## jsonApi.c/.h
In order to import json information to the robot, jsonApi.c contain main functions manipulate this information and convert them to C structures wich can be used by the main program of the robot.

## Dijkstra.c/.h
 
