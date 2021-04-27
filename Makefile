##Copyright 2021 Cole Hunter colejh@bu.edu
# This Makefile
# gives instructions to the command
# make
# about how and whether to build programs

wellness_app: wellness_app.cpp
	g++ wellness_app.cpp -o wa -lsfml-graphics -lsfml-window -lsfml-system -lboost_date_time


