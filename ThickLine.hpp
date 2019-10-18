#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class sfLine : public sf::Drawable
{
public:
    sfLine(const sf::Vector2f &point1, const sf::Vector2f &point2) : color(sf::Color::White), thickness(2.f), mpoint1(point1), mpoint2(point2)
    {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i = 0; i < 4; ++i)
            vertices[i].color = color;
    }
    sfLine() : color(sf::Color::White), thickness(2.f)
    {
        for (int i = 0; i < 4; ++i)
            vertices[i].color = color;
    }
    void SetPoints(const sf::Vector2f &point1, const sf::Vector2f &point2)
    {
        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;
    }
    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(vertices, 4, sf::Quads);
    }

    void setPoint1(const sf::Vector2f &point1)
    {
        sf::Vector2f direction = mpoint2 - point1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = mpoint2 + offset;
        vertices[2].position = mpoint2 - offset;
        vertices[3].position = point1 - offset;
    }

    void setPoint2(const sf::Vector2f &point2)
    {
        sf::Vector2f direction = point2 - mpoint1;
        sf::Vector2f unitDirection = direction / std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y, unitDirection.x);

        sf::Vector2f offset = (thickness / 2.f) * unitPerpendicular;

        vertices[0].position = mpoint1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = mpoint1 - offset;
    }

private:
    sf::Vertex vertices[4];
    float thickness;
    sf::Color color;
    sf::Vector2f mpoint1;
    sf::Vector2f mpoint2;
};