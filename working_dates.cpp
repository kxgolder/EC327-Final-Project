#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <locale>
#include "Button.h"
#include <sstream>
#include <iomanip>
#include <thread>
#include "boost/date_time/posix_time/posix_time.hpp"
#define SECONDS_IN_DAY 86400;
using std::cin;
using std::cout;
using std::to_string;
using std::string;
using std::stoi;
using std::vector;
using std::stringstream;
using std::ifstream;
using std::time_t;
using namespace boost::posix_time;
using namespace boost::gregorian;

// General spaicng values
const int event_parts = 3;
const int app_width = 1400;
const int app_length = 700;
const int calendar_spaces = 8;
const int calendar_days = 7;
const float rec_x = (float)350 / calendar_spaces;
const int rec_y = 40;
const int rec_width = 150;
const int rec_length = 400;
const int water_bar_length = app_width - 2 * rec_x - 80;


// Seven day period for checking valid displayable events
class day_period_7 : public time_period {
 public:
  day_period_7(date d) : time_period(ptime(d),
                                       ptime(d, hours(24 * 7)))
  {}

};

// Class that stores all relevant information for the calendar events
class Event {
 public:
  string date,
         time,
         description;

  boost::gregorian::date d; // for checking if the event is in the current 7 day period
  float button_spacing;
  float buttonx;
  float buttony;

  int start_time;
  int end_time;
  Button b;
  bool in_period;

};

// Periodically checks if the user is on track to meet their water goals
bool watercheck(float total_water, float watergoal, sf::Text& water_popup_message, int hour) {
  vector<float> hours = {{12}, {15}, {18}, {21}};
  float increment = watergoal / 4;
  vector<float> minimumwater = {{increment}, {2 * increment}, {3 * increment}, {watergoal}};
  vector<string> stringwater;
  string behind_goal;
  float behind_goal_val;


  int count = 0;
  for (auto test : hours) {
    if (hour > test)
      count = count + 1;
  }

// Checks to see how far behind you are the water goal and
// updates the string displayed to the users screen as a reminder
  if(count == 0)
    return false;
  else if (total_water < minimumwater.at(count - 1)) {
    behind_goal_val = minimumwater.at(count - 1) - total_water;
    behind_goal = to_string(behind_goal_val);
    behind_goal = behind_goal.substr(0, 4);
    behind_goal = "Behind goal by " + behind_goal + " cups";
    water_popup_message.setString(behind_goal);
    return true;
  } else
    return false;

}

// How all events are created and place into the Events class
void create_event(string event_date, string event_time, string event_desc, Event& a) {
  string desc;
  int button_char_size;
// store the user inputs into the event
  a.date = event_date;
  a.time = event_time;
  a.description = event_desc;

// standard width of each calendar event
  a.buttonx = 150;

// Calculating the relative length of the calendar event
  vector<string> dates;

  stringstream split_date(event_date);

  while(split_date.good()) {  // Splitting the date into its parts, rearranging for use with the boost library
    string d;
    getline(split_date, d, '/');
    dates.push_back(d);
  }

  string usable_date = dates.at(2) + "-" + dates.at(0) + "-" + dates.at(1);
  date d(from_simple_string(usable_date));
  a.d = d; // gets the date in the correct format for checking against the current time period


  vector<string> times;

  stringstream split_time(event_time);

  while(split_time.good()) {  // Splitting the time to represent start and end times as seconds since 00:00
    string d;
    getline(split_time, d, '-');
    times.push_back(d);
  }

  int first_hour,
      second_hour,
      first_min,
      second_min;


  first_hour = stoi(times.at(0).substr(0, times.at(0).find(":")));
  second_hour = stoi(times.at(1).substr(0, times.at(1).find(":")));
  first_min = stoi(times.at(0).substr(times.at(0).find(":") + 1, 2));
  second_min = stoi(times.at(1).substr(times.at(1).find(":") + 1, 2));


  int first_time_as_seconds = first_hour * 3600 + first_min * 60;
  int second_time_as_seconds = second_hour * 3600 + second_min * 60;

  a.start_time = first_time_as_seconds;
  a.end_time = second_time_as_seconds;
  int time_dif = second_time_as_seconds - first_time_as_seconds;

  a.buttony = 355 * (float)time_dif / SECONDS_IN_DAY;

  a.button_spacing = 355 * (float)first_time_as_seconds / SECONDS_IN_DAY // needs to be a percentage of the final calendar box

  if(a.buttony<19){
    desc = event_desc+ " " + event_time;
    button_char_size = 8;
  }
  else{
    desc = event_desc +  "\n"+event_time;
    button_char_size = 10;
  }

  Button b(desc, {a.buttonx, a.buttony}, button_char_size, sf::Color::Black);
  a.b = b;
  a.b.setOutlineColor(sf::Color::Black);
  a.b.setOutlineThickness(1);

}

// Function simply checks if the user has input a valid date date, takes into account leap years
bool check_date(string date) {
  int month,
      day,
      year;
  vector<string> dates;
  bool leapyear;



  stringstream split_date(date);

  while(split_date.good()) {  // Split date into day month and year
    string d;
    getline(split_date, d, '/');
    dates.push_back(d);
  }

  month = stoi(dates.at(0));
  day = stoi(dates.at(1));
  year = stoi(dates.at(2));
  if(month < 0 || month > 12)
    return 0;
  if(month == 4 || month == 6 || month == 9 || month == 11) {
    if (day < 0 || day > 30) //account for the 30 day months
      return 0;
  }
  if(day < 0 || day > 31)
    return 0;
  if(year < 2021 || year > 9999)
    return 0;

  leapyear = gregorian_calendar::is_leap_year(year);

  if(!leapyear & month == 2 & day > 28)
    return 0;

  return 1;
}


// Function to remove spaces
string removeSpaces(string str) {
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  return str;
}

// Function to remove dashes
string removeDashes(string str) {
  str.erase(remove(str.begin(), str.end(), '-'), str.end());
  return str;
}

