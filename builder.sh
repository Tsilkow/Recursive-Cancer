#!/bin/bash

#rm -rf build/
#mkdir build/
#cp Sicretmono.ttf build/Sicretmono.ttf
#cp station.png build/station.png
#cp scores.json build/scores.json
cp -r data/ build/data
clear
cd build/
cmake ..
if eval cmake --build .; then
    if [ "$#" -ge 1 ]; then
	exec "./RecursiveCancer" "$1"
    else
	exec "./RecursiveCancer"
    fi
    cd ..
else
    cd ..
fi
    
