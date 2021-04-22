##Copyright 2021 Cole Hunter colejh@bu.edu
# This Makefile
# gives instructions to the command
# make
# about how and whether to build programs


water: water.cpp
	g++ water.cpp -o water -lsfml-graphics -lsfml-window -lsfml-system

other: other.cpp
	g++ other.cpp -o o -lsfml-graphics -lsfml-window -lsfml-system -lboost_date_time

another_other: another_other.cpp
	g++ another_other.cpp -o a -lsfml-graphics -lsfml-window -lsfml-system -lboost_date_time



timetry: timetry.cpp
	g++ timetry.cpp -o t -lsfml-graphics -lsfml-window -lsfml-system -lboost_date_time