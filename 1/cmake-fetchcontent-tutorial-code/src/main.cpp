#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

#include "ImGuiMenu.h"
#include "function.h"

triangle T1{

    std::array<vec2, 3>{vec2{100.0f, 200.0f},
                        vec2{500.0f, 700.0f},
                        vec2{200.0f, 100.0f}}

};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();
//this no dont work but insted i wrt the message 
        // if(isColliding(T1,T2))
        //     T2.color=sf::Color::Red;
        // else
        //     T2.color= sf::Color::Green;

        ShowDemoWindowWidgets();

        // draw it
        window.clear();
        window.draw(T1);
        window.draw(T2);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}