// Function to check if time input is valid
bool check_time(string user_time) {
  vector<string> vec, vec_first, vec_second;
  stringstream ss(user_time);
  string line;
  // split string at '-'
  while(std::getline(ss, line, '-')) {  // Split the time into its hours and minutes
    vec.push_back(line);
  }
  // check there is a start and end time

  if (vec.size() != 2)
    return 0;

  int first_hour, second_hour, first_min, second_min;
  string first = removeSpaces(vec.at(0));
  string second = removeSpaces(vec.at(1));
  stringstream ss1(first);
  // split start time into hour and min
  while(std::getline(ss1, line, ':')) {
    vec_first.push_back(line);
  }

  // Making sure there is an hour and min input
  if (vec_first.size() != 2)
    return 0;
  first_hour = stoi(vec_first.at(0));
  first_min = stoi(vec_first.at(1));
  // Checking appropiate input
  if (first_hour < 0 || first_hour > 23)
    return 0;
  if (first_min < 0 || first_min > 59)
    return 0;

  // Repeating for the end time
  stringstream ss2(second);
  while(std::getline(ss2, line, ':')) {
    vec_second.push_back(line);
  }
  if (vec_second.size() != 2)
    return 0;
  second_hour = stoi(vec_second.at(0));
  second_min = stoi(vec_second.at(1));
  if (second_hour < 0 || second_hour > 23)
    return 0;
  if (second_min < 0 || second_min > 59)
    return 0;

// Check that the start time is before end time
  if (first_hour > second_hour)
    return 0;

  return 1;
}

// Function updates the current water intake, stores the value externally so its remembered on close
void update_water(string w, float& u, float& j, int watergoal) {

  float added_water;
  // check for multiple decimal inputs, return 0 if only decimal or multiples
  int count = 0;
  for (int i = 0; i < w.size(); i++) {
    if(w.at(i) + 0 == 46)
      count = count + 1;
  }
  if(w.size() == 1 & count == 1 | count >= 2) {
    added_water = 0;
    u = u + added_water;
    j = u / watergoal;
  } else {
    j = std::stof(w);
    added_water = std::stof(w);
    added_water = floorf(added_water * 100) / 100;
    u = u + added_water;
    j = u / watergoal;
  }
  std::ofstream water_state("water_state.txt"); // update the water save file
  water_state << u;
  water_state.close();
}

// Check if the event is in a displayable time period
bool check_in_period(boost::gregorian::date local_time, date d) {
  day_period_7 dp(local_time);
  ptime t(d);
  if (dp.contains(t)) {
    return 1;
  }
  return 0;
}

// Check if the event is in the current day
bool check_in_period_by_day(boost::gregorian::date local_time, date d, int i) {
  ptime t(local_time);
  ptime x(local_time, hours(24) * (i + 1));
  time_period g(t, x);
  ptime work(d);
  if (g.contains(work)) {
    return 1;
  }
  return 0;
}

// Check if newly added event conflicts with previous events
bool time_conflict(Event checking, vector<Event> calendar) {

  if(calendar.size() == 1)
    return 0;

  for(int i = 0; i < calendar.size() - 1; i++) {
    if(checking.date == calendar.at(i).date) {
      if((checking.start_time<calendar.at(i).end_time&checking.start_time>calendar.at(i).start_time) |
          (checking.end_time<calendar.at(i).end_time&checking.end_time>calendar.at(i).start_time))
        return 1;
    }
  }
  return 0;

}
// Function for setting the current calendar date, adjusts based on the local time
vector<string> set_calendar_date(boost::gregorian::date local_time) {
  vector<string> times;
  for (int i = -1; i < 6; i++) {
    ptime t(local_time, hours(24) * (i + 1));
    string s = to_simple_string(t);
    times.push_back(s.substr(5, 3) + " " + to_string(stoi(s.substr(9))));
  }
  return times;
}
// Function for moving the calendar forward or backwards a week
void set_calendar_date(boost::gregorian::date local_time, vector<string>& days, vector<sf::Text>& day) {
  for (int i = -1; i < 6; i++) {  // Getting the month value as well as the numeric day in that month for the given time period
    ptime t(local_time, hours(24) * (i + 1));
    string s = to_simple_string(t);
    days.at(i + 1) = (s.substr(5, 3) + " " + to_string(stoi(s.substr(9))));
  }
  int day_x = rec_x + 50;
  int day_y = 50;
  sf::Text day_text;
  day_text.setString(days.at(0));
  day_text.setFillColor(sf::Color::Black);
  day_text.setOutlineColor(sf::Color::White);
  day_text.setOutlineThickness(1);
  day_text.setPosition(day_x, day_y);

  int x = day_x + 50;
  for (int i = 0; i < days.size(); i++) {
    day.push_back(day_text);
    day.at(i).setPosition(x - 65, day_y);
    day.at(i).setString(days.at(i));
    x = x + rec_x + rec_width;
  }
}

// Function checks the input event to see if its in the 7 day period being displayed, correctly spaces the event boxes accordingly
void please_give_position(date local_time, boost::gregorian::date d, Event& calendar_event) {

  if(check_in_period_by_day(local_time, calendar_event.d, 0))
    calendar_event.b.setPosition({rec_x, 85 + calendar_event.button_spacing});
  else if(check_in_period_by_day(local_time, calendar_event.d, 1))
    calendar_event.b.setPosition({2 * (rec_x) + rec_width, 85 + calendar_event.button_spacing});
  else if(check_in_period_by_day(local_time, calendar_event.d, 2))
    calendar_event.b.setPosition({3 * (rec_x) + 2 * rec_width, 85 + calendar_event.button_spacing});
  else if(check_in_period_by_day(local_time, calendar_event.d, 3))
    calendar_event.b.setPosition({4 * (rec_x) + 3 * rec_width, 85 + calendar_event.button_spacing});
  else if(check_in_period_by_day(local_time, calendar_event.d, 4))
    calendar_event.b.setPosition({5 * (rec_x) + 4 * rec_width, 85 + calendar_event.button_spacing});
  else if(check_in_period_by_day(local_time, calendar_event.d, 5))
    calendar_event.b.setPosition({6 * (rec_x) + 5 * rec_width, 85 + calendar_event.button_spacing});
  else
    calendar_event.b.setPosition({7 * (rec_x) + 6 * rec_width, 85 + calendar_event.button_spacing});

}

