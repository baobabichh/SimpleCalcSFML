#include "MyLib.h"



Button createButton(const sf::Vector2f& position, const std::string& text)
{
    Button b;
    b.setRectSize({ 100,100 });
    b.setText(text);
    b.setPosition(position);

    return b;
}

sf::Text createText()
{
    sf::Text text;
    text.setFont(Resources::getInstance().getFont());
    text.setString("");
    text.setCharacterSize(50);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::White);

    return text;
}


int main()
{
    CalculatorEngine calc_engine;

    Resources::getInstance().load();
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");

    std::map<std::string, Button> buttons;
    buttons["1"] = createButton({ 10,10 }, "1");
    buttons["2"] = createButton({ 120,10 }, "2");
    buttons["3"] = createButton({ 230,10 }, "3");
    buttons["4"] = createButton({ 10,120 }, "4");
    buttons["5"] = createButton({ 120,120 }, "5");
    buttons["6"] = createButton({ 230,120 }, "6");
    buttons["7"] = createButton({ 10,230 }, "7");
    buttons["8"] = createButton({ 120,230 }, "8");
    buttons["9"] = createButton({ 230,230 }, "9");
    buttons["0"] = createButton({ 120,340 }, "0");

    buttons["+"] = createButton({ 340,10 }, "+");
    buttons["-"] = createButton({ 340,120 }, "-");
    buttons["/"] = createButton({ 340,230 }, "/");
    buttons["*"] = createButton({ 340,340 }, "*");
    buttons["="] = createButton({ 340,450 }, "=");

    buttons["CE"] = createButton({ 10,450 }, "CE");
    buttons["C"] = createButton({ 120,450 }, "C");

    sf::Text result_text = createText();
    result_text.setPosition({ 450, 10 });
   

    Events events;

    while (window.isOpen())
    {
        events.load(window);

        for (const auto& event : events)
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        for (auto& [key, b] : buttons)
        {
            if (b.isHovered(window))
            {
                b.setRectColor(sf::Color::Green);

                if (b.isPressed(window))
                {
                    b.setRectColor(sf::Color::Red);
                }
            }
            else
            {
                b.setRectColor(sf::Color::White);
            }

            if (b.isClicked(events, window))
            {
                std::cout << "Clicked " << key << "\n";
                if (key == "C")
                {
                    result_text.setString("");
                }
                else
                {
                    result_text.setString(result_text.getString() + key);
                }
                calc_engine.onKeyPressed(key);
                result_text.setString(calc_engine.getText());
            }

        }


        window.clear();
        for (const auto& [key, button] : buttons)
        {
            window.draw(button);
        }
        window.draw(result_text);
        window.display();
    }

    return 0;
}