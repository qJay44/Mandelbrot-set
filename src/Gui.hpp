#include "pch.h"
#include "imgui.h"
#include "imgui-SFML.h"

class Gui {
  sf::RenderWindow& window;
  int& iters;

public:
  Gui(sf::RenderWindow& win, int& iters)
    : window(win), iters(iters) {
    if (!ImGui::SFML::Init(window))
      throw std::runtime_error("ImGui initialization failed");
  }

  void draw(sf::Time time) {
    static bool doOnce = true;
    ImGui::SFML::Update(window, time);

    // Adjust imgui position
    if (doOnce) {
      ImGui::SetNextWindowPos({ 0, 0 });
      ImGui::SetNextWindowCollapsed(true);
      ImGui::SetNextWindowSize({window.getSize().x / 3.f, window.getSize().y / 3.f});
      doOnce = false;
    }

    ImGui::Begin("Settings");
    ImGui::SliderInt("Max iterations", &iters, 100, 5000);

    if (ImGui::Button("Reset")) reset();

    ImGui::End();

    ImGui::SFML::Render(window);
  }

  void processEvent(const sf::Event& event) {
    ImGui::SFML::ProcessEvent(event);
  }

  void reset() {
    iters = 1000;
  }
};

