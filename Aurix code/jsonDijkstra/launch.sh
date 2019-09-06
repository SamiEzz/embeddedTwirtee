echo "|------------------------------------------|"
echo "|------" Extracting map to json file "-------|"
echo "|                                          |"
python3 ./map2json/main_extraction.py -i ./map2json/Map.dxf -o ./res/map.json


echo "|------------------------------------------|"
echo "|------" Compiling C files "-----------------|"
echo "|                                          |"
make


echo "|------------------------------------------|"
echo "|------" Initiating devices "----------------|"
echo "|                                          |"
#./bin/init.exe


echo "|------------------------------------------|"
echo "|------" Launching main routine "------------|"
echo "|                                          |"
./bin/mainRoutine


echo "|------------------------------------------|"
echo "|------" End of the programme "--------------|"
echo "|                                          |"
