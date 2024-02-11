#include <cmath>
#include "colormap.hpp"
#include "pch.h"

#define WIDTH 1000
#define HEIGHT 720

int main() {
  sf::RenderWindow window;
  sf::VertexArray vertices{sf::Points, WIDTH * HEIGHT};
  sf::Shader shader;
  sf::RectangleShape rect({WIDTH, HEIGHT});

  float bound = 2.5f;
  int iters = 1000;

  // Setup main window
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot set", sf::Style::Close);
  window.setFramerateLimit(90);

  // Shader setup
  shader.loadFromFile("../../src/mandelbrot.frag", sf::Shader::Fragment);
  shader.setUniformArray("colormap", plasma, 256);
  shader.setUniform("resolution", sf::Glsl::Vec2{WIDTH, HEIGHT});
  shader.setUniform("bound", bound);
  shader.setUniform("iters", iters);
  shader.setUniform("colormapStep", 255.f / iters);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
          iters = std::clamp(iters + 100 * event.mouseWheelScroll.delta, 100.f, 5000.f);
          shader.setUniform("iters", iters);
          shader.setUniform("colormapStep", 255.f / iters);
        } else {
          bound += 0.1f * event.mouseWheelScroll.delta;
          shader.setUniform("bound", bound);
        }
      }
    }

    window.clear();
    window.draw(rect, &shader);
    window.display();
  }

  return 0;
}

