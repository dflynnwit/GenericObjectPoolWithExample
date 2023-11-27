#include <SFML/Graphics.hpp>
#include <cmath>
#include "Particle.h"
#include "GenericObjectPool.h"

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 800;
const int NUM_PARTICLES = 50;
const int MIN_SPEED = 50;
const int MAX_SPEED = 150;
const float MAX_ANGLE = 360.0f;
const float PARTICLE_LIFETIME_BASE = 2.0f;
const float PARTICLE_LIFETIME_VARIANCE = 1.0f;
using namespace std;

bool TestDataLocalitiesEnabled = false; // Initial state is disabled

// Function to redraw the checkbox based on TestDataLocalitiesEnabled state
void drawCheckbox(sf::RectangleShape& checkbox) {
    checkbox.setFillColor(TestDataLocalitiesEnabled ? sf::Color::Green : sf::Color::Red);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle System with Generic Pool");
    GenericObjectPool<Particle> particlePool(4000);

    // Create a checkbox
    sf::RectangleShape checkbox(sf::Vector2f(20, 20));
    checkbox.setPosition(10, 10);
    drawCheckbox(checkbox); // Initial draw based on state


    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        return -1; // Error loading font
    }

    sf::Text checkboxLabel("Data Locality", font, 16);
    checkboxLabel.setPosition(40, 10);
    checkboxLabel.setFillColor(sf::Color::White);


    sf::Text fpsText;
    fpsText.setFont(font); // Assuming you have loaded the font as shown in your code
    fpsText.setCharacterSize(16);
    fpsText.setPosition(10, 40); // Adjust the position as needed
    fpsText.setFillColor(sf::Color::White);

    int frameCount = 0;
    float totalTime = 0.0f;


    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Handle checkbox click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (checkbox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    TestDataLocalitiesEnabled = !TestDataLocalitiesEnabled; // Toggle data locality
                    // Toggle the checkbox's appearance (checked/unchecked)
                    drawCheckbox(checkbox); // Redraw checkbox based on state
                    // checkbox.setFillColor(TestDataLocalitiesEnabled ? sf::Color::Green : sf::Color::Red);
                }
            }
        }

        float dt = clock.restart().asSeconds();

        // Update frame count and total time
        frameCount++;
        totalTime += dt;

        if (totalTime >= 1.0f) {
            // Calculate FPS and update text
            float fps = static_cast<float>(frameCount) / totalTime;
            fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));

            // Reset frame count and total time
            frameCount = 0;
            totalTime = 0.0f;
        }


        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)  ) {
            auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
            if (mousePos.y > 40){
                for (int i = 0; i < NUM_PARTICLES; ++i) {
                float angle = (MAX_ANGLE / NUM_PARTICLES) * i;
                float speed = static_cast<float>(rand() % MAX_SPEED + MIN_SPEED);
                auto velocity = sf::Vector2f(speed * cos(angle * M_PI / 180.0f), -speed * sin(angle * M_PI / 180.0f));
                float lifetime = PARTICLE_LIFETIME_BASE + static_cast<float>(rand() % static_cast<int>(PARTICLE_LIFETIME_VARIANCE * 1000)) / 1000.0f;

                particlePool.createObject([&](ParticleProperties& p) -> void {
                    p.position = mousePos;
                    p.velocity = velocity;
                    p.color = sf::Color::White;
                    p.lifetime = lifetime;
                    p.isActive = true;
                });
             }
            }
        }

        particlePool.updateObjects(dt);

        window.clear();
        for (size_t i = 0; i < particlePool.getSize(); ++i) {
            const ParticleProperties& particle = particlePool.get(i).properties;
            if (particle.isActive) {
                sf::CircleShape shape(particle.size / 2);
                shape.setPosition(particle.position);
                shape.setFillColor(particle.color);
                window.draw(shape);
            }
        }


        // Draw the checkbox and its label
        window.draw(checkbox);
        window.draw(checkboxLabel);

        // Draw the FPS text
        window.draw(fpsText);

        if (TestDataLocalitiesEnabled)
            particlePool.sortObjects();
        particlePool.drawDefragmentationDisplay(window);
        window.display();
    }

    return 0;
}
