#include <iostream>
#include <SFML/Graphics.hpp>
#include "Matrix.hpp"
#include "Vector.hpp"
#include "ThickLine.hpp"
#include <cmath>

#define NEAR_Z 0.5
#define FAR_Z 4.5

float map(float value,
          float start1, float stop1,
          float start2, float stop2)
{
    return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
}

int main()
{
    sf::ContextSettings settings(0, 0, 8);
    sf::RenderWindow window(sf::VideoMode(400, 400), "Tesseract", sf::Style::Close, settings);
    window.setFramerateLimit(60);
    sf::CircleShape shape(5.f);
    shape.setOrigin(5, 5);
    shape.setFillColor(sf::Color::White);

    Vector<float> points4D[16] = {
        {{-1, -1, -1, 1}},
        {{1, -1, -1, 1}},
        {{1, 1, -1, 1}},
        {{-1, 1, -1, 1}},
        {{-1, -1, 1, 1}},
        {{1, -1, 1, 1}},
        {{1, 1, 1, 1}},
        {{-1, 1, 1, 1}},

        {{-1, -1, -1, -1}},
        {{1, -1, -1, -1}},
        {{1, 1, -1, -1}},
        {{-1, 1, -1, -1}},
        {{-1, -1, 1, -1}},
        {{1, -1, 1, -1}},
        {{1, 1, 1, -1}},
        {{-1, 1, 1, -1}},
    };

    Vector<float> projpoints3D[16];
    Vector<float> projpoints[16];
    float angle = 0.f;
    unsigned int frameCount = 0;
    sfLine line;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        window.clear(sf::Color(100, 100, 100));

        for (size_t i = 0; i < 16; i++)
        {
            Vector<float> v = points4D[i];

            Matrix<float> rotationXY({
                {std::cos(angle), -std::sin(angle), 0, 0},
                {std::sin(angle), std::cos(angle), 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1},
            });
            Matrix<float> rotationZW({
                {1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, std::cos(angle), -std::sin(angle)},
                {0, 0, std::sin(angle), std::cos(angle)},
            });

            Matrix<float> rotated = rotationZW * Matrix<float>(v);
            rotated = rotationXY * rotated;

            float distance = 2;
            float w = 1 / (distance - rotated[3][0]);

            Matrix<float> projection({
                {w, 0, 0, 0},
                {0, w, 0, 0},
                {0, 0, w, 0},
            });
            Matrix<float> projected = projection * rotated;
            projpoints3D[i] = projected.AsVector();
        }

        for (size_t i = 0; i < 16; i++)
        {
            Vector<float> v = projpoints3D[i];
            v.Append(1);
            // Matrix<float> projection({
            //     {1, 0, 0},
            //     {0, 1, 0},
            // });

            float fov = 60.0f * (M_PI / 180.0f);
            float aspectRatio = 1.0;

            Matrix<float> projection({
                {fov * aspectRatio, 0, 0, 0},
                {0, fov, 0, 0},
                {0, 0, (FAR_Z + NEAR_Z) / (FAR_Z - NEAR_Z), (2 * NEAR_Z * FAR_Z) / (FAR_Z - NEAR_Z)},
                {0, 0, 1, 0},
            });

            float angle3D = -M_PI / 2.f;

            Matrix<float> rotationX({
                {1, 0, 0, 0},
                {0, std::cos(angle3D), -std::sin(angle3D), 0, 0},
                {0, std::sin(angle3D), std::cos(angle3D), 0},
                {0, 0, 0, 1},
            });

            Matrix<float> rotationY({
                {std::cos(angle3D), 0, std::sin(angle3D), 0},
                {0, 1, 0, 0},
                {-std::sin(angle3D), 0, std::cos(angle3D), 0},
                {0, 0, 0, 1},
            });

            Matrix<float> rotationZ({
                {std::cos(angle3D), -std::sin(angle3D), 0, 0},
                {std::sin(angle3D), std::cos(angle3D), 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1},

            });

            Matrix<float> point(v);
            point = rotationX * point;
            //point = rotationY * point;
            //point = rotationZ * point;
            point[2][0] -= 5;

            Matrix<float> projected = (projection * point);
            
            Vector<float> projectedpoint = projected.AsVector();
            projectedpoint /= projectedpoint[3];
            projectedpoint.Resize(2);

            projectedpoint[0] = (projectedpoint[0] * 300) + 200;
            projectedpoint[1] = (projectedpoint[1] * 300) + 200;
            projpoints[i] = projectedpoint;
            shape.setPosition(projectedpoint[0], projectedpoint[1]);
            window.draw(shape);
        }
        for (size_t i = 0; i < 4; i++)
        {
            line.SetPoints(sf::Vector2f(projpoints[i][0], projpoints[i][1]), sf::Vector2f(projpoints[(i + 1) % 4][0], projpoints[(i + 1) % 4][1]));
            window.draw(line);
            line.SetPoints(sf::Vector2f(projpoints[i + 4][0], projpoints[i + 4][1]), sf::Vector2f(projpoints[((i + 1) % 4) + 4][0], projpoints[((i + 1) % 4) + 4][1]));
            window.draw(line);
            line.SetPoints(sf::Vector2f(projpoints[i][0], projpoints[i][1]), sf::Vector2f(projpoints[i + 4][0], projpoints[i + 4][1]));
            window.draw(line);

            line.SetPoints(sf::Vector2f(projpoints[i + 8][0], projpoints[i + 8][1]), sf::Vector2f(projpoints[((i + 1) % 4) + 8][0], projpoints[((i + 1) % 4) + 8][1]));
            window.draw(line);
            line.SetPoints(sf::Vector2f(projpoints[i + 12][0], projpoints[i + 12][1]), sf::Vector2f(projpoints[((i + 1) % 4) + 12][0], projpoints[((i + 1) % 4) + 12][1]));
            window.draw(line);
            line.SetPoints(sf::Vector2f(projpoints[i + 8][0], projpoints[i + 8][1]), sf::Vector2f(projpoints[i + 12][0], projpoints[i + 12][1]));
            window.draw(line);
        }

        for (size_t i = 0; i < 8; i++)
        {
            line.SetPoints(sf::Vector2f(projpoints[i][0], projpoints[i][1]), sf::Vector2f(projpoints[i + 8][0], projpoints[i + 8][1]));
            window.draw(line);
        }

        window.display();
        frameCount++;
        angle = -(frameCount / (float)60);
    }

    return EXIT_SUCCESS;
}