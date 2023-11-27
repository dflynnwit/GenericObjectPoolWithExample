#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

struct ParticleProperties {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;
    bool isActive;
    float size;
};


class Particle {
public:
    ParticleProperties properties;

    //Particle() : position(0, 0), velocity(0, 0), color(sf::Color::Blue), lifetime(0), initialLifetime(0), size(10.0f), active(false) {}

    Particle() : properties() {}

    void init(const sf::Vector2f& position, const sf::Vector2f& velocity, const sf::Color& color, float lifetime, float size) {
        properties.position = position;
        properties.velocity = velocity;
        properties.color = color;
        properties.lifetime = lifetime;
        properties.isActive = true;
        properties.size = size;
    }
    //void setActive(bool a) { active = a; }
    //bool isActive() const { return active; }

//    void init(const sf::Vector2f& pos, const sf::Vector2f& vel, const sf::Color& col, float life) {
//        position = pos;
//        velocity = vel;
//        color = col;
//        lifetime = life;
//        initialLifetime = life;
//        size = 10.0f;
//        active = true;
//    }

    void update(float dt) {
        if (!properties.isActive) return;
        properties.lifetime -= dt;
        if (properties.lifetime <= 0) {
            properties.isActive = false;
            return;
        }
        properties.position += properties.velocity * dt;
        float lifeRatio = properties.lifetime / properties.lifetime;
        properties.size = 10.0f * lifeRatio;  // Example size update
        // Update other properties (e.g., color) here if needed
    }
};

#endif // PARTICLE_H
