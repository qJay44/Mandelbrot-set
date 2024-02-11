#include <cmath>
#include "colormap.hpp"
#include "Gui.hpp"

#define WIDTH 1000
#define HEIGHT 720

int main() {
  sf::RenderWindow window;
  sf::Shader shader;
  sf::RectangleShape rect({WIDTH, HEIGHT});
  sf::Clock clock;

  sf::Glsl::Vec2 boundsX{-2.5f, 1.f};
  sf::Glsl::Vec2 boundsY{-1.f, 1.f};
  int iters = 1000;

  // Setup main window
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot set", sf::Style::Close);
  window.setFramerateLimit(90);

  // ImGui init
  Gui gui(window, boundsX, boundsY, iters);

  // Shader setup
  shader.loadFromFile("../../src/mandelbrot.frag", sf::Shader::Fragment);
  shader.setUniformArray("colormap", colormap, 256);
  shader.setUniform("resolution", sf::Glsl::Vec2{WIDTH, HEIGHT});

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
    }

    shader.setUniform("boundsX", boundsX);
    shader.setUniform("boundsY", boundsY);
    shader.setUniform("iters", iters);

    window.clear();
    window.draw(rect, &shader);
    gui.draw(clock.restart());
    window.display();
  }

  return 0;
}

