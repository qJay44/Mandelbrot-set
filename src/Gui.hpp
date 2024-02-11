#include "pch.h"
#include "imgui.h"
#include "imgui-SFML.h"

class Gui {
  sf::RenderWindow& window;
  sf::Glsl::Vec2& boundsX;
  sf::Glsl::Vec2& boundsY;
  int& iters;

  void reset() {
    boundsX = {-2.5f, 1.f};
    boundsY = {-1.f, 1.f};
    iters = 1000;
  }

public:
  Gui(sf::RenderWindow& win, sf::Glsl::Vec2& bX, sf::Glsl::Vec2& bY, int& iters)
    : window(win), boundsX(bX), boundsY(bY), iters(iters) {
    if (!ImGui::SFML::Init(window))
      throw std::runtime_error("ImGui initialization failed");
  }

  void draw(sf::Time time) {
    static bool doOnce = true;
    ImGui::SFML::Update(window, time);

    if (doOnce) {
      ImGui::SetNextWindowPos({ 0, 0 });
      ImGui::SetNextWindowCollapsed(true);
      ImGui::SetNextWindowSize({window.getSize().x / 3.f, window.getSize().y / 3.f});
      doOnce = false;
    }

    ImGui::Begin("Settings");

    ImGui::Text("X-axis bounds");
    ImGui::SliderFloat("Min##1", &boundsX.x, -5.f, 5.f);
    ImGui::SliderFloat("Max##1", &boundsX.y, -5.f, 5.f);

    ImGui::Text("Y-axis bounds");
    ImGui::SliderFloat("Min##2", &boundsY.x, -5.f, 5.f);
    ImGui::SliderFloat("Max##2", &boundsY.y, -5.f, 5.f);

    ImGui::Spacing();
    ImGui::SliderInt("Max iterations", &iters, 100, 1000);

    if (ImGui::Button("Reset")) reset();

    ImGui::End();

    ImGui::SFML::Render(window);
  }

  void processEvent(const sf::Event& event) {
    ImGui::SFML::ProcessEvent(event);
  }

  bool isHovered() {
    return
      ImGui::IsAnyItemHovered() ||
      ImGui::IsWindowHovered(ImGuiFocusedFlags_AnyWindow) ||
      ImGui::IsAnyItemActive();
  };
};

