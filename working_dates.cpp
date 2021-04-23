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
#include "boost/date_time/posix_time/posix_time.hpp"
#define SECONDS_IN_DAY 86400;
using std::cin;
using std::cout;
using std::to_string;
using std::string;
using std::stoi;
using std::vector;
using std::stringstream;
using std::time_t;
using namespace boost::posix_time;
using namespace boost::gregorian;


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

const int watergoal = 16;

// Seven day period for checking valid displayable events
class day_period_7 : public time_period {
 public:
  day_period_7(date d) : time_period(ptime(d),
                                       ptime(d, hours(24 * 7)))
  {}

};

class Event {
 public:
  string date,
         time,
         description;

  boost::gregorian::date d; // for checking if the event is in the current 7 day period
 float button_spacing;
  float buttonx;
  float buttony;
// placeholder, difference between time/24
  Button b;

};

void create_event(string event_date, string event_time, string event_desc, Event& a) {
// store the user inputs into the event
  a.date = event_date;
  a.time = event_time;
  a.description = event_desc;

// standard width of each calendar event
  a.buttonx = 150;

// Calculating the relative length of the calendar event
  vector<string> dates;

  stringstream split_date(event_date);

  while(split_date.good()) {
    string d;
    getline(split_date, d, '/');
    dates.push_back(d);
  }

  string usable_date = dates.at(2) + "-" + dates.at(0) + "-" + dates.at(1);
  date d(from_simple_string(usable_date));
  a.d = d; // gets the date in the correct format for checking against the current time period


  vector<string> times;

  stringstream split_time(event_time);

  while(split_time.good()) {
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

  int time_dif = second_time_as_seconds - first_time_as_seconds;

  a.buttony = 250 * (float)time_dif / SECONDS_IN_DAY;

  a.button_spacing = 250*(float)first_time_as_seconds/SECONDS_IN_DAY// needs to be a percentage of the final calendar box

// creating the button for the calendar event
  Button b(event_desc, {a.buttonx, a.buttony}, 15, sf::Color::Black);
  a.b = b;
//// needs to be caluclated




}

bool check_date(string date) {
  int month,
      day,
      year;
  vector<string> dates;
  bool leapyear;



  stringstream split_date(date);

  while(split_date.good()) {
    string d;
    getline(split_date, d, '/');
    dates.push_back(d);
  }

  month = stoi(dates.at(0));
  day = stoi(dates.at(1));
  year = stoi(dates.at(2));
  if(month < 0 || month > 12)
    return 0;
  if(day < 0 || day > 31)
    return 0;
  if(year < 2021 || year > 9999)
    return 0;

  leapyear = gregorian_calendar::is_leap_year(year);

  if(!leapyear & month == 2 & day > 28) // could have a thing that says leap year
    return 0;

  return 1;
}

// function to remoove spaces
string removeSpaces(string str) {
  str.erase(remove(str.begin(), str.end(), ' '), str.end());
  return str;
}


//function to check time input
bool check_time(string user_time) {
  vector<string> vec, vec_first, vec_second;
  stringstream ss(user_time);
  string line;
  // split string at '-'
  while(std::getline(ss, line, '-')) {
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

  // make sure there is an hour and min input
  if (vec_first.size() != 2)
    return 0;
  first_hour = stoi(vec_first.at(0));
  first_min = stoi(vec_first.at(1));
  // check appropiate input
  if (first_hour < 0 || first_hour > 23)
    return 0;
  if (first_min < 0 || first_min > 59)
    return 0;

  // do the same thing to end time
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

// check that the start time is before end time
  if (first_hour > second_hour)
    return 0;

  return 1;
}




void update_water(string w, float& u, float& j) {

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

bool check_in_period_by_day(boost::gregorian::date local_time, date d, int i){
  ptime t(local_time);
  ptime x(local_time,hours(24)*(i+1));
  time_period g(t,x);
  ptime work(d);
  if (g.contains(work)) {
    return 1;
  }
  return 0;
}
vector<string> set_calendar_date(boost::gregorian::date local_time){
    vector<string> times;
    for (int i = 0;i<8;i++){
     ptime t(local_time, hours(24)*(i+1));
     string s = to_simple_string(t);
     times.push_back(s.substr(5,3)+" "+to_string(stoi(s.substr(9))));
  }
  return times;
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
  sf::Text confirm_event_text;
  Event calendar_event;
  vector<Event> calendar;


  date local_time(day_clock::local_day()); //get local time to set calendar date

// read in the files here and check which are in period


  // Load in the state of water
  std::ifstream water_in ("water_state.txt"); // need a save file for day closed, compare to read in and reset water if different
  if (water_in.is_open()) {
    while ( getline (water_in, load_water) ) {
      /*      cout << load_water;*/
    }
    water_in.close();
    if (load_water.size() > 0)
      total_water = total_water + std::stof(load_water);
  }



  percent_water = total_water / watergoal;



  sf::RenderWindow window(sf::VideoMode(app_width, app_length), "Calendar");

// TEXTURES & FONTS
///////////////////////////////////////////////////////////////
  sf::Font font;
  font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");

  sf::Texture wood_background; // Background for weekdays
  if (!wood_background.loadFromFile("wood.jpg")) {
    cout << "didnt work\n";
  };
  wood_background.setSmooth(true);


  sf::Texture app_background; // Background for app
  if (!app_background.loadFromFile("pastel.jpg")) {
    cout << "didnt work\n";
  };
  app_background.setSmooth(true);

  sf::Texture water_texture; // Water progress bar
  if (!water_texture.loadFromFile("water.jpg", sf::IntRect(0, 120, 480, 240))) {
    cout << "didnt work\n";
  };
  water_texture.setSmooth(true);

  sf::Texture button_texture; // Button for water
  if (!button_texture.loadFromFile("button.jpg", sf::IntRect(60, 60, 240, 220))) {
    cout << "didnt work\n";
  };
  button_texture.setSmooth(true);

  sf::Texture clear_button_t; // Button for water
  if (!clear_button_t.loadFromFile("b.png")) {
    cout << "didnt work\n";
  };
  clear_button_t.setSmooth(true);

  sf::Texture set_texture; // Button for water
  if (!set_texture.loadFromFile("setting.png")) {
    cout << "didnt work\n";
  };
  set_texture.setSmooth(true);

// "Icon made by Freeplk from www.flaticon.com"
  sf::Texture plus_event; // Button for water
  if (!plus_event.loadFromFile("plus.png")) {
    cout << "didnt work\n";
  };
  plus_event.setSmooth(true);

  sf::Texture yes_add; // add event
  if (!yes_add.loadFromFile("yes.png")) {
    cout << "didnt work\n";
  };
  yes_add.setSmooth(true);

  sf::Texture no_add; // dont add event
  if (!no_add.loadFromFile("no.png")) {
    cout << "didnt work\n";
  };
  no_add.setSmooth(true);

  sf::Texture new_plus;
  if (!new_plus.loadFromFile("Pluspink.png")) {
    cout << "didnt work\n";
  };
  new_plus.setSmooth(true);
  
/////////////////////////////////////////////////////////////

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


// For the calendar shapes
  vector<sf::RectangleShape> rect_vec;
  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(rec_width, rec_length));
  rectangle.setOutlineThickness(1);
  rectangle.setPosition(rec_x, rec_y);
  rectangle.setTexture(&wood_background);


// Button to clear the current water count
  Button clear_button("Clear Water", { 100, 100 }, 15, sf::Color::Black);
  clear_button.setFont(font);
  clear_button.setTexture(clear_button_t);
  clear_button.setFillColor(sf::Color::Green);
  clear_button.setPosition({app_width - rec_x * 2 - 20, rec_length + rec_y + 110 });

// Setting button
  Button settings("", {100, 100}, 0, sf::Color::Black);
  settings.setFont(font);
  settings.setTexture(set_texture);
  settings.setPosition({1300, 600});

// Confirm event button
  Button confirm_event("", {400, 250}, 0, sf::Color::Black);
  confirm_event.setFont(font);
  confirm_event.centerScreen(window);

// Event plus button
  Button event_add("", {50, 50}, 0, sf::Color::Black);
  event_add.setTexture(plus_event);
  event_add.setPosition({90, 600});

  /*Button event_add_back("", {60, 60}, 0, sf::Color::Red);
  event_add_back.setFillColor(sf::Color::Transparent);
  event_add_back.setOutlineColor(sf::Color::Red);
  event_add_back.setPosition({85,595});
*/
// confirm event yes
  Button yes("", {50, 50}, 0, sf::Color::Black);
  yes.setTexture(yes_add);
  yes.setPosition({550, 250});

// confirm event no
  Button no("", {50, 50}, 0, sf::Color::Black);
  no.setTexture(no_add);
  no.setPosition({800, 250});


// Names for the Calendar boxes
  vector<string> days = set_calendar_date(local_time);
  vector<sf::Text> day;
// First day to copy
  int day_x = rec_x + 50;
  int day_y = 50;
  sf::Text day_text;
  day_text.setFont(font);
  day_text.setString(days.at(0));
  day_text.setFillColor(sf::Color::Black);
  day_text.setOutlineColor(sf::Color::White);
  day_text.setOutlineThickness(1);
  day_text.setPosition(day_x, day_y);

  int x = day_x + 50; // spacing on these needs to be calculated
  for (int i = 0; i < days.size(); i++) {
    day.push_back(day_text);
    day.at(i).setPosition(150 + x, day_y);
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

// create directions to add event
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

// add line under user input
  sf::RectangleShape line(sf::Vector2f(800, 2));
  line.setOutlineColor(sf::Color::Black);
  line.setFillColor(sf::Color::Black);
  line.setPosition(180, 670);

// show error by adding a "XX" to the user line
  /*sf::Text input_error_text;
  input_error_text.setString("XX");
  input_error_text.setFillColor(sf::Color::Red);
  input_error_text.setOutlineColor(sf::Color::Red);
  input_error_text.setFont(font);
  input_error_text.setOutlineThickness(1);
  input_error_text.setPosition(180,640);
  input_error_text.setCharacterSize(25);*/
  


// add textbox
  string input_text;
  sf::Text text("", font);
  sf::Clock clock;

/// Box to show user what input of water is
  sf::RectangleShape water_box;
  water_box.setSize(sf::Vector2f(100, 100));
  water_box.setFillColor(sf::Color::Blue);
  water_box.setOutlineThickness(1);
  water_box.setPosition(700, 350);

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



// Settings text box
  /*  Textbox settings_textbox(100,sf::Color::Red,1);
    settings_textbox.setFont(font);
    settings_textbox.setPosition({700,700});*/
// DISPLAY BOOLEANS
//////////////////////////////////////
  bool display_water_box = false;
  bool disp_text = false;
  bool water_enter = false;
  bool display_settings_box = false;
  bool flash_clear_water = false;
  bool enter_event_bool = false;
  bool confirm_event_bool = false;
  bool add_event_bool = false;

///////////////////////////////////////////
  window.setFramerateLimit(60);

  while(window.isOpen()) {
    sf::Event event;
    if(total_water >= 0) { // moving progress bar; needs a timer to look nice; could have count to check days you've met goal;
      if(total_water >= watergoal)
        water_bar.setSize(sf::Vector2f(water_bar_length, 50));
      else
        water_bar.setSize(sf::Vector2f(percent_water * water_bar_length, 50));
    }
//DRAWING SHAPES
///////////////////////////////////////////////////
    /*    window.draw(confirm_event);*/
// background image
    window.draw(background);
    window.draw(water_bar); // need to link this to the water input, cover up watergoal-remaining for a progress bar

// Draw the boxes
    window.draw(rectangle);
    for (int i = 0; i < rect_vec.size(); i++)
      window.draw(rect_vec.at(i));

// Draw the names
    window.draw(day_text);
    for (int i = 0; i < day.size(); i++)
      window.draw(day.at(i));


// Settings butotn
    settings.drawTo(window);

// water button
    window.draw(water_button);

// water bar outline
    window.draw(water_bar_outline);

// add event button
    event_add.drawTo(window);
    /*event_add_back.drawTo(window);*/

// draw text: add_event & boxes
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

    // clear button
    clear_button.drawTo(window);
    // Flash color on click of clear button
    if(flash_clear_water) {
      clear_button.setFillColor(sf::Color::Green);
      cout << "here\n";
      flash_clear_water = false;
    }

// Draw input for event to window
    static sf::Time text_effect_time;
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


    if(add_event_bool) {
      /*      cout<<event_count;*/
      create_event(event_date, event_time, event_desc, calendar_event);
      calendar_event.b.setFont(font);
      calendar_event.b.setTextFill(sf::Color::Black);
      if(check_in_period(local_time, calendar_event.d)){

       if(check_in_period_by_day(local_time,calendar_event.d,0))
        calendar_event.b.setPosition({rec_x,80+calendar_event.button_spacing});
      else if(check_in_period_by_day(local_time,calendar_event.d,1))
        calendar_event.b.setPosition({2*(rec_x) + rec_width,80+calendar_event.button_spacing});
      else if(check_in_period_by_day(local_time,calendar_event.d,2))
        calendar_event.b.setPosition({3*(rec_x) + 2*rec_width,80+calendar_event.button_spacing});
      else if(check_in_period_by_day(local_time,calendar_event.d,3))
        calendar_event.b.setPosition({4*(rec_x) + 3*rec_width,80+calendar_event.button_spacing});
      else if(check_in_period_by_day(local_time,calendar_event.d,4))
        calendar_event.b.setPosition({5*(rec_x) + 4*rec_width,80+calendar_event.button_spacing});
      else if(check_in_period_by_day(local_time,calendar_event.d,5))
        calendar_event.b.setPosition({6*(rec_x) + 5*rec_width,80+calendar_event.button_spacing});
      else
        calendar_event.b.setPosition({7*(rec_x) + 6*rec_width,80+calendar_event.button_spacing});

      } // need to check if event is already taken up time wise here
    
      calendar.push_back(calendar_event);
      add_event_bool = false;
    }
    /*
      if(display_settings_box)
        settings_textbox.drawTo(window);*/
if(calendar.size()>0){                        ////NEEDS A TIMER
  for(int i = 0;i<calendar.size();i++)
  calendar.at(i).b.drawTo(window);
}
    // Events
    while(window.pollEvent(event)) {



      //// code for textbox // //timer for after an invalid input that prints out a box saying "invalid input"
      if(enter_event_bool) {
        if(!water_enter) {
          if (event_count == 0) {
            if (event.type == sf::Event::TextEntered) {
              if (event.text.unicode >= 47 & event.text.unicode < 58 |
                  event.text.unicode == 32)
                if(input_text.size()<10)
                input_text += event.text.unicode;
            } else if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::BackSpace) {
                if (!input_text.empty())
                  input_text.pop_back();
              } else if (event.key.code == sf::Keyboard::Return) {
                if(check_date(input_text)) {
                  event_date = input_text;
                  event_count = event_count + 1;
                  cout << input_text << "\n";
                  input_text.clear();
                } else{
                  /*window.draw(input_error_text);
                  sf::sleep(sf::seconds(2));*/
                  input_text.clear();
                  
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
                if(input_text.size()<11)
                input_text += event.text.unicode;
            } else if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::BackSpace) {
                if (!input_text.empty())
                  input_text.pop_back();
              } else if (event.key.code == sf::Keyboard::Return) {
                if(check_time(input_text)) {
                  event_time = input_text;
                  event_count = event_count + 1;
                  cout << input_text << "\n";
                  input_text.clear();
                } else
                  input_text.clear();
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
                if(input_text.length() < 28) // keep the box containing text
                  input_text += event.text.unicode;
              }
            } else if (event.type == sf::Event::KeyPressed) {
              if (event.key.code == sf::Keyboard::BackSpace) {
                if (!input_text.empty())
                  input_text.pop_back();
              } else if (event.key.code == sf::Keyboard::Return) {
                event_desc = input_text;
                event_count = event_count + 1;
                cout << input_text << "\n";
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
          confirm_event_bool = true;
          event_count = 0;
          confirm_event_text.setString("Add this Event?\n" +
                                       event_desc + "\n" + event_date + "\n" + event_time);
        }
      }
/////////////////////////////////////////////////////////////////////
      // escape to close water/ general close everything
      if(water_enter) {
        if(event.type == sf::Event::TextEntered) {
          if (event.text.unicode > 47 & event.text.unicode < 58 | event.text.unicode == 46) {
            tmp = static_cast<char>(event.text.unicode);
            water_consumed.append(tmp); // need a time buffer to prevent double clicks
            cout << water_consumed << "\n";
            water_input += event.text.unicode;
            water_output.setString(water_input);
            disp_text = true;
          } else if (event.text.unicode == 13) {
            if(water_consumed.size() == 0) {
              display_water_box = false;
              disp_text = false;
              water_enter = false;
            }
            if(water_consumed.size() > 0) {
              update_water(water_consumed, total_water, percent_water);
              water_consumed.clear();
              water_input.clear();
              display_water_box = false;
              disp_text = false;
              water_enter = false;
            }
          } else if (event.text.unicode == 8) {
            if(water_consumed.size() > 0) {
              water_consumed.pop_back();
              water_input.erase(water_input.getSize() - 1, 1);
              water_output.setString(water_input);
            }
          }
        }
      }
/////////////////////////////////////////////////////
// Check if in water button region
      if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::FloatRect bounds = water_button.getGlobalBounds();
        if (bounds.contains(mouse)) {
          cout << "you are in water enter\n";
          display_water_box = true;
          water_enter = true;
        }
      }

///////////////////////////////////////////////////////

// check if in pop-up
      if(water_enter) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
          sf::FloatRect bounds = water_box.getGlobalBounds();
          if (bounds.contains(mouse)) {
            cout << "you are in water pop up\n";
            display_water_box = false;
            water_enter = false;

          }
        }
      }

///////////////////////////////////////////////////////
// Check if in clear water area
      if(clear_button.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          clear_button.setFillColor(sf::Color::Red);
          flash_clear_water = true;
          total_water = 0;
          percent_water = 0; // placeholder for clear button
          if (water_consumed.size() < 1)
            update_water("0", total_water, percent_water);
          else
            update_water(water_consumed, total_water, percent_water);
          cout << "You are in water clear button\n";

        }
      }
// Check if in add event area
      if(event_add.isMouseOver(window)) {
        confirm_event.setTexture(new_plus);
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          enter_event_bool = true;
          /*display_settings_box = true;*/
          /* cout <<"You are in add event\n";*/

        }
      }

// Check if in settings area
      if(settings.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          /*display_settings_box = true;*/
          cout << "You are in settings\n";

        }
      }
// Check if in add event button
      if(yes.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          /*display_settings_box = true;*/
          enter_event_bool = false;
          confirm_event_bool = false;
          add_event_bool = true;
          cout << "You are in add button\n";

        }
      }
      if(no.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          /*display_settings_box = true;*/
          cout << "You are in no button\n";
          enter_event_bool = false;
          confirm_event_bool = false;
        }
      }


      if (event.type == sf::Event::Closed) // close
        window.close();

    }

    window.display();
  }
  return 0;
}






