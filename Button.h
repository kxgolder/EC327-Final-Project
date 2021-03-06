#include <iostream>
#include <SFML/Graphics.hpp>

class Button {
 public:
  Button() {

  }

  Button(std::string t, sf::Vector2f size, int charSize, sf:: Color textColor) {
    text.setString(t);
    text.setOutlineColor(textColor);
    text.setCharacterSize(charSize);

    button.setSize(size);

  }
  void setString(std::string t) {
    text.setString(t);
  }

  void setSize(sf::Vector2f size) {
    button.setSize(size);
  }

  void setCharacterSize(int charSize) {
    text.setCharacterSize(charSize);
  }

  void centerOn(Button a) {
    sf::FloatRect a_bounds = a.getGlobalBounds();
    sf::FloatRect button_bounds = button.getGlobalBounds();

    button.setPosition(
      a_bounds.left + (a_bounds.width / 2) - (button_bounds.width / 2),
      a_bounds.top + (a_bounds.height / 2) - button_bounds.height);
  }

  sf::FloatRect getGlobalBounds() {
    return button.getGlobalBounds();
  }

  void centerScreen(sf::RenderWindow &window) {
    int windowBoundsx = window.getSize().x;
    int windowBoundsy = window.getSize().y;
    sf::FloatRect buttonBounds = button.getGlobalBounds();

    button.setPosition(
      0 + (windowBoundsx / 2) - (buttonBounds.width / 2),
      0 + (windowBoundsy / 2) - buttonBounds.height);
  }

  void setColor(sf::Color color) {
    button.setFillColor(color);
  }

  void setOutlineColor(sf::Color color) {
    button.setOutlineColor(color);
  }

  void setOutlineThickness(float thickness) {
    button.setOutlineThickness(thickness);
  }

  void setFont(sf::Font &font) {
    text.setFont(font);
  }

  void setTextFill(sf::Color color) {
    text.setFillColor(color);
  }

  void setTexture(sf::Texture &texture) {
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

    float xPos = (pos.x + button.getGlobalBounds().width / 2) - (text.getGlobalBounds().width / 2);
    float yPos = (pos.y + button.getGlobalBounds().height / 2) - (text.getGlobalBounds().height / 2);
    text.setPosition(xPos, yPos - 2);
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