#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

class Resources
{
public:
    static Resources& getInstance()
    {
        static Resources s;
        return s;
    }
    void load();
    const sf::Font& getFont()const;

private:
    Resources() = default;
    sf::Font m_font;
};

class Events
{
public:
    std::vector<sf::Event>::const_iterator begin()const;
    std::vector<sf::Event>::const_iterator end()const;

    void load(sf::RenderWindow& window);

private:
    std::vector<sf::Event> m_events;

};

class Button : public sf::Drawable, public sf::Transformable
{
public:
    Button();
    void setText(const std::string& text);
    void setRectSize(const sf::Vector2f& size);
    void setRectColor(sf::Color color);
    bool isHovered(const sf::RenderWindow& window)const;
    bool isPressed(const sf::RenderWindow& window)const;
    bool isClicked(const Events& events, const sf::RenderWindow& window)const;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;

    void centerText();
};


class CalculatorEngine
{
public:

    enum class State : uint8_t
    {
        Result = 0, 
        Plus,
        Minus,
        Divide,
        Multiply,

    };
    void onKeyPressed(const std::string& key);
    std::string getText()const;
private:
    std::string m_left_number = "0", m_right_number = "";
    State m_state = State::Result;
    
};

