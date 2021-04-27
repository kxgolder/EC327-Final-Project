##Copyright 2021 Cole Hunter colejh@bu.edu
# This Makefile
# gives instructions to the command
# make
# about how and whether to build programs

wellness_app: wellness_app.cpp
	g++ wellness_app.cpp -o wa -lsfml-graphics -lsfml-window -lsfml-system -lboost_date_time

working_dates: working_dates.cpp
	g++ working_dates.cpp -o w -lsfml-graphics -lsfml-window -lsfml-system -lboost_date_time

try: try.cpp
	g++ try.cpp -o t -lsfml-graphics -lsfml-window -lsfml-system -lboost_date_time

