#include "pch/compat.h"
#include "implot.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "trader.h"

#ifdef _MSC_VER
#define sprintf sprintf_s
#endif

#ifndef PI
#define PI 3.14159265358979323846
#endif

template <typename T>
inline T RandomRange(T min, T max) {
    T scale = rand() / (T)RAND_MAX;
    return min + scale * (max - min);
}

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer() {
        MaxSize = 2000;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset       = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};
using ScrollingBufferPtr = std::shared_ptr<ScrollingBuffer>;

// Huge data used by Time Formatting example (~500 MB allocation!)
struct HugeTimeData {
    HugeTimeData(double min) {
        Ts = new double[Size];
        Ys = new double[Size];
        for (int i = 0; i < Size; ++i) {
            Ts[i] = min + i;
            Ys[i] = GetY(Ts[i]);
        }
    }
    ~HugeTimeData() {
        delete[] Ts;
        delete[] Ys;
    }
    static double GetY(double t) {
        return 0.5 + 0.25 * sin(t / 86400 / 12) + 0.005 * sin(t / 3600);
    }
    double *Ts;
    double *Ys;
    static const int Size = 60 * 60 * 24 * 366;
};
using HugeTimeDataPtr = std::shared_ptr<HugeTimeData>;



void MyApp::ShowBasicWindow() {
    bool             show_login   = false;
    bool             show_close   = true;
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;

    // We specify a default position/size in case there's no data in the .ini file.
    // We only do it to make the demo applications a little more welcoming, but typically this isn't
    // required.
    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Dear Trader", &show_close, window_flags)) {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Tools")) {
            ImGui::MenuItem("Login", NULL, &show_login);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::CollapsingHeader("Configuration")) {
    }

    if (ImGui::CollapsingHeader("Tick Graph")) {
        ImGui::BeginGroup();
        static char text[1024 * 16]      = "";
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

        ImGui::InputTextMultiline("InstrumentID", text, IM_ARRAYSIZE(text),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
        {
            static std::unordered_map<std::string, RollingBufferPtr> plotlines;
            //static bool show_lines = true;
            //static bool show_fills = true;
            //static float fill_ref  = 0;
            //ImGui::Checkbox("Lines", &show_lines);
            //ImGui::Checkbox("Fills", &show_fills);
            //ImGui::SameLine();
            if (ImGui::Button("Update")) {
                dataeg_->loadMarketData(text);
            } else {
                dataeg_->iterate([&](InstrumentPackPtr &pack) -> void {
                    auto it = plotlines.find(pack->insid);
                    if (plotlines.cend() == it) {
                        plotlines.insert(std::make_pair(pack->insid, std::make_shared<RollingBuffer>()));
                        it = plotlines.find(pack->insid);
                    }
                    auto line = it->second;
                    for (int i = 0; i < pack->buffer.Data.size(); ++i) {
                        line->AddPoint(pack->buffer.Data[i].x, pack->buffer.Data[i].y);
                    }
                    pack->buffer.Data.clear();
                });
            }
            //ImGui::DragFloat("Reference", &fill_ref, 1, -100, 500);

            if (ImPlot::BeginPlot("Stock Prices", "Ticks", "Price")) {
                for (auto line : plotlines) {
                    ImPlot::PlotLine(line.first.c_str(), &line.second->Data[0].x,
                                     &line.second->Data[0].y, line.second->Data.size(),
                        0, 2 * sizeof(float));
                }
                ImPlot::EndPlot();
            }
        }
        ImGui::EndGroup();
    }

    // End of ShowDemoWindow()
    ImGui::End();
}