// Function adds dashes, used when storing the event description in external file for easier file input
string add_dashes(string desc) {
  for(int i = 0; i < desc.length(); i++) {
    if (desc.at(i) == ' ')
      desc.at(i) = '-';
  }
  return desc;

}

// Function removes the dashes from the event description in put on open, stores them for use by the event class
string remove_dashes(string remove_dashes) {

  for(int i = 0; i < remove_dashes.length(); i++) {
    if (remove_dashes.at(i) == '-')
      remove_dashes.at(i) = ' ';
  }
  return remove_dashes;

}

// Function checks how many events there are on the given day
bool start_day_reminder(vector<int> months, vector<int> dates, int* count) {
  std::time_t time = std::time(NULL);
  std::tm now = *std::localtime(&time);
  int tempcount = 0;
  int index = 0;
  int k;

  for (auto j : months) {
    k = dates.at(index);
    if (j == now.tm_mon + 1)
      if (k == now.tm_mday)
        tempcount++;
    index++;
  }
  if (tempcount > 0) {
    *count = tempcount;
    return true;
  } else return false;
}

// Function to format calander events
void getevents(vector<int>* mlocation, vector<int>* dlocation) {
  vector<string> allevents;
  string line;
  vector<int> months;
  vector<int> dates;
  string mtemp = "";
  string dtemp = "";

  ifstream read_events;
  read_events.open("calendar.txt");

  if (read_events.is_open()) {
    while(getline (read_events, line)) {
      allevents.push_back(line);
    }
    read_events.close();
  }

  for (string i : allevents) {
    if (i.at(1) == '/') {
      // months.push_back(stoi(i.at(0)));
      mtemp = i.at(0);
      dtemp = dtemp + i.at(2) + i.at(3);
    } else {
      // temp = i.at(0) + i.at(1);
      mtemp = mtemp + i.at(0) + i.at(1);
      dtemp = dtemp + i.at(3) + i.at(4);
    }
    months.push_back(stoi(mtemp));
    dates.push_back(stoi(dtemp));
    mtemp.clear();
    dtemp.clear();
  }
  *mlocation = months;
  *dlocation = dates;
}

// Stores the current water goal so that it persists on app close
void save_water_goal(int watergoal) {
  std::ofstream water_goal("water_goal.txt"); // update the water save file
  water_goal << watergoal;
  water_goal.close();

}


int main() {
  int event_count = 0;
  float total_water = (float)0;
  float percent_water = (float)0;
  float per_bar = (float)0;
  string load_water;
  string water_consumed;
  string tmp;
  string event_date,
         event_time,
         event_desc;
  string water_goal_string;
  sf::Text confirm_event_text;
  Event calendar_event;
  vector<Event> calendar;
  string dashed;
  string water_goal_string2;
  int watergoal = 8; // Initial water goal, gets updated based on user preference

  date local_time(day_clock::local_day()); // Get local time to set calendar date


  // Load in the state of water
  std::ifstream water_in ("water_state.txt");
  if (water_in.is_open()) {
    while ( getline (water_in, load_water) ) {

    }
    water_in.close();
    if (load_water.size() > 0)
      total_water = total_water + std::stof(load_water);
  }

  // Load in the water goal
  std::ifstream water_goal_in ("water_goal.txt");
  if (water_goal_in.is_open()) {
    while ( getline (water_goal_in, water_goal_string) ) {

    }
    water_goal_in.close();
    if (water_goal_string.length() >= 1)
      watergoal = 0 + std::stoi(water_goal_string);
    else {
      watergoal = 8;
    }
  }

// Determining the initial state of progress towards water goal
  percent_water = total_water / watergoal;



  sf::RenderWindow window(sf::VideoMode(app_width, app_length), "Calendar");



// TEXTURES & FONTS
///////////////////////////////////////////////////////////////
  sf::Font font;
  font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");

  sf::Texture paper_texture;
  if (!paper_texture.loadFromFile("paper.png")) {
    cout << "Texture did not load correctly\n";
  };
  paper_texture.setSmooth(true);

  sf::Texture app_background; // Background for app
  if (!app_background.loadFromFile("pastel.jpg")) {
    cout << "Texture did not load correctly\n";
  };
  app_background.setSmooth(true);

  sf::Texture water_texture; // Water progress bar
  if (!water_texture.loadFromFile("water.jpg", sf::IntRect(0, 120, 480, 240))) {
    cout << "Texture did not load correctly\n";
  };
  water_texture.setSmooth(true);

  sf::Texture button_texture; // Button for water
  if (!button_texture.loadFromFile("button.jpg", sf::IntRect(60, 60, 240, 220))) {
    cout << "Texture did not load correctly\n";
  };
  button_texture.setSmooth(true);


  sf::Texture set_texture; // Button for water
  if (!set_texture.loadFromFile("setting.png")) {
    cout << "Texture did not load correctly\n";
  };
  set_texture.setSmooth(true);

// "Icon made by Freeplk from www.flaticon.com"
  sf::Texture plus_event; // Button for water
  if (!plus_event.loadFromFile("plus.png")) {
    cout << "Texture did not load correctly\n";
  };
  plus_event.setSmooth(true);

// "Icon made by Freeplk from www.flaticon.com"
  sf::Texture yes_add; // add event
  if (!yes_add.loadFromFile("yes.png")) {
    cout << "Texture did not load correctly\n";
  };
  yes_add.setSmooth(true);

// "Icon made by Freeplk from www.flaticon.com"
  sf::Texture no_add; // dont add event
  if (!no_add.loadFromFile("no.png")) {
    cout << "Texture did not load correctly\n";
  };
  no_add.setSmooth(true);

  sf::Texture reset;
  if (!reset.loadFromFile("reset-button.png")) {
    cout << "Texture did not load correctly\n";
  };
  reset.setSmooth(true);

  sf::Texture cups_entered;
  if (!cups_entered.loadFromFile("enter_cups.png")) {
    cout << "Texture did not load correctly\n";
  };
  cups_entered.setSmooth(true);

// "Icon made by Freeplk from www.flaticon.com"
  sf::Texture calendar_right;
  if (!calendar_right.loadFromFile("moveright.png")) {
    cout << "Texture did not load correctly\n";
  };
  calendar_right.setSmooth(true);

// "Icon made by Freeplk from www.flaticon.com"
  sf::Texture calendar_left;
  if (!calendar_left.loadFromFile("moveleft.png")) {
    cout << "Texture did not load correctly\n";
  };
  calendar_left.setSmooth(true);

// creating the button for the calendar event
  sf::Texture event_texture;
  if(!event_texture.loadFromFile("Event_background.png")) {
    cout << "Texture did not load correctly\n";
  };
  event_texture.setSmooth(true);

  sf::Texture water_prompt;
  if(!water_prompt.loadFromFile("enter_water_prompt.png")) {
    cout << "Texture did not load correctly\n";
  };
  water_prompt.setSmooth(true);

/////////////////////////////////////////////////////////////
  // Reading in the calendar events and checking which are in the displayed period


  std::ifstream read_events;
  read_events.open("calendar.txt");

  Event cal_event;
  while (read_events >> cal_event.date >> cal_event.time >> cal_event.description) {
    string descrip = remove_dashes(cal_event.description);
    create_event(cal_event.date, cal_event.time, descrip, cal_event);
    calendar.push_back(cal_event);
  }


  for(int i = 0; i < calendar.size(); i++) {
    if(check_in_period(local_time, calendar.at(i).d)) {
      calendar.at(i).in_period = true;
    } else
      calendar.at(i).in_period = false;
    calendar.at(i).b.setFont(font);
    calendar.at(i).b.setTextFill(sf::Color::Black);
    calendar.at(i).b.setTexture(event_texture);
  }


// SHAPES
////////////////////////////////////////////////////////////
// Background Shape

  sf::RectangleShape background;
  background.setSize(sf::Vector2f(app_width, app_length));
  background.setTexture(&app_background);

// Water progress bar
  sf::RectangleShape water_bar;
  water_bar.setPosition(rec_x, rec_length + rec_y + 40);
  water_bar.setTexture(&water_texture);

// Outline of progess bar
  sf::RectangleShape water_bar_outline;
  water_bar_outline.setSize({water_bar_length, 50});
  water_bar_outline.setPosition(rec_x, rec_length + rec_y + 40);
  water_bar_outline.setFillColor(sf::Color::Transparent);
  water_bar_outline.setOutlineColor(sf::Color::Magenta);
  water_bar_outline.setOutlineThickness(2);

// Water input button
  sf::CircleShape water_button(40);
  water_button.setTexture(&button_texture);
  water_button.setPosition(app_width - rec_x * 2 - 20, rec_length + rec_y + 30);

// Calendar shapes
  vector<sf::RectangleShape> rect_vec;
  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(rec_width, rec_length));
  rectangle.setOutlineThickness(1);
  rectangle.setPosition(rec_x, rec_y);
  rectangle.setTexture(&paper_texture);

// Button to clear the current water count
  Button clear_button("", { 80, 30 }, 15, sf::Color::Black);
  clear_button.setFont(font);
  clear_button.setTexture(reset);
  clear_button.setFillColor(sf::Color::White);
  clear_button.setPosition({app_width - rec_x * 2 - 20, rec_length + rec_y + 120 });

// Go forward a week button
  Button forward_week("", {20, 20}, 0, sf::Color::Black);
  forward_week.setPosition({1370, 50});
  forward_week.setTexture(calendar_right);

// Go back a week button
  Button back_week("", {20, 20}, 0, sf::Color::Black);
  back_week.setPosition({10, 50});
  back_week.setTexture(calendar_left);

// Settings button
  Button settings("", {100, 100}, 0, sf::Color::Black);
  settings.setFont(font);
  settings.setTexture(set_texture);
  settings.setPosition({1300, 600});

// Confirm event button
  Button confirm_event("", {400, 250}, 0, sf::Color::Black);
  confirm_event.setFont(font);
  confirm_event.centerScreen(window);

// Event add button
  Button event_add("", {50, 50}, 0, sf::Color::Black);
  event_add.setTexture(plus_event);
  event_add.setFillColor(sf::Color::White);
  event_add.setPosition({90, 600});

// Confirm event yes
  Button yes("", {50, 50}, 0, sf::Color::Black);
  yes.setTexture(yes_add);
  yes.setPosition({550, 250});

// Confirm event no
  Button no("", {50, 50}, 0, sf::Color::Black);
  no.setTexture(no_add);
  no.setPosition({800, 250});

// Names for the Calendar boxes
  vector<string> days = set_calendar_date(local_time);
  vector<sf::Text> day;

// First day to copy for the calendar names
  int day_x = rec_x + 50;
  int day_y = 50;
  sf::Text day_text;
  day_text.setFont(font);
  day_text.setString(days.at(0));
  day_text.setFillColor(sf::Color::Black);
  day_text.setOutlineColor(sf::Color::White);
  day_text.setOutlineThickness(1);
  day_text.setPosition(day_x, day_y);

  int x = day_x + 50;
  for (int i = 0; i < days.size(); i++) {
    day.push_back(day_text);
    day.at(i).setPosition(x - 65, day_y);
    day.at(i).setString(days.at(i));
    x = x + rec_x + rec_width;
  }

//Creating the Day Boxes
  float a = rec_width + (float)350 / calendar_spaces;
  for (int i = 0; i < 6; i++) {
    rect_vec.push_back(rectangle);
    rect_vec.at(i).setPosition(rec_x + a, rec_y);
    a = a + rec_width + (float)350 / calendar_spaces;
  }

// Directions to add event
  sf::Text add_event;
  add_event.setFont(font);
  add_event.setString("Add Event: Enter Day, Time with minutes in 24-hour time, and Event in the Textbox below, separated by the RETURN key");
  add_event.setCharacterSize(20);
  add_event.setFillColor(sf::Color::Black);
  add_event.setPosition(60, 550);
  sf::Text add_info;
  add_info.setFont(font);
  add_info.setString("Example: 12/23/2021, 12:00-14:30, Tennis Practice");
  add_info.setCharacterSize(20);
  add_info.setFillColor(sf::Color::Black);
  add_info.setPosition(180, 600);

// Line under user input
  sf::RectangleShape line(sf::Vector2f(800, 2));
  line.setOutlineColor(sf::Color::Black);
  line.setFillColor(sf::Color::Black);
  line.setPosition(180, 670);

// Textbox for the users input
  string input_text;
  sf::Text text("", font);
  sf::Clock clock;

// Box to prompt user for water goal
  sf::RectangleShape water_prompt_box;
  water_prompt_box.setSize(sf::Vector2f(500, 300));
  water_prompt_box.setTexture(&water_prompt);
  water_prompt_box.setOutlineThickness(1);
  water_prompt_box.setPosition(450, 200);

// Text user inputs in water prompt goal
  sf::String water_goal_input;
  sf::Text water_goal_output;
  water_goal_output.setPosition(700, 450);
  water_goal_output.setFillColor(sf::Color::Black);
  water_goal_output.setFont(font);
  water_goal_output.setOutlineColor(sf::Color::Black);
  water_goal_output.setOutlineThickness(1);
  water_goal_output.setCharacterSize(40);

/// Box to show user what input of water consumed
  sf::RectangleShape water_box;
  water_box.setSize(sf::Vector2f(200, 150));
  water_box.setTexture(&cups_entered);
  water_box.setOutlineThickness(1);
  water_box.setPosition(600, 400);

// Text user inputs in water box
  sf::String water_input;
  sf::Text water_output;
  water_output.setPosition(1000, 400);
  water_output.setFillColor(sf::Color::Red);
  water_output.setFont(font);
  water_output.setFillColor(sf::Color::Black);
  water_output.setOutlineColor(sf::Color::White);
  water_output.setOutlineThickness(1);

// Text of the event the user entered
  confirm_event_text.setFillColor(sf::Color::Red);
  confirm_event_text.setFont(font);
  confirm_event_text.setFillColor(sf::Color::Black);
  confirm_event_text.setOutlineColor(sf::Color::White);
  confirm_event_text.setOutlineThickness(1);

// Text to show percentage of water
  sf::Text perc_water_text;
  perc_water_text.setFillColor(sf::Color::Blue);
  perc_water_text.setFont(font);
  perc_water_text.setOutlineThickness(0.1);
  perc_water_text.setPosition(1050, 450);
  perc_water_text.setCharacterSize(20);
  if(percent_water < 1) {
    string water_string = to_string(percent_water * 100);
    water_string = water_string.substr(0, 5);
    perc_water_text.setString(water_string + " Percent of Goal");
  } else
    perc_water_text.setString("Water Goal Complete!");

// Water popup
  Button popup("", {300, 100}, 0, sf::Color::Black);
  popup.setFillColor(sf::Color::Blue);
  popup.setOutlineColor(sf::Color::Black);

  sf::Text water_popup_message;
  water_popup_message.setString("DRINK MORE WATER");
  water_popup_message.setCharacterSize(20);
  water_popup_message.setPosition(1000, 400);
  water_popup_message.setFont(font);
  water_popup_message.setFillColor(sf::Color::White);
  water_popup_message.setOutlineColor(sf::Color::Black);
  water_popup_message.setOutlineThickness(0.1);

  // Button to close the water popup
  Button close("", {30, 30}, 0, sf::Color::Red);
  close.setPosition({810, 260});
  close.setTexture(no_add);

  // Display an error message for incorrect user input on date or time
  Button error_popup("", {300, 150}, 0, sf::Color::Black);
  error_popup.setFillColor(sf::Color::White);
  error_popup.setOutlineColor(sf::Color::Red);
  error_popup.setOutlineThickness(5);

  sf::Text input_error_text;
  input_error_text.setFillColor(sf::Color::Black);
  input_error_text.setOutlineColor(sf::Color::Red);
  input_error_text.setFont(font);
  input_error_text.setOutlineThickness(0.08);
  input_error_text.setCharacterSize(25);

  Button close_error("", {30, 30}, 0, sf::Color::Red);
  close_error.setPosition({810, 200});
  close_error.setTexture(no_add);

  // Text to show current water goal
  sf::Text current_watergoal;
  current_watergoal.setFillColor(sf::Color::Blue);
  current_watergoal.setFont(font);
  current_watergoal.setOutlineThickness(0.1);
  current_watergoal.setPosition(1050, 600);
  current_watergoal.setCharacterSize(25);
  current_watergoal.setString("Current Water Goal:\n" + to_string(watergoal) + " cups");

  //Button to notify how many events are occuring on the current day
  Button event_count_reminder("", {400, 200}, 0, sf::Color::Black);
  event_count_reminder.setFillColor(sf::Color::White);
  event_count_reminder.setOutlineColor(sf::Color::Black);
  event_count_reminder.setPosition({500, 400});

// Text for the event popup
  sf::Text event_popup_message;
  event_popup_message.setCharacterSize(20);
  event_popup_message.setPosition(1000, 400);
  event_popup_message.setFont(font);
  event_popup_message.setFillColor(sf::Color::Black);
  event_popup_message.setOutlineColor(sf::Color::Black);
  event_popup_message.setOutlineThickness(0.1);

// Button to close the event popup
  Button close_event_pop("", {30, 30}, 0, sf::Color::Red);
  close_event_pop.setPosition({850, 440});
  close_event_pop.setTexture(no_add);

// DISPLAY BOOLEANS
//////////////////////////////////////
  // Controls for where user input should be directed, as well as when shapes should be displayed
  bool display_water_box = false;
  bool disp_text = false;
  bool water_enter = false;
  bool display_settings_box = false;
  bool water_goal_enter = false;
  bool display_water_goal = false;
  bool flash_clear_water = false;
  bool enter_event_bool = false;
  bool confirm_event_bool = false;
  bool add_event_bool = false;
  bool increase_by_week = false;
  bool reduce_by_week = false;
  bool checker = false;
  bool flash_enter_water = false;
  bool error_bool = false;
  bool show_event_count = false;


///////////////////////////////////////////
  window.setFramerateLimit(30);

  while(window.isOpen()) {
    window.clear();
    sf::Event event;
    if(total_water >= 0) { // moving progress bar for water goal
      if(total_water >= watergoal)
        water_bar.setSize(sf::Vector2f(water_bar_length, 50));
      else
        water_bar.setSize(sf::Vector2f(percent_water * water_bar_length, 50));
    }
// Checking the users water intake periodically, reminding them if they are behind schedule
    std::time_t time = std::time(NULL);
    std::tm now = *std::localtime(&time);
    int h = now.tm_hour;
    int m = now.tm_min;
    int s = now.tm_sec;
    float ms = (float)1 / 60;

    if (m == 1 & s == 0) {
      checker = watercheck(total_water, watergoal, water_popup_message, h);
    }

// Checking number of events for the day
    int ncount;
    vector<int> months;
    vector<int> dates;

    if (h == 8 & m == 0 & s == 0) {
      getevents(&months, &dates);
      show_event_count = start_day_reminder(months, dates, &ncount);
    }

// Makes the calendar display the next weeks dates and events
    if(increase_by_week) {
      local_time = local_time + weeks(1);
      set_calendar_date(local_time, days, day);
      increase_by_week = false;
      for(int i = 0; i < calendar.size(); i++) {
        if(check_in_period(local_time, calendar.at(i).d)) {
          calendar.at(i).in_period = true;
        } else
          calendar.at(i).in_period = false;
        calendar.at(i).b.setFont(font);
        calendar.at(i).b.setTextFill(sf::Color::Black);
      }
    }

// Makes the calendar display the previous weeks dates and events
    if(reduce_by_week) {
      local_time = local_time - weeks(1);
      set_calendar_date(local_time, days, day);
      reduce_by_week = false;
      for(int i = 0; i < calendar.size(); i++) {
        if(check_in_period(local_time, calendar.at(i).d)) {
          calendar.at(i).in_period = true;
        } else
          calendar.at(i).in_period = false;
        calendar.at(i).b.setFont(font);
        calendar.at(i).b.setTextFill(sf::Color::Black);
      }
    }

//DRAWING SHAPES
///////////////////////////////////////////////////

// Background image
    window.draw(background);
    window.draw(water_bar); // need to link this to the water input, cover up watergoal-remaining for a progress bar

// Drawing the calendar boxes
    window.draw(rectangle);
    for (int i = 0; i < rect_vec.size(); i++)
      window.draw(rect_vec.at(i));

// Drawing the dates
    /*    window.draw(day_text);*/
    for (int i = 0; i < day.size(); i++)
      window.draw(day.at(i));

// Settings button
    settings.drawTo(window);

// Water bar outline
    window.draw(water_bar_outline);

// Percent of water goal
    window.draw(perc_water_text);

// Water goal
    window.draw(current_watergoal);

// Add event button
    event_add.drawTo(window);

// Draw text: add_event & boxes
    window.draw(add_event);
    window.draw(add_info);
    window.draw(line);

    // Dispaly Water input background box
    if(display_water_box) {
      window.draw(water_box);
      sf::FloatRect rectBounds = water_box.getGlobalBounds();
      sf::FloatRect textBounds = water_output.getGlobalBounds();

      water_output.setPosition(
        rectBounds.left + (rectBounds.width / 2) - (textBounds.width / 2),
        rectBounds.top + (rectBounds.height / 2) - textBounds.height);
    }
    // Display Water input
    if(disp_text)
      window.draw(water_output);

    // Clear button
    clear_button.drawTo(window);

    // Flash color on click of clear button
    if(flash_clear_water) {
      clear_button.setFillColor(sf::Color::Magenta);
      flash_clear_water = false;
    }
// Water button
    window.draw(water_button);
// Flash color on click of water button
    if(flash_enter_water) {
      water_button.setFillColor(sf::Color::Magenta);
      flash_enter_water = false;
    }

// Draw input for event to window
    static sf::Time text_effect_time;  // Used to flash a cursor under the users text inputs
    static bool show_cursor;

    text_effect_time += clock.restart();

    if(enter_event_bool) {
      if (text_effect_time >= sf::seconds(0.5f)) {
        show_cursor = !show_cursor;
        text_effect_time = sf::Time::Zero;
      }
      text.setString(input_text + (show_cursor ? '_' : ' '));
      window.draw(text);
      text.setPosition(190, 630);
      text.setFillColor(sf::Color::Blue);
    }

    if(add_event_bool) { // Takes users input and creates a corresponding event, error checks for valid date and time
      create_event(event_date, event_time, event_desc, calendar_event);
      calendar.push_back(calendar_event);
      calendar.back().b.setFont(font);
      calendar.back().b.setTexture(event_texture);
      calendar.back().b.setTextFill(sf::Color::Black);
      if(time_conflict(calendar.back(), calendar)) {
        input_error_text.setString("Error! Time Conflict\n Please Try Again");
        error_bool = true;
        calendar.pop_back();
      } else {
        if(check_in_period(local_time, calendar_event.d)) {
          calendar.back().in_period = true;
        }
      }
      add_event_bool = false;
    }

    if (show_event_count) {
      event_popup_message.setString("You have " + to_string(ncount) + " events today!\n Remember to take breaks!");
      event_count_reminder.drawTo(window);
      close_event_pop.drawTo(window);

      sf::FloatRect event_count_reminder_bounds = event_count_reminder.getGlobalBounds();
      sf::FloatRect event_popup_message_bounds = event_popup_message.getGlobalBounds();

      event_popup_message.setPosition(
        event_count_reminder_bounds.left + (event_count_reminder_bounds.width / 2) - (event_popup_message_bounds.width / 2),
        event_count_reminder_bounds.top + (event_count_reminder_bounds.height / 2) - event_popup_message_bounds.height);
      window.draw(event_popup_message);
    }

// Draw the calendar
    if(calendar.size() > 0) {
      for(auto e : calendar) {
        if(e.in_period == true) {
          please_give_position(local_time, e.d, e);
          e.b.drawTo(window);
        }
      }
    }

    // Draw forward week/back week
    back_week.drawTo(window);
    forward_week.drawTo(window);

    // Displaying the settings box
    if(display_settings_box) {
      window.draw(water_prompt_box);
      sf::FloatRect water_prompt_Bounds = water_prompt_box.getGlobalBounds();
      sf::FloatRect water_goal_text_Bounds = water_goal_output.getGlobalBounds();

      water_goal_output.setPosition(
        water_prompt_Bounds.left + (water_prompt_Bounds.width / 2) - (water_goal_text_Bounds.width / 2),
        water_prompt_Bounds.top + (water_prompt_Bounds.height / 2) - water_goal_text_Bounds.height);
    }

    // display water goal input
    if(display_water_goal)
      window.draw(water_goal_output);

// Displaying the confirm event box
    if(confirm_event_bool) {
      confirm_event.drawTo(window);
      yes.drawTo(window);
      no.drawTo(window);
      sf::FloatRect confirm_bounds = confirm_event.getGlobalBounds();
      sf::FloatRect confirm_text_bounds = confirm_event_text.getGlobalBounds();

      confirm_event_text.setPosition(
        confirm_bounds.left + (confirm_bounds.width / 2) - (confirm_text_bounds.width / 2),
        confirm_bounds.top + (confirm_bounds.height / 2) - confirm_text_bounds.height);
      window.draw(confirm_event_text);
    }

    // Displaying the 'drink water' popup
    if (checker) {
      popup.centerScreen(window);
      popup.drawTo(window);
      close.drawTo(window);

      sf::FloatRect popup_bounds = popup.getGlobalBounds();
      sf::FloatRect water_popup_message_bounds = water_popup_message.getGlobalBounds();

      water_popup_message.setPosition(
        popup_bounds.left + (popup_bounds.width / 2) - (water_popup_message_bounds.width / 2),
        popup_bounds.top + (popup_bounds.height / 2) - water_popup_message_bounds.height);
      window.draw(water_popup_message);
    }

    // Displaying an error popop when an error occurs
    if (error_bool) {
      error_popup.centerScreen(window);
      error_popup.drawTo(window);
      close_error.drawTo(window);

      sf::FloatRect error_popup_bounds = error_popup.getGlobalBounds();
      sf::FloatRect input_error_message_bounds = input_error_text.getGlobalBounds();

      input_error_text.setPosition(
        error_popup_bounds.left + (error_popup_bounds.width / 2) - (input_error_message_bounds.width / 2),
        error_popup_bounds.top + (error_popup_bounds.height / 2) - input_error_message_bounds.height + 30);
      window.draw(input_error_text);

    }

    // Events
    while(window.pollEvent(event)) {

      if(enter_event_bool) {  // Used to take in user events, blocks invalid text entrys for date and time
        if(!water_enter) {
          if (event_count == 0) {
            if (event.type == sf::Event::TextEntered) {
              if (event.text.unicode >= 47 & event.text.unicode < 58 |
                  event.text.unicode == 32)
                if(input_text.size() < 10)
                  input_text += event.text.unicode;
            } else if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::BackSpace) {
                if (!input_text.empty())
                  input_text.pop_back();
              } else if (event.key.code == sf::Keyboard::Return) {
                if(check_date(input_text)) {
                  event_date = input_text;
                  event_count = event_count + 1;
                  input_text.clear();
                } else {
                  input_text.clear();
                  input_error_text.setString("Error! Invalid Date\n Please Try Again");
                  error_bool = true;
                }
              } else if (event.key.code == sf::Keyboard::Escape) {
                input_text.clear();
                enter_event_bool = false;
                event_count = 0;
              }
            }
          }

          else if (event_count == 1) {
            if (event.type == sf::Event::TextEntered) {
              if (event.text.unicode > 47 & event.text.unicode < 58 |
                  event.text.unicode == 58 |
                  event.text.unicode == 45 )
                if(input_text.size() < 11)
                  input_text += event.text.unicode;
            } else if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::BackSpace) {
                if (!input_text.empty())
                  input_text.pop_back();
              } else if (event.key.code == sf::Keyboard::Return) {
                if(check_time(input_text)) {
                  event_time = input_text;
                  event_count = event_count + 1;
                  input_text.clear();
                } else {
                  input_text.clear();
                  input_error_text.setString("Error! Invalid Time\n Please Try Again");
                  error_bool = true;
                }
              } else if (event.key.code == sf::Keyboard::Escape) {
                input_text.clear();
                enter_event_bool = false;
                event_count = 0;
              }
            }
          }

          else if (event_count == 2) {
            if (event.type == sf::Event::TextEntered) {
              if (std::isprint(event.text.unicode)) {
                if(input_text.length() < 18)
                  input_text += event.text.unicode;
              }
            } else if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::BackSpace) {
                if (!input_text.empty())
                  input_text.pop_back();
              } else if (event.key.code == sf::Keyboard::Return) {
                event_desc = input_text;
                event_count = event_count + 1;
                input_text.clear();
              } else if (event.key.code == sf::Keyboard::Escape) {
                input_text.clear();
                enter_event_bool = false;
                event_count = 0;
              }
            }
          }
        }
        if (event_count == 3) {
          enter_event_bool = false;
          confirm_event_bool = true;
          event_count = 0;
          confirm_event_text.setString("Add this Event?\n" +
                                       event_desc + "\n" + event_date + "\n" + event_time);
        }
      }
/////////////////////////////////////////////////////////////////////
      // Takes in the users water input, blocks non-numeric entry other than decimal points
      if(water_enter) {
        if(event.type == sf::Event::TextEntered) {
          if (event.text.unicode > 47 & event.text.unicode < 58 | event.text.unicode == 46) {
            tmp = static_cast<char>(event.text.unicode);
            water_consumed.append(tmp);
            water_input += event.text.unicode;
            water_output.setString(water_input);
            string water_string = to_string(percent_water * 100);
            water_string = water_string.substr(0, 5);
            perc_water_text.setString(water_string + " Percent of Goal");
            disp_text = true;
          } else if (event.text.unicode == 13) {
            if(water_consumed.size() == 0) {
              display_water_box = false;
              disp_text = false;
              water_enter = false;
            }
            if(water_consumed.size() > 0) {
              update_water(water_consumed, total_water, percent_water, watergoal);
              water_consumed.clear();
              water_input.clear();
              if(percent_water < 1) {
                string water_string = to_string(percent_water * 100);
                water_string = water_string.substr(0, 5);
                perc_water_text.setString(water_string + " Percent of Goal");
              } else
                perc_water_text.setString("Water Goal Complete!");
              display_water_box = false;
              disp_text = false;
              water_enter = false;
            }
          } else if (event.text.unicode == 8) {
            if(water_consumed.size() > 0) {
              water_consumed.pop_back();
              water_input.erase(water_input.getSize() - 1, 1);
              water_output.setString(water_input);
              string water_string = to_string(percent_water * 100);
              water_string = water_string.substr(0, 5);
              perc_water_text.setString(water_string + " Percent of Goal");
            }
          } else if (event.text.unicode == 27) {
            water_input.clear();
            water_consumed.clear();
            disp_text = false;
            display_water_box = false;
            water_enter = false;
          }
        }
      }


      // Used to update the users daily water goal
      if (water_goal_enter) {
        if(event.type == sf::Event::TextEntered) {
          if (event.text.unicode > 47 & event.text.unicode < 58) {
            tmp = static_cast<char>(event.text.unicode);
            water_goal_string2.append(tmp);
            water_goal_input += event.text.unicode;
            water_goal_output.setString(water_goal_input);
            display_water_goal = true;
          } else if (event.text.unicode == 13) {
            if(water_goal_string2.size() == 0) {
              display_settings_box = false;
              display_water_goal = false;
              water_goal_enter = false;
            }
            if(water_goal_string2.size() > 0) {
              watergoal = stoi(water_goal_string2);
              water_goal_string = water_goal_string2;
              update_water("0", total_water, percent_water, watergoal);
              save_water_goal(watergoal);
              current_watergoal.setString("Current Water Goal:\n" + to_string(watergoal) + " cups");
              water_goal_string.clear();
              water_goal_string2.clear();
              water_goal_input.clear();
              if(percent_water < 1) {
                string water_string = to_string(percent_water * 100);
                water_string = water_string.substr(0, 5);
                perc_water_text.setString(water_string + " Percent of Goal");
              } else
                perc_water_text.setString("Water Goal Complete!");
              display_settings_box = false;
              display_water_goal = false;
              water_goal_enter = false;

            } else if (event.text.unicode == 8) {
              if(water_goal_string2.size() > 0) {
                water_goal_string2.pop_back();
                water_goal_input.erase(water_goal_input.getSize() - 1, 1);
                water_goal_output.setString(water_goal_input);
              }
            }
          }
           else if (event.text.unicode == 27) {
            water_goal_string2.clear();
            water_goal_string.clear();
            water_goal_input.clear();
            display_settings_box = false;
            display_water_goal = false;
            water_goal_enter = false;
          }
        }
      }


// CHECKING MOUSE POSITION/MOUSE CLICKS ON SHAPES

// Check if in the water button region
      if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::FloatRect bounds = water_button.getGlobalBounds();
        if (bounds.contains(mouse)) {
          flash_enter_water = true;
          display_water_box = true;
          water_enter = true;
        }
      }


// check if in the water entry pop-up
      if(water_enter) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
          sf::FloatRect bounds = water_box.getGlobalBounds();
          if (bounds.contains(mouse)) {
            display_water_box = false;
            water_enter = false;

          }
        }
      }
      water_button.setFillColor(sf::Color::White);

// Check if in clear water area
      if(clear_button.isMouseOver(window)) {
        clear_button.setFillColor(sf::Color::Magenta);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          clear_button.setFillColor(sf::Color::Red);
          flash_clear_water = true;
          total_water = 0;
          percent_water = 0; // placeholder for clear button
          if (water_consumed.size() < 1) {
            update_water("0", total_water, percent_water, watergoal);
            string water_string = to_string(percent_water * 100);
            water_string = water_string.substr(0, 5);
            perc_water_text.setString(water_string + " Percent of Goal");
          } else {
            update_water(water_consumed, total_water, percent_water, watergoal);

          }

        }
      }
      clear_button.setFillColor(sf::Color::White);

// Check if in add event area
      if(event_add.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          enter_event_bool = true;
        }
      }

// Check if in settings area
      if(settings.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          display_settings_box = true;
          display_water_goal = true;
          water_goal_enter = true;

        }
      }

// Check if in add event button
      if(confirm_event_bool) {
        if(yes.isMouseOver(window)) {
          event_add.setFillColor(sf::Color::Magenta);
          if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            enter_event_bool = false;
            confirm_event_bool = false;
            add_event_bool = true;

          }
        }
      }

// Check if in do not add event
      if(confirm_event_bool) {
        if(no.isMouseOver(window)) {
          if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            enter_event_bool = false;
            confirm_event_bool = false;
          }
        }
      }

// Check if in go forward a week region
      if(forward_week.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          increase_by_week = true;
        }
      }

// Check if in go back a week region
      if(back_week.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          reduce_by_week = true;
        }
      }

// Check if water popup is closed
      if(checker) {
        if(close.isMouseOver(window)) {
          if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            checker = false;
          }
        }
      }

// Check if event reminder popup is closed
      if(show_event_count) {
        if(close_event_pop.isMouseOver(window)) {
          if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            show_event_count = false;
          }
        }
      }

// check is error message is closed
      if(error_bool) {
        if(close_error.isMouseOver(window)) {
          if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            error_bool = false;
          }
        }
      }

// Close and save the calendar files
      if (event.type == sf::Event::Closed) {
        std::ofstream read_into_calendar("calendar.txt");
        for (auto event : calendar) {
          dashed = add_dashes(event.description);
          read_into_calendar << event.date << " " << event.time << " " << dashed << "\n";
        }
        window.close();
      }

    }

    window.display();
  }



  return 0;
}

