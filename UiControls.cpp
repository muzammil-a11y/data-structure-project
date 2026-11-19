#include "UiControls.h"

Button::Button(sf::Font& font,const std::string& text,sf::Vector2f position,sf::Vector2f size,sf::Color bgColor,sf::Color hoverBgColor):
    label(font,text,20),defaultcolor(bgColor),hoverColor(hoverBgColor){
        
        box.setSize(size);
        box.setPosition(position);
        box.setFillColor(defaultcolor);
        box.setOutlineThickness(1.5f);
        box.setOutlineColor(sf::Color(255,255,255,40));

        label.setFillColor(sf::Color::White);
        setText(text);
    }

    void Button::setText(const std::string& text){
        label.setString(text);
        sf::FloatRect textBounds = label.getLocalBounds();
        sf::Vector2f pos = box.getPosition();
        sf::Vector2f size = box.getSize();

        label.setOrigin({textBounds.position.x +textBounds.size.x/2.f,textBounds.position.y + textBounds.size.y/2.f});

        label.setPosition({pos.x + size.x/2.f,pos.y + size.y/2.f});
    }

    void Button::updateHover(sf::Vector2f mousePos){
        if(box.getGlobalBounds().contains(mousePos)){
            box.setFillColor(hoverColor);
        }
        else{
            box.setFillColor(defaultcolor);
        }
    }

    bool Button::isClicked(sf::Vector2f mousePos)const{
        return box.getGlobalBounds().contains(mousePos);
    }

    void Button::draw(sf::RenderWindow& window)const{
        window.draw(box);
        window.draw(label);
    }


TextBox::TextBox(sf::Font& font,const std::string& placeholder,sf::Vector2f position,sf::Vector2f size,bool numbersOnly,std::size_t MaxLength) : 
    displayText(font,"",18),placeholderText(font,placeholder,18),inputString(""),isSelected(false),numbersOnly(numbersOnly),MaxLength(MaxLength),normalBorderColor(sf::Color(110,115,130)),hoverBorderColor(sf::Color(160,185,225)),activeBorderColor(sf::Color(70,150,255)){

        box.setSize(size);
        box.setPosition(position);
        box.setFillColor(sf::Color(35,38,46));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(normalBorderColor);

        float textY = position.y+(size.y-22.f)/2.f;

        displayText.setPosition({position.x+12.f,textY});
        displayText.setFillColor(sf::Color::White);

        placeholderText.setPosition({position.x+12.f,textY});
        placeholderText.setFillColor(sf::Color(140,145,160));
    }

    void TextBox::handleClick(sf::Vector2f mousePos){
        if(box.getGlobalBounds().contains(mousePos)){
            isSelected=true;
            box.setOutlineColor(activeBorderColor);
        }
        else{
            isSelected=false;
            box.setOutlineColor(normalBorderColor);
        }
    }

    void TextBox::handleTextEntered(char32_t unicode){
        if(!isSelected){
            return;
        }
        if(unicode==8){
            if(!inputString.empty()){
                inputString.pop_back();
                displayText.setString(inputString);
            }
            return;
        }
        if(unicode>=32 && unicode<127){
            if(inputString.size() >=MaxLength){
                return;
            }
            char ch= static_cast<char>(unicode);
            if(numbersOnly && (ch<'0' || ch>'9')){
                return;
            }
            inputString+=ch;
            displayText.setString(inputString);
        }
    }

    void TextBox::updateHover(sf::Vector2f mousePos){
        if(isSelected){
            return;
        }
        if(box.getGlobalBounds().contains(mousePos)){
            box.setOutlineColor(hoverBorderColor);
        }
        else{
            box.setOutlineColor(normalBorderColor);
        }
    }

    std::string TextBox::getText()const{
        return inputString;
    }

    void TextBox::setText(const std::string& text){
        inputString=text;
        displayText.setString(inputString);
    }

    void TextBox::clear(){
        inputString.clear();
        displayText.setString("");
    }

    bool TextBox::isEmpty()const{
        return inputString.empty();
    }

    void TextBox::draw(sf::RenderWindow& window)const{
        window.draw(box);
        if(inputString.empty()){
            window.draw(placeholderText);
        }
        else{
            window.draw(displayText);
        }
    }