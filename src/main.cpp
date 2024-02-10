#include "pch.h"
#include <cmath>

float normalize(float val, float minFirst, float maxFirst, float minSecond, float maxSecond) {
  float lenFirst = maxFirst - minFirst;
  float lenSecond = maxSecond - minSecond;

  return minSecond + ((lenSecond * (val - minFirst)) / lenFirst);
};

int main() {
  constexpr float minVal = -2.5f;
  constexpr float maxVal = -minVal;
  constexpr int maxIters = 100;

  sf::RenderWindow window;
  sf::VertexArray vertices{sf::Points, WIDTH * HEIGHT};

  // Setup main window
  window.create(sf::VideoMode(WIDTH, HEIGHT), "Mandelbrot set", sf::Style::Close);
  window.setFramerateLimit(90);

  /*
   * z(n-1) = z(n)**2 + c
   * z0 = 0
   * z1 = z0 ** 2 + c -> c            1st iteration
   * z2 = z1 ** 2 + c -> c**2 + c     2nd iteration
  */

  for (int x = 0; x < WIDTH; x++) {
    for (int y = 0; y < HEIGHT; y++) {
      float a = normalize(x, 0, WIDTH, minVal, maxVal);
      float b = normalize(y, 0, HEIGHT, minVal, maxVal);

      // The default "c" component
      float ca = a;
      float cb = b;

      int n = 0;
      for (; n < maxIters; n++) {
        // This two make the "z(n-1)²" component
        float aa = a * a - b * b;
        float bb = 2.f * a * b; // Its actually "2*ab*i" but why its not here in anyway?

        // This two make the "z(n)²" component
        a = aa + ca;
        b = bb + cb;

        // The absolute value of z(n)
        if (std::abs(a + b) > 20) break;
      }

      float bright = static_cast<float>(n) / maxIters;
      bright = n == maxIters ? 0.f : sqrt(bright) * 255.f;

      vertices[(x + y * WIDTH)] = sf::Vertex(sf::Vector2f(x, y), sf::Color(bright, bright, bright));
    }
  }

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
    }

    window.clear();
    window.draw(vertices);
    window.display();
  }

  return 0;
}

