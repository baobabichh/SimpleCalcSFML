#include "MyLib.h"



void Resources::load()
{
	if(!m_font.loadFromFile("times new roman.ttf"))
	{ 
		std::cout << "Failed to load font\n";
	}
}

const sf::Font& Resources::getFont() const
{
	return m_font;
}

Button::Button()
{
	m_text.setFont(Resources::getInstance().getFont());
	m_text.setString("");
	m_text.setCharacterSize(50);
	m_text.setStyle(sf::Text::Bold);
	m_text.setFillColor(sf::Color::Black);
	
	m_shape.setSize({ 200, 200 });
	m_shape.setFillColor(sf::Color::White);

	centerText();
}

void Button::setText(const std::string& text)
{
	m_text.setString(text);

	centerText();
}

void Button::setRectSize(const sf::Vector2f& size)
{
	m_shape.setSize(size);

	centerText();
}

void Button::setRectColor(sf::Color color)
{
	m_shape.setFillColor(color);
}

bool Button::isClicked(const Events& events, const sf::RenderWindow& window) const
{
	for (const auto& event : events)
	{
		if ((event.type == sf::Event::MouseButtonPressed) && (event.mouseButton.button == sf::Mouse::Left) && isHovered(window))
		{
			return 1;
		}
	}
	return 0;
}

bool Button::isHovered(const sf::RenderWindow& window) const
{
	const sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	const sf::Transform& transform = getTransform();
	sf::FloatRect rect = m_shape.getGlobalBounds();
	rect = transform.transformRect(rect);

	return (rect.contains(mouse_pos));
}

bool Button::isPressed(const sf::RenderWindow& window) const
{
	return isHovered(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_shape, states);
	target.draw(m_text, states);
}

void Button::centerText()
{
	auto text_rect = m_text.getLocalBounds();
	auto shape_rect = m_shape.getLocalBounds();

	m_text.setOrigin(text_rect.left + text_rect.width / 2.0f, text_rect.top + text_rect.height / 2.0f);
	m_text.setPosition(shape_rect.left + shape_rect.width / 2.0f, shape_rect.top + shape_rect.height / 2.0f);
}

std::vector<sf::Event>::const_iterator Events::begin() const
{
	return m_events.begin();
}

std::vector<sf::Event>::const_iterator Events::end() const
{
	return m_events.end();
}

void Events::load(sf::RenderWindow& window)
{
	m_events.clear();

	sf::Event ev;
	while (window.pollEvent(ev))
	{
		m_events.push_back(ev);
	}
}

void CalculatorEngine::onKeyPressed(const std::string& key)
{
	static const auto isDigit = [](const std::string& key)
	{
		static std::vector<std::string> digits = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };
		return std::find(std::begin(digits), std::end(digits), key) != std::end(digits);
	};

	if (key == "CE")
	{
		m_left_number = "0";
		m_right_number = "";
		m_state = State::Result;
	}
	else if (State::Result == m_state)
	{
		if (key == "C")
		{
			if (m_left_number.size())
			{
				m_left_number.pop_back();
			}
		}
		else if (key == "+")
		{
			m_state = State::Plus;
		}
		else if (key == "-")
		{
			m_state = State::Minus;
		}
		else if (key == "/")
		{
			m_state = State::Divide;
		}
		else if (key == "*")
		{
			m_state = State::Multiply;
		}
		else if (isDigit(key))
		{
			if (m_left_number == "0")
			{
				m_left_number = key;
			}
			else
			{
				m_left_number += key;
			}
		}
	}
	else if (State::Plus == m_state || State::Minus == m_state || State::Divide == m_state || State::Multiply == m_state)
	{
		if (key == "C")
		{
			m_state = State::Result;
		}
		else if (isDigit(key))
		{
			if (m_right_number == "0")
			{
				m_right_number = key;
			}
			else
			{
				m_right_number += key;
			}
		}
		else if (key == "+")
		{
			m_state = State::Plus;
		}
		else if (key == "-")
		{
			m_state = State::Minus;
		}
		else if (key == "/")
		{
			m_state = State::Divide;
		}
		else if (key == "*")
		{
			m_state = State::Multiply;
		}
		else if (key == "=")
		{
			double left = 0.0;
			double right = 0.0;

			std::istringstream ss(m_left_number);
			ss >> left;

			ss = std::istringstream(m_right_number);
			ss >> right;

			double res = 0.0;

			switch (m_state)
			{
			case CalculatorEngine::State::Plus:
				res = left + right;
				break;
			case CalculatorEngine::State::Minus:
				res = left - right;
				break;
			case CalculatorEngine::State::Divide:
				res = left / right;
				break;
			case CalculatorEngine::State::Multiply:
				res = left * right;
				break;
			default:
				break;
			}

			m_left_number = std::to_string(res);
			m_right_number = "";

			m_state = State::Result;
		}
	}
	

}

std::string CalculatorEngine::getText() const
{
	std::stringstream ss;

	switch (m_state)
	{
	case CalculatorEngine::State::Result:
	{
		ss << m_left_number;
	}
		break;
	case CalculatorEngine::State::Plus:
	{
		ss << m_left_number << " + " << m_right_number;
	}
		break;
	case CalculatorEngine::State::Minus:
	{
		ss << m_left_number << " - " << m_right_number;
	}
		break;
	case CalculatorEngine::State::Divide:
	{
		ss << m_left_number << " / " << m_right_number;
	}
		break;
	case CalculatorEngine::State::Multiply:
	{
		ss << m_left_number << " * " << m_right_number;
	}
		break;
	default:
		break;
	}

	return ss.str();
}
