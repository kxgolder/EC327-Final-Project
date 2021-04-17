#include <iostream>
#include <SFML/Graphics.hpp>

class Button {
public:
  Button(){

  }

  Button(std::string t, sf::Vector2f size, int charSize, sf::Color bgColor, sf:: Color textColor) {
    text.setString(t);
    text.setOutlineColor(textColor);
    text.setCharacterSize(charSize);

    button.setSize(size);
    button.setFillColor(bgColor);

  }

  void setFont(sf::Font &font) {
    text.setFont(font);
  }

  void setTexture(sf::Texture &texture){
    button.setTexture(&texture);
  }

  void setFillColor(sf::Color color) {
    button.setFillColor(color);
  }

  void setTextColor(sf::Color color) {
    text.setOutlineColor(color);
  }

  void setPosition(sf::Vector2f pos) {
    button.setPosition(pos);

    float xPos = (pos.x + button.getLocalBounds().width / 2) - (text.getLocalBounds().width / 2);
    float yPos = (pos.y + button.getLocalBounds().height / 2) - (text.getLocalBounds().height / 2);
    text.setPosition(xPos,yPos);
  }

  void drawTo(sf::RenderWindow &window) {
    window.draw(button);
    window.draw(text);
  }

  bool isMouseOver(sf::RenderWindow &window) {
    sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    sf::FloatRect bounds = button.getGlobalBounds();

  if (bounds.contains(mouse)) {
    return true;
  }
  return false;
}

private: 
  sf::RectangleShape button;
  sf::Text text;

};