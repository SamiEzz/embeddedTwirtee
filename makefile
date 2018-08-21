# *. Bandmaster
# *. Localization	
# *. Odometrie		({from device}---{Odo->x,Odo->y})
# *. IMU			({from device}---{IMU->x,IMU->y})
# *. DWM gps		({from device}---{dwm->x,dwm->y})
# *. DWM gps		({from device}---{dwm->x,dwm->y})
# *. kalman			({dwm,Odo,IMU}---{})
# *. mission_mgr	({Nodes,carto,source,destination,*path}---{*path})
# *. Dijkstra
CC		:= gcc
C_FLAGS := -Wall -Wextra

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
else
EXECUTABLE	:= main
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	-$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*
	$(CC) $(C_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES)