#include <texturen/texturen.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <fstream>
#include <array>
#define M_PI 3.14


std::vector<Point> Texture::readPointsFromFile(const std::string& filename) const{
    std::ifstream inFile(filename);
    std::vector<Point> points;

    if (!inFile.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return points;
    }

    double theta, phi;
    while (inFile >> phi >> theta) {
        double x, y;
        x = sin(theta) * cos(phi);
        y = sin(theta) * sin(phi);
        points.emplace_back(x, y);
    }

    inFile.close();
    return points;
}

void Texture::Heatmap(const std::string& filename) const{
    std::vector<Point> points = readPointsFromFile(filename);

    // ������� ���� SFML
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Heatmap Example");
    window.setFramerateLimit(60);

    // ����� ��� �������� ���������� ����� � �������
    const int gridSize = 100; // ������ �����
    std::array<std::array<int, gridSize>, gridSize> grid = {};

    // ������� ���������� ����� � ������ ������ �����
    for (const auto& point : points) {
        int xIndex = static_cast<int>((point.x + M_PI) / (2 * M_PI) * gridSize);
        int yIndex = static_cast<int>((point.y + M_PI / 2) / M_PI * gridSize);

        if (xIndex >= 0 && xIndex < gridSize && yIndex >= 0 && yIndex < gridSize) {
            grid[xIndex][yIndex]++;
        }
    }

    // �������� �������� ��� �������� �����
    sf::RenderTexture heatmapTexture;
    heatmapTexture.create(1000, 1000);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // ������� �������� ��� ������� �����
        heatmapTexture.clear();

        // ��������� �������� ����� �� ������ ���������� ����� � ������� �����
        for (int x = 0; x < gridSize; ++x) {
            for (int y = 0; y < gridSize; ++y) {
                double intensity = static_cast<double>(grid[x][y]) / 100; // ��������������� �������������
                sf::RectangleShape pixel(sf::Vector2f(10, 10)); // ������ ������
                pixel.setPosition(x * 10, y * 10); // �������� �� ������ ������ ��� ������������
                pixel.setFillColor(sf::Color(static_cast<sf::Uint8>(intensity * 255), 0, 0));
                heatmapTexture.draw(pixel);
            }
        }

        // ������������� ����� �������� ����� �� �����
        heatmapTexture.display();

        // ������� ��������� ���� � ����� �������� �������� �����
        window.clear();
        sf::Sprite heatmapSprite(heatmapTexture.getTexture());
        window.draw(heatmapSprite);
        window.display();
        sf::Image image = heatmapTexture.getTexture().copyToImage();
        if (image.saveToFile("heatmap.png")) {
            std::cout << "�������� ����� ��������� � ���� 'heatmap.png'" << std::endl;

        }
        else {
            std::cerr << "������ ���������� �������� �����!" << std::endl;

        }

    }

