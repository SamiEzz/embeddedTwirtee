echo "|------------------------------------------|"
echo "|------" Extracting map to json file "-------|"
echo "|                                          |"
python3 ../python_tools/map2json/main_extraction.py -i ../python_tools/map2json/Map.dxf -o ./res/map.json


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
./bin/spf


echo "|------------------------------------------|"
echo "|------" End of the programme "--------------|"
echo "|                                          |"
