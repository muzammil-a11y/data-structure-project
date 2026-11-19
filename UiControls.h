#ifndef UiControls
#define UiControls

#include <SFML/Graphics.hpp>
#include <string>

class Button{

    private:
        sf::RectangleShape box;
        sf::Text label;
        sf::Color defaultcolor;
        sf::Color hoverColor;

    public:

    Button(sf::Font& font, const std::string& text, sf::Vector2f pos, sf::Vector2f size, sf::Color bgcolor =sf::Color(45,110,210),sf::Color hoverBGcolor=sf::Color(70,140,240));

    void updateHover(sf::Vector2f mousePos);
    bool isClicked(sf::Vector2f mousePose)const;
    void setText(const std::string& text);
    void draw(sf::RenderWindow& window)const;

};

class TextBox{

    private:

        sf::RectangleShape box;
        sf::Text displayText;
        sf::Text placeholderText;
        std::string inputString;
        bool isSelected;
        bool numbersOnly;
        std::size_t MaxLength;

        sf::Color normalBorderColor;
        sf::Color hoverBorderColor;
        sf::Color activeBorderColor;

    public:
        TextBox(sf::Font& font, const std::string& placeholder,sf::Vector2f position,sf::Vector2f size,bool numbersOnly = false,std::size_t MaxLength = 30);

        void handleClick(sf::Vector2f mousePos);
        void handleTextEntered(char32_t unicode);
        void updateHover(sf::Vector2f mousePos);

        std::string getText() const;
        void setText(const std::string& text);
        void clear();
        bool isEmpty() const;

        void draw(sf::RenderWindow& window) const;

};

#endif