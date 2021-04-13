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




int main(){

sf::RenderWindow window(sf::VideoMode(1400, 700), "Calendar");

// For the calendar shapes
vector<sf::RectangleShape> rect_vec;
sf::RectangleShape rectangle;
rectangle.setSize(sf::Vector2f(200, 697));
rectangle.setFillColor(sf::Color::Red);
rectangle.setOutlineThickness(1);
rectangle.setPosition(2,2);

// First day to copy 
sf::Text Sunday;
sf::Font font;
font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-BI.ttf");
Sunday.setFont(font);
Sunday.setString("Sunday");
Sunday.setFillColor(sf::Color::Black);
Sunday.setOutlineColor(sf::Color::White);
Sunday.setOutlineThickness(1);
Sunday.setPosition(12,2);

// Names for the Calendar boxes
vector<string> days = {"Monday",
"Tuesday","Wednesday","Thursday","Friday","Saturday"};
vector<sf::Text> day;
int x = 200;
for (int i = 0; i<days.size();i++){
  day.push_back(Sunday);
  day.at(i).setPosition(12+x,2);
  day.at(i).setString(days.at(i));
  x = x+200;
}

//Creating the Day Boxes
int y = 200;
for (int i = 0;i<6;i++){
  rect_vec.push_back(rectangle);
  rect_vec.at(i).setPosition(2+y,2);
  y= y + 200;
}




window.setFramerateLimit(60);
while(window.isOpen()){


// Draw the boxes
window.draw(rectangle);
for (int i = 0; i < rect_vec.size();i++)
  window.draw(rect_vec.at(i));

// Draw the names
window.draw(Sunday);
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






