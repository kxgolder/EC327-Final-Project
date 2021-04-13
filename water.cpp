#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <ctime>
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
const float rec_x = (float)350/calendar_spaces;
const int rec_y = 40;
const int rec_width = 150;
const int rec_length = 300;

int main(){

sf::RenderWindow window(sf::VideoMode(app_width, app_length), "Calendar");


// Textures 
sf::Texture wood_background; // Background for weekdays
if (!wood_background.loadFromFile("wood.jpg")){
  cout << "didnt work\n";
};
wood_background.setSmooth(true);


sf::Texture app_background; // Background for app
if (!app_background.loadFromFile("pastel.jpg")){
  cout << "didnt work\n";
};
app_background.setSmooth(true);

sf::Texture water_texture; // water progress bar
if (!water_texture.loadFromFile("water.jpg",sf::IntRect(0,120,480,240))){
  cout << "didnt work\n";
};
app_background.setSmooth(true);

// Background Shape
sf::RectangleShape background;
background.setSize(sf::Vector2f(app_width,app_length));
background.setTexture(&app_background);

// water progress bar
sf::RectangleShape water_bar;
water_bar.setSize(sf::Vector2f(app_width-2*rec_x,50));
water_bar.setPosition(rec_x,rec_length+rec_y+40);
water_bar.setTexture(&water_texture);


// For the calendar shapes
vector<sf::RectangleShape> rect_vec;
sf::RectangleShape rectangle;
rectangle.setSize(sf::Vector2f(rec_width, rec_length));
rectangle.setFillColor(sf::Color::Red);
rectangle.setOutlineThickness(1);
rectangle.setPosition(rec_x,rec_y);
rectangle.setTexture(&wood_background);

// First day to copy 
int day_x = rec_x+50;
int day_y = 50;
sf::Text Sun;
sf::Font font;
font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-BI.ttf");
Sun.setFont(font);
Sun.setString("Sun");
Sun.setFillColor(sf::Color::Black);
Sun.setOutlineColor(sf::Color::White);
Sun.setOutlineThickness(1);
Sun.setPosition(day_x,day_y);

// Names for the Calendar boxes
vector<string> days = {"Mon",
"Tue","Wed","Thu","Fri","Sat"};
vector<sf::Text> day;

int x = day_x+50; // spacing on these needs to be calculated
for (int i = 0; i<days.size();i++){
  day.push_back(Sun);
  day.at(i).setPosition(150+x,day_y);
  day.at(i).setString(days.at(i));
  x = x+rec_x+rec_width;
}

//Creating the Day Boxes
float a = rec_width+(float)350/calendar_spaces;
for (int i = 0;i<6;i++){
  rect_vec.push_back(rectangle);
  rect_vec.at(i).setPosition(rec_x+a,rec_y);
  a= a+rec_width+(float)350/calendar_spaces;
}




window.setFramerateLimit(60);
while(window.isOpen()){

// background image
window.draw(background);
window.draw(water_bar); // need to link this to the water input, cover up 16-remaining for a progress bar

// Draw the boxes
window.draw(rectangle);
for (int i = 0; i < rect_vec.size();i++)
  window.draw(rect_vec.at(i));

// Draw the names
window.draw(Sun);
for (int i = 0; i < day.size();i++)
  window.draw(day.at(i));


window.display();


// Check if application is closed
sf::Event event;
  while (window.pollEvent(event)) {
  if (event.type == sf::Event::Closed)
      window.close();
}

}

return 0;
}






