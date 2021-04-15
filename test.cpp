// testing textbook function in SFML

#include <iostream>
#include <SFML/Graphics.hpp>
#include <chrono>

#include "Textbox.h"

int main() {
  sf::RenderWindow window;

  sf::Vector2i centerWindow((sf::VideoMode::getDesktopMode().width / 2) - 445,
    (sf::VideoMode::getDesktopMode().height / 2) - 480);


  window.create(sf::VideoMode(900, 900), "SFML Project", sf::Style::Titlebar | sf::Style::Close);
  window.setPosition(centerWindow);

  window.setKeyRepeatEnabled(true);

  sf::Font font;
  font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-BI.ttf");

  Textbox textbox1(15, sf::Color::White, false);
  textbox1.setFont(font);
  textbox1.setPosition({100,100});

  
  while(window.isOpen()){

    sf::Event Event;

    while(window.pollEvent(Event)) {
      switch (Event.type) {

      case sf::Event::Closed:
        window.close();
      }
    }
    window.clear();
    textbox1.drawTo(window);
    window.display();
  }
}