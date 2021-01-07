#pragma once
/*
 Copyright: brent.
 GUI for select stock list.
 select from different sources, output to '<workdir>/stocks_list.csv', and return to
 API user.
*/

/// 这是一个Stacked Model Window
/// 用来选择一种类型的股票列表。
/// 例如，
/// - 可以选择沪深300指数所包含的股票
/// - 或者可以选择当日所有不停牌的股票
/// - 或者可以选择当日（某交易所）所有的商品期货

#include "imgui/imgui.h"
#include "ort/ORDataStruct.h"
#include "tdgui/common/names.h"

using namespace cycnoches::ort;

namespace tdgui {
namespace market {
int ShowStockListSelector(std::list<ORInstrumentFieldPtr>& fields) {
    if (ImGui::BeginPopupModal(stock_selector_title, NULL, ImGuiWindowFlags_MenuBar)) {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Some menu item")) {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        ImGui::Text(welcome_to_use_stock_selector);

        // Testing behavior of widgets stacking their own regular popups over the modal.
        static int   item     = 1;
        static float color[4] = {0.4f, 0.7f, 0.0f, 0.5f};
        ImGui::Combo("Combo", &item, "aaaa\0bbbb\0cccc\0dddd\0eeee\0\0");
        ImGui::ColorEdit4("color", color);

        if (ImGui::Button("Add another modal.."))
            ImGui::OpenPopup("Stacked 2");

        // Also demonstrate passing a bool* to BeginPopupModal(), this will create a regular close
        // button which will close the popup. Note that the visibility state of popups is owned by
        // imgui, so the input value of the bool actually doesn't matter here.
        bool unused_open = true;
        if (ImGui::BeginPopupModal("Stacked 2", &unused_open)) {
            ImGui::Text("Hello from Stacked The Second!");
            if (ImGui::Button("Close"))
                ImGui::CloseCurrentPopup();
            ImGui::EndPopup();
        }

        if (ImGui::Button("Close"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }

    return 0;
}
}  // namespace market
}  // namespace tdgui