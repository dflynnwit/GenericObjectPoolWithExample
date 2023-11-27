#ifndef GENERIC_OBJECT_POOL_H
#define GENERIC_OBJECT_POOL_H

#include <vector>
#include <memory>
#include <functional>
#include <SFML/Graphics.hpp>

template<typename T>
class GenericObjectPool {
public:
    using ObjectInitializer = std::function<void(T&)>;

    void setDataLocalityEnabled(bool enabled) {
        dataLocalityEnabled = enabled;
    }

    explicit GenericObjectPool(size_t poolSize, bool debugMode = true)
            : poolSize(poolSize), debugMode(debugMode), currentObjectIndex(0), dataLocalityEnabled(true) {
        for (size_t i = 0; i < poolSize; ++i) {
            pool.emplace_back(std::make_unique<T>());
        }
    }

    void drawPoolUsage(sf::RenderWindow& window) const {
        if (!debugMode) return;

        const float barWidth = 200.0f;
        const float barHeight = 30.0f;
        const float barX = window.getSize().x - barWidth - 10; // 10px padding
        const float barY = 10; // 10px from the top
        size_t activeCount = std::count_if(pool.begin(), pool.end(), [](const auto& obj) { return obj->properties.isActive; });
        float activeWidth = static_cast<float>(activeCount) / poolSize * barWidth;

        // Draw active part (green)
        sf::RectangleShape activeBar(sf::Vector2f(activeWidth, barHeight));
        activeBar.setPosition(barX, barY);
        activeBar.setFillColor(sf::Color::Green);
        window.draw(activeBar);

        // Draw inactive part (red)
        sf::RectangleShape inactiveBar(sf::Vector2f(barWidth - activeWidth, barHeight));
        inactiveBar.setPosition(barX + activeWidth, barY);
        inactiveBar.setFillColor(sf::Color::Red);
        window.draw(inactiveBar);


    }

    T* createObject(const std::function<void(ParticleProperties&)>& initializer){
    //T* createObject(const ObjectInitializer& initializer) {
        if (dataLocalityEnabled) {
            for (auto& obj : pool) {
                if (!obj->properties.isActive) {
                    initializer(obj->properties);
                    obj->properties.isActive = true;
                    return obj.get();
                }
            }
        } else {
            for (size_t i = 0; i < pool.size(); ++i) {
                currentObjectIndex = (currentObjectIndex + 1) % pool.size();
                if (!pool[currentObjectIndex]->properties.isActive) {
                    initializer(pool[currentObjectIndex]->properties);
                    pool[currentObjectIndex]->properties.isActive = true;
                    return pool[currentObjectIndex].get();
                }
            }
        }
        return nullptr; // No available object
    }

    /**
     * @brief Updates all active objects in the pool.
     *
     * Iterates over the pool and updates each active object.
     *
     * @param dt Delta time for update calculations.
     */
    void updateObjects(float dt) {
        for (auto& obj : pool) {
            if (obj->properties.isActive) {
                obj->update(dt);
            }
        }
    }

    size_t getSize() const {
        return pool.size();
    }

    T& get(size_t index) {
        return *pool[index];
    }

    void sortObjects() {
        std::sort(pool.begin(), pool.end(), [](const auto& a, const auto& b) -> bool {
            return a->properties.lifetime > b->properties.lifetime;
        });
    }

    void drawDefragmentationDisplay(sf::RenderWindow& window) {
        const float barWidth = 200.0f;
        const float barHeight = 30.0f;
        const float barX = window.getSize().x - barWidth - 10;
        const float barY = 10;
        const float segmentWidth = barWidth / pool.size();

        for (size_t i = 0; i < pool.size(); ++i) {
            sf::RectangleShape segment(sf::Vector2f(segmentWidth, barHeight));
            segment.setPosition(barX + i * segmentWidth, barY);
            segment.setFillColor(pool[i]->properties.isActive ? sf::Color::Green : sf::Color::Red);
            window.draw(segment);
        }
    }

private:
    std::vector<std::unique_ptr<T>> pool;
    size_t poolSize;
    bool debugMode;
    bool dataLocalityEnabled = true;
    size_t currentObjectIndex = 0;
};

#endif // GENERIC_OBJECT_POOL_H
