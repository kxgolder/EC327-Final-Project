##Copyright 2021 Cole Hunter colejh@bu.edu
# This Makefile
# gives instructions to the command
# make
# about how and whether to build programs


working_dates: working_dates.cpp
	g++ working_dates.cpp -o w -lsfml-graphics -lsfml-window -lsfml-system -lboost_date_time

