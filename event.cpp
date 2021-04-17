#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include "json.hpp"

using nlohmann::json;

using std::vector;
using std::cout;
using std::cin;
using std::stoi;
using std::string;
using std::to_string;
using std::stoi;

class Week {
  public: 
    json Mon, Tues, Wed, Thur, Fri, Sat, Sun;
};

int main(int argc, char **argv){
  string title, starttime, endtime, allday;
  int DayofWeek;
  Week testweek;
  cout << "Event Name: ";
  cin >> title;

  // 7 options
  cout << "What day of the week is it: ";
  cin >> DayofWeek;

  // two options yes or no 
  cout << "Is it an all day event? ";
  cin >> allday;

  if (allday == "NO")
  {
    cout << "When does it start: ";
    cin >> starttime;
    cout << "When does it end: ";
    cin >> endtime;
  }

  cout << "Event Created! \n";


 
  if (DayofWeek == 1){
    testweek.Mon["EventName"] = title;
  }
  if (DayofWeek == 2){
    testweek.Tues["EventName"] = title;
  }
  if (DayofWeek == 3){
    testweek.Wed["EventName"] = title;
  }
  if (DayofWeek == 4){
    testweek.Thur["EventName"] = title;
  }
  if (DayofWeek == 5){
    testweek.Fri["EventName"] = title;
  }
  if (DayofWeek == 6){
    testweek.Sat["EventName"] = title;
  }
  if (DayofWeek == 7){
    testweek.Sun["EventName"] = title;
  }
  // json j;
  // j["EventName"] = title;
  // j["DayofWeek"] = weekday;
  // j["Allday"] = true;
  // j["StartTime"] = starttime;
  // j["EndTime"] = endtime;
  std::cout <<  testweek.Mon << '\n';
}