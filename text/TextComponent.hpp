#ifndef __TextComponent_hpp_2018_07_10_09_58
#define __TextComponent_hpp_2018_07_10_09_58
#include <string>
#include <json/json.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace graphics{
    class Image;//Forward Declare
};

namespace text{
    
    enum class Color{
        Black, DarkBlue, DarkGreen, DarkAqua,
        DarkRed, DarkPurple, Gold, Gray,
        DarkGray, Blue, Green, Aqua,
        Red, Purple, Yellow, White
    };
    class RGB{
        private:
            uint8_t r;
            uint8_t g;
            uint8_t b;
        public:
            RGB(uint8_t,uint8_t,uint8_t);
            RGB(Color);
            operator sf::Color();
            uint8_t getRed();
            uint8_t getGreen();
            uint8_t getBlue();
        };
    namespace detail{
        RGB colorMap[] = 
        {
            RGB(0,0,0),RGB(0,0,128),RGB(0,100,0),RGB(0,139,139),
            RGB(139,0,0),RGB(75,0,130),RGB(218,165,32),RGB(128,128,128),
            RGB(64,64,64),RGB(0,0,255),RGB(0,255,0),RGB(0,255,255),
            RGB(255,0,0),RGB(128,0,128),RGB(255,255,0),RGB(255,255,255)
        };
    };

    struct Style final{
        Color color;
        bool bold;
        bool underline;
        bool strikethrough;
        bool italics;
        Style();
    };

    struct translate_t{};

    const translate_t translate{};

    class TextComponent final:public sf::Drawable{
    public:
        enum TextComponentType{
            Raw,Formatted,Translateable,
            Argument, Image,Newline
        };
    private:
        TextComponent* next;
        string text;
        string type;
        Style formatting;
        int argumentOrX;
        int y;
        int width;
        int height;
    public:
        TextComponent(const string&);
        TextComponent();
        TextComponent(translate_t,const string&);
        TextComponent(const graphics::Image&,int=0,int=0,int=-1,int=-1);
        TextComponent(const string&,const Style&);
        TextComponent(size_t);
        TextComponent(const Json::Value&);
        ~TextComponent();
        void draw(sf::RenderTarget&,sf::RenderStates)const;
        TextComponent format(TextComponent[],size_t,size_t&);
        operator Json::Value()const;
        Json::Value toJson()const;
        bool hasChild()const;
        TextComponent& getChild()const;
        void appendChild(const TextComponent&);
    };
    class I18NNode{
    private:
        map<string,I18NNode> nodes;
        TextComponent component;
        bool hasNodes;
        bool hasComponent;
    public:
        I18NNode();
        I18NNode(const Json::Value&);
        const TextComponent& getComponent()const;
        const I18NNode& getNode(const string&)const;
        operator const TextComponent&()const;
        const I18NNode& operator[](const string&)const;
    };
    class I18N final:private I18NNode{
    private:
        string name;
    public:
        I18N(const Json::Value&);
        const TextComponent& translate(const TextComponent&)const;
    };
};
#endif