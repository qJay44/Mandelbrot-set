#include <cmath>
#include <stdio.h>
#include "colormap.hpp"
#include "Gui.hpp"

#define WIDTH 1920
#define HEIGHT 1080

int main() {
  sf::RenderWindow window;
  sf::Shader shader;
  sf::RectangleShape dummyRect({WIDTH, HEIGHT});
  sf::Clock clock;
  sf::Vector2i mouseCurr = sf::Mouse::getPosition();
  sf::Vector2i mousePrev = sf::Mouse::getPosition();

  const sf::Vector2f boundsX{-2.5f, 1.f};
  const sf::Vector2f boundsY{-1.f, 1.f};
  constexpr float zoomDirScale = 1.f / 2048.f;
  constexpr float mouseDragDirScale = 1.f / 512.f;

  sf::Vector2f move{0.f, 0.f};
  float zoom = 1;
  int iters = 1000;

  // Setup main window
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot set", sf::Style::Fullscreen);
  window.setVerticalSyncEnabled(true);
  //window.setFramerateLimit(90);

  // ImGui init
  Gui gui(window, iters);

  // Shader setup
  shader.loadFromFile("../../src/mandelbrot.frag", sf::Shader::Fragment);
  shader.setUniformArray("colormap", colormap, 256);
  shader.setUniform("resolution", sf::Glsl::Vec2{WIDTH, HEIGHT});
  shader.setUniform("boundsX", boundsX);
  shader.setUniform("boundsY", boundsY);
  shader.setUniform("move", move);
  shader.setUniform("zoom", zoom);

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

      if (event.type == sf::Event::MouseMoved) {
        mouseCurr = sf::Mouse::getPosition();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          sf::Vector2i v = mousePrev - mouseCurr;
          move.x += (v.x * mouseDragDirScale) / zoom;
          move.y -= (v.y * mouseDragDirScale) / zoom;
          shader.setUniform("move", move);
        }

        mousePrev = mouseCurr;
      }

      if (event.type == sf::Event::MouseWheelScrolled) {
        static const sf::Vector2i center(WIDTH * 0.5f, HEIGHT * 0.5f);

        // Calculate direction vector between mouse and the center
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        sf::Vector2i v = mouse - center;

        // Big zoom out/in
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
          if (event.mouseWheelScroll.delta == -1)
            zoom += zoom * -0.2f; // To avoid instanly returning to zero, decrease it by some value (0.2f)
          else
            zoom += zoom;

        // Simple zoom out/in
        } else
          zoom += event.mouseWheelScroll.delta * 0.2f; // To slow down zoom a bit (0.2f)

        // Avoid zero division
        if (!zoom) zoom = 1.f;

        // Move only when zooming in
        if (event.mouseWheelScroll.delta == 1) {
          // The bigger zoom the less movement applied
          move.x += (v.x * zoomDirScale) / zoom;
          move.y -= (v.y * zoomDirScale) / zoom;
        }

        shader.setUniform("move", move);
        shader.setUniform("zoom", zoom);
      }
    }

    shader.setUniform("iters", iters);

    window.clear();
    window.draw(dummyRect, &shader);
    gui.draw(clock.restart());
    window.display();
  }

  return 0;
}

