#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <vector>
#include <math.h>
using std::cin;
using std::cout;
using std::to_string;
using std::string;
using std::stoi;
using std::vector;

const int app_width = 1400;
const int app_length = 700;
const int calendar_spaces = 8;
const int calendar_days = 7;
const float rec_x = (float)350 / calendar_spaces;
const int rec_y = 40;
const int rec_width = 150;
const int rec_length = 300;

/*
void enter_water(float &x){


}
*/

class Event {
 public:
  string day;
  string description;

};

void update_water(string w, float& u) {
  float added_water;
  added_water = std::stof(w);
  added_water = floorf(added_water * 100) / 100;
  u = u + added_water;
}





int main() {

  sf::RenderWindow window(sf::VideoMode(app_width, app_length), "Calendar");
  window.setKeyRepeatEnabled(false);

  float total_water = (float)0;
  string water_consumed;
  string tmp;


// Textures
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

// Background Shape
  sf::RectangleShape background;
  background.setSize(sf::Vector2f(app_width, app_length));
  background.setTexture(&app_background);

// Water progress bar
  sf::RectangleShape water_bar;
  water_bar.setSize(sf::Vector2f(app_width - 2 * rec_x - 80, 50));
  water_bar.setPosition(rec_x, rec_length + rec_y + 40);
  water_bar.setTexture(&water_texture);


// Water button
  sf::CircleShape water_button(40);
  water_button.setTexture(&button_texture);
  water_button.setPosition(app_width - rec_x * 2 - 20, rec_length + rec_y + 30);


// For the calendar shapes
  vector<sf::RectangleShape> rect_vec;
  sf::RectangleShape rectangle;
  rectangle.setSize(sf::Vector2f(rec_width, rec_length));
  /*rectangle.setFillColor(sf::Color::Red);*/
  rectangle.setOutlineThickness(1);
  rectangle.setPosition(rec_x, rec_y);
  rectangle.setTexture(&wood_background);

// First day to copy
  int day_x = rec_x + 50;
  int day_y = 50;
  sf::Text Sun;
  sf::Font font;
  font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf");
  Sun.setFont(font);
  Sun.setString("Sun");
  Sun.setFillColor(sf::Color::Black);
  Sun.setOutlineColor(sf::Color::White);
  Sun.setOutlineThickness(1);
  Sun.setPosition(day_x, day_y);

// Names for the Calendar boxes
  vector<string> days = {"Mon",
                         "Tue", "Wed", "Thu", "Fri", "Sat"
                        };
  vector<sf::Text> day;

  int x = day_x + 50; // spacing on these needs to be calculated
  for (int i = 0; i < days.size(); i++) {
    day.push_back(Sun);
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

// create add event
  sf::Text add_event;
  add_event.setFont(font);
  add_event.setString("Add Event:");
  add_event.setCharacterSize(20);
  add_event.setFillColor(sf::Color::Black);
  add_event.setPosition(60, 470);

// create other labels
// Day
  sf::Text add_day;
  add_day.setFont(font);
  add_day.setString("Day");
  add_day.setCharacterSize(20);
  add_day.setFillColor(sf::Color::Black);
  add_day.setPosition(190, 470);
// Time
  sf::Text add_time;
  add_time.setFont(font);
  add_time.setString("Time");
  add_time.setCharacterSize(20);
  add_time.setFillColor(sf::Color::Black);
  add_time.setPosition(190, 540);
// Event
  sf::Text add_ev;
  add_ev.setFont(font);
  add_ev.setString("Event");
  add_ev.setCharacterSize(20);
  add_ev.setFillColor(sf::Color::Black);
  add_ev.setPosition(190, 610);

// add boxes around textbox
  sf::RectangleShape day_box(sf::Vector2f(300, 40));
  day_box.setOutlineColor(sf::Color::Black);
  day_box.setOutlineThickness(5);
  day_box.setPosition(270, 450);
  sf::RectangleShape time_box(sf::Vector2f(300, 40));
  time_box.setOutlineColor(sf::Color::Black);
  time_box.setOutlineThickness(5);
  time_box.setPosition(270, 520);
  sf::RectangleShape ev_box(sf::Vector2f(300, 40));
  ev_box.setOutlineColor(sf::Color::Black);
  ev_box.setOutlineThickness(5);
  ev_box.setPosition(270, 590);


/// just checking
  sf::RectangleShape rectangle2;
  rectangle2.setSize(sf::Vector2f(rec_width, rec_length));
  rectangle2.setFillColor(sf::Color::Red);
  rectangle2.setOutlineThickness(1);
  rectangle2.setPosition(rec_x, rec_y);

sf::String water_input;
sf::Text water_output;
water_output.setPosition(200,200);
water_output.setFillColor(sf::Color::Red);
water_output.setFont(font);
water_output.setFillColor(sf::Color::Black);
water_output.setOutlineColor(sf::Color::White);
water_output.setOutlineThickness(1);


  bool display_water_box = false;
  bool disp_text = false;
  window.setFramerateLimit(60);

  while(window.isOpen()) {
    sf::Event event;

    // background image
    window.draw(background);
    window.draw(water_bar); // need to link this to the water input, cover up 16-remaining for a progress bar

// Draw the boxes
    window.draw(rectangle);
    for (int i = 0; i < rect_vec.size(); i++)
      window.draw(rect_vec.at(i));

// Draw the names
    window.draw(Sun);
    for (int i = 0; i < day.size(); i++)
      window.draw(day.at(i));


// water button
    window.draw(water_button);

// draw text: add_event, add_day, add_time, add_ev & boxes
    window.draw(add_event);
    window.draw(add_day);
    window.draw(add_time);
    window.draw(add_ev);
    window.draw(day_box);
    window.draw(time_box);
    window.draw(ev_box);

    if(display_water_box)
      window.draw(rectangle2);
    if(disp_text)
  window.draw(water_output);

    while(window.pollEvent(event)) {

/////////////////////////////////////////////////////////////////////
// need to add a boolean that indicates we are in enter water state, keep enter available
      // for the other input events
        if(event.type == sf::Event::TextEntered) {
          if (event.text.unicode > 47 & event.text.unicode < 58 | event.text.unicode == 46) {
            tmp = static_cast<char>(event.text.unicode);
            water_consumed.append(tmp); // need a time buffer to prevent double clicks
            cout << water_consumed << "\n";
            water_input +=event.text.unicode;
            water_output.setString(water_input);
            disp_text = true;
          }
          else if (event.text.unicode == 13) {
            if(water_consumed.size() == 0){
              display_water_box = false;
              disp_text = false;
            }
            if(water_consumed.size()>0){
              update_water(water_consumed, total_water);
              water_consumed.clear();
              display_water_box = false;
              disp_text = false;
            }
          }
          else if (event.text.unicode == 8){
            if(water_consumed.size()>0){
              water_consumed.pop_back();
              water_input.erase(water_input.getSize() - 1, 1);
              water_output.setString(water_input);
        }
        }
}
/////////////////////////////////////////////////////

// check if in pop-up
      if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::FloatRect bounds = rectangle2.getGlobalBounds();
        if (bounds.contains(mouse)) {
          cout << "hi\n";
          display_water_box = false;
        }
      }
///////////////////////////////////////////////////////

// Check if in water button region
      if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        sf::FloatRect bounds = water_button.getGlobalBounds();
        if (bounds.contains(mouse)) {
          cout << "hi\n";
          display_water_box = true;
        }
      }

///////////////////////////////////////////////////////

      if (event.type == sf::Event::Closed)  // close
        window.close();



    }
    window.display();
  }
  cout << total_water << "\n";
  return 0;
}






