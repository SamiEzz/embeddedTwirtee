# EmebeddedTwirtee
This Repository contain the embedded code to run in the TwIRTee Robot.
# Main programs
## ./Map2json 
This folder contain a python script to export information from a 3D map edited by Autocad (.dxf to .json), the json file is stored at **./res** and will be used by the C program to import these information.
Concretely, json file contain :
- Beacons (id,position)
- Nodes (position,neighbours)
- Constants (speed and acceleration limits)
- Legs (start and end nodes)
- Waypoints (type, position)
