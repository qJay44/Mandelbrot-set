#include <cmath>
#include "colormap.hpp"
#include "Gui.hpp"

#define WIDTH 1920
#define HEIGHT 1080

int main() {
  sf::RenderWindow window;
  sf::Shader shader;
  sf::RectangleShape rect({WIDTH, HEIGHT});
  sf::Clock clock;

  const sf::Vector2f boundsX{-2.5f, 1.f};
  const sf::Vector2f boundsY{-1.f, 1.f};

  sf::Vector2f move{0.f, 0.f};
  float zoom = 1;
  int iters = 1000;

  // Setup main window
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot set", sf::Style::Close);
  window.setFramerateLimit(90);

  // ImGui init
  Gui gui(window, iters);

  // Shader setup
  shader.loadFromFile("../../src/mandelbrot.frag", sf::Shader::Fragment);
  shader.setUniformArray("colormap", colormap, 256);
  shader.setUniform("resolution", sf::Glsl::Vec2{WIDTH, HEIGHT});
  shader.setUniform("boundsX", boundsX);
  shader.setUniform("boundsY", boundsY);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      gui.processEvent(event);
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyReleased)
        switch (event.key.code) {
          case sf::Keyboard::Q:
            window.close();
            break;
          default:
            break;
        }

      if (event.type == sf::Event::MouseWheelScrolled) {
        static const sf::Vector2i center(WIDTH * 0.5f, HEIGHT * 0.5f);

        // Calculate direction vector between mouse and the center
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        sf::Vector2i v = mouse - center;
        float d = sqrt(v.x * v.x + v.y * v.y);
        sf::Vector2f dir{v.x / d, v.y / d};

        // Big zoom out/in
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
          if (event.mouseWheelScroll.delta == -1)
            zoom += zoom * -0.2f; // To avoid instanly returning to zero decrease it by some value (0.2f)
          else
            zoom += zoom * 0.8f;

        // Simple zoom out/in
        } else
          zoom += event.mouseWheelScroll.delta;

        // Avoid zero division
        if (!zoom) zoom = 1.f;

        // Move only when zooming in
        if (event.mouseWheelScroll.delta == 1) {
          // The bigger zoom the less movement applied
          move.x += (dir.x * 0.25f) / zoom;
          move.y -= (dir.y * 0.25f) / zoom;
        }
      }
    }

    shader.setUniform("move", move);
    shader.setUniform("zoom", zoom);
    shader.setUniform("iters", iters);

    window.clear();
    window.draw(rect, &shader);
    gui.draw(clock.restart());
    window.display();
  }

  return 0;
}

