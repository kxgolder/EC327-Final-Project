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
#include "Textbox.h"
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
const int water_bar_length = app_width - 2 * rec_x - 80;

const int watergoal = 16;


class Event {
 public:
  string day;
  string description;

};

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
  std::ofstream water_state("water_state.txt");
  water_state << u;
  water_state.close();
}





int main() {
  float total_water = (float)0;
  float percent_water = (float)0;
  float per_bar = (float)0;
  string load_water;
  string water_consumed;
  string tmp;

  // Load in the state of water
  std::ifstream water_in ("water_state.txt"); // need a save file for day closed, compare to read in and reset water if different
  if (water_in.is_open()) {
    while ( getline (water_in, load_water) ) {
      cout << load_water;
    }
    water_in.close();
    if (load_water.size() > 0)
      total_water = total_water + std::stof(load_water);
  }



  percent_water = total_water / watergoal;



  sf::RenderWindow window(sf::VideoMode(app_width, app_length), "Calendar");

// TEXTURES
///////////////////////////////////////////////////////////////

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

// Event plus button
  Button event_add("", {50, 50}, 0, sf::Color::Black);
  event_add.setTexture(plus_event);
  event_add.setPosition({1000, 450});

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

// create directions to add event
  sf::Text add_event;
  add_event.setFont(font);
  add_event.setString("Add Event: Enter Day, Time (with am/pm), and Event in the Textbox below, separated by commas");
  add_event.setCharacterSize(20);
  add_event.setFillColor(sf::Color::Black);
  add_event.setPosition(60, 470);
  sf::Text add_info;
  add_info.setFont(font);
  add_info.setString("Example: Monday, 12-3 pm, Tennis Practice");
  add_info.setCharacterSize(20);
  add_info.setFillColor(sf::Color::Black);
  add_info.setPosition(180, 510);

// add line under user input
  sf::RectangleShape line(sf::Vector2f(800, 2));
  line.setOutlineColor(sf::Color::Black);
  line.setFillColor(sf::Color::Black);
  line.setPosition(180, 590);


// add textbox
  string input_text;
  sf::Text text("", font);
  sf::Clock clock;

/// Box to show user what input of water is
  sf::RectangleShape water_box;
  water_box.setSize(sf::Vector2f(100, 100));
  water_box.setFillColor(sf::Color::Blue);
  water_box.setOutlineThickness(1);
  water_box.setPosition(900, 200);

// Text user inputs in water box
  sf::String water_input;
  sf::Text water_output;
  water_output.setPosition(1000, 400);
  water_output.setFillColor(sf::Color::Red);
  water_output.setFont(font);
  water_output.setFillColor(sf::Color::Black);
  water_output.setOutlineColor(sf::Color::White);
  water_output.setOutlineThickness(1);

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
// background image
    window.draw(background);
    window.draw(water_bar); // need to link this to the water input, cover up watergoal-remaining for a progress bar

// Draw the boxes
    window.draw(rectangle);
    for (int i = 0; i < rect_vec.size(); i++)
      window.draw(rect_vec.at(i));

// Draw the names
    window.draw(Sun);
    for (int i = 0; i < day.size(); i++)
      window.draw(day.at(i));


// Settings butotn
    settings.drawTo(window);

// water button
    window.draw(water_button);

// add event button
    event_add.drawTo(window);

// draw text: add_event & boxes
    window.draw(add_event);
    window.draw(add_info);
    window.draw(line);

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
    text.setPosition(190, 550);
    text.setFillColor(sf::Color::Blue);
  }

   

    /*
      if(display_settings_box)
        settings_textbox.drawTo(window);*/

    window.display();
    // Events
    while(window.pollEvent(event)) {



      //// code for textbox // should this be the standard text entry?
      if(enter_event_bool) {
        if(!water_enter) {
          if (event.type == sf::Event::TextEntered) {
            if (std::isprint(event.text.unicode))
              input_text += event.text.unicode;
          } else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::BackSpace) {
              if (!input_text.empty())
                input_text.pop_back();
            }
            if (event.key.code == sf::Keyboard::Return) {
              cout << input_text << "\n";
              input_text.clear();
              enter_event_bool = false;
            }
          }
        }
      }
/////////////////////////////////////////////////////////////////////
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
          cout << "hi\n";
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
            cout << "hi\n";
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
          cout << "You are here\n";

        }
      }
// Check if in add event area
      if(event_add.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          enter_event_bool = true;
          /*display_settings_box = true;*/
          cout << "You are here\n";

        }
      }

// Check if in settings area
      if(settings.isMouseOver(window)) {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          /*display_settings_box = true;*/
          cout << "You are here\n";

        }
      }






      if (event.type == sf::Event::Closed) // close
        window.close();


    }
/*     window.display();*/
  }
  return 0;
}






