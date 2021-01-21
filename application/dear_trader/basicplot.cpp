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
    double* Ts;
    double* Ys;
    static const int Size = 60 * 60 * 24 * 366;
};
using HugeTimeDataPtr = std::shared_ptr<HugeTimeData>;

void MyApp::ShowBasicWindow() {
    bool show_login               = false;
    bool show_close               = true;
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

    if (ImGui::CollapsingHeader("HDF5-EADB")) {
        static char text[1024 * 16]      = "";
        static char logtext[1024 * 16]   = "";
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

        ImGui::InputTextMultiline("InstrumentID", text, IM_ARRAYSIZE(text),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
        if (ImGui::Button("Convert")) {
            dataeg_->convertHdf5Eadb(text);
        }
        ImGui::InputTextMultiline("InstrumentID", logtext, IM_ARRAYSIZE(logtext),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
    }

    if (ImGui::CollapsingHeader("HDF5 Reader")) {
        ImGui::BeginGroup();
        static char text[1024 * 16]      = "";
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

        ImGui::InputTextMultiline("Command here", text, IM_ARRAYSIZE(text),
                                  ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
        {
            static std::unordered_map<std::string, RollingBufferPtr> plotlines;
            // static bool show_lines = true;
            // static bool show_fills = true;
            // static float fill_ref  = 0;
            // ImGui::Checkbox("Lines", &show_lines);
            // ImGui::Checkbox("Fills", &show_fills);
            // ImGui::SameLine();
            if (ImGui::Button("Update")) {
                dataeg_->processCommand(text);
            } else {
                dataeg_->iterate([&](InstrumentPackPtr& pack) -> void {
                    auto it = plotlines.find(pack->insid);
                    if (plotlines.cend() == it) {
                        plotlines.insert(
                            std::make_pair(pack->insid, std::make_shared<RollingBuffer>()));
                        it = plotlines.find(pack->insid);
                    }
                    auto line = it->second;
                    for (int i = 0; i < pack->buffer.Data.size(); ++i) {
                        line->AddPoint(pack->buffer.Data[i].x, pack->buffer.Data[i].y);
                    }
                    pack->buffer.Data.clear();
                });
            }
            // ImGui::DragFloat("Reference", &fill_ref, 1, -100, 500);
            if (ImGui::CollapsingHeader("Graph")) {
                if (ImPlot::BeginPlot("Stock Prices", "Ticks", "Price")) {
                    for (auto line : plotlines) {
                        ImPlot::PlotLine(line.first.c_str(), &line.second->Data[0].x,
                                         &line.second->Data[0].y, line.second->Data.size(), 0,
                                         2 * sizeof(float));
                    }
                    ImPlot::EndPlot();
                }
            }
        }
        ImGui::EndGroup();
    }

    if (ImGui::CollapsingHeader("Log")) {
        ShowExampleAppLog();
    }
    // End of ShowDemoWindow()
    ImGui::End();
}

ExampleAppLog::ExampleAppLog() {
    AutoScroll = true;
    Clear();
}

void ExampleAppLog::Clear() {
    Buf.clear();
    LineOffsets.clear();
    LineOffsets.push_back(0);
}

void ExampleAppLog::AddLog(const char* fmt, ...) IM_FMTARGS(2) {
    int old_size = Buf.size();
    va_list args;
    va_start(args, fmt);
    Buf.appendfv(fmt, args);
    va_end(args);
    for (int new_size = Buf.size(); old_size < new_size; old_size++)
        if (Buf[old_size] == '\n')
            LineOffsets.push_back(old_size + 1);
}

void ExampleAppLog::Draw(const char* title, bool* p_open) {
    if (!ImGui::Begin(title, p_open)) {
        ImGui::End();
        return;
    }

    // Options menu
    if (ImGui::BeginPopup("Options")) {
        ImGui::Checkbox("Auto-scroll", &AutoScroll);
        ImGui::EndPopup();
    }

    // Main window
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    Filter.Draw("Filter", -100.0f);

    ImGui::Separator();
    ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (clear)
        Clear();
    if (copy)
        ImGui::LogToClipboard();

    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    const char* buf     = Buf.begin();
    const char* buf_end = Buf.end();
    if (Filter.IsActive()) {
        // In this example we don't use the clipper when Filter is enabled.
        // This is because we don't have a random access on the result on our filter.
        // A real application processing logs with ten of thousands of entries may want to store
        // the result of search/filter.. especially if the filtering function is not trivial
        // (e.g. reg-exp).
        for (int line_no = 0; line_no < LineOffsets.Size; line_no++) {
            const char* line_start = buf + LineOffsets[line_no];
            const char* line_end =
                (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
            if (Filter.PassFilter(line_start, line_end))
                ImGui::TextUnformatted(line_start, line_end);
        }
    } else {
        // The simplest and easy way to display the entire buffer:
        //   ImGui::TextUnformatted(buf_begin, buf_end);
        // And it'll just work. TextUnformatted() has specialization for large blob of text and
        // will fast-forward to skip non-visible lines. Here we instead demonstrate using the
        // clipper to only process lines that are within the visible area. If you have tens of
        // thousands of items and their processing cost is non-negligible, coarse clipping them
        // on your side is recommended. Using ImGuiListClipper requires
        // - A) random access into your data
        // - B) items all being the  same height,
        // both of which we can handle since we an array pointing to the beginning of each line
        // of text. When using the filter (in the block of code above) we don't have random
        // access into the data to display anymore, which is why we don't use the clipper.
        // Storing or skimming through the search result would make it possible (and would be
        // recommended if you want to search through tens of thousands of entries).
        ImGuiListClipper clipper;
        clipper.Begin(LineOffsets.Size);
        while (clipper.Step()) {
            for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                const char* line_start = buf + LineOffsets[line_no];
                const char* line_end   = (line_no + 1 < LineOffsets.Size)
                                           ? (buf + LineOffsets[line_no + 1] - 1)
                                           : buf_end;
                ImGui::TextUnformatted(line_start, line_end);
            }
        }
        clipper.End();
    }
    ImGui::PopStyleVar();

    if (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
        ImGui::SetScrollHereY(1.0f);

    ImGui::EndChild();
    ImGui::End();
}

void MyApp::ShowExampleAppLog() {
    bool show_close = true;
    bool* p_open    = &show_close;
    static ExampleAppLog applog;

    // For the demo: add a debug button _BEFORE_ the normal log window contents
    // We take advantage of a rarely used feature: multiple calls to Begin()/End() are appending to
    // the _same_ window. Most of the contents of the window will be added by the log.Draw() call.
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("Example: Log", p_open);
    dataeg_->fetchLogs([&](const LogInfoItemPtr& logitem) -> void {
        applog.AddLog("%s\n", logitem->logmsg.c_str());
    });
    ImGui::End();

    // Actually call in the regular Log helper (which will Begin() into the same window as we just
    // did)
    applog.Draw("HDF5 Reader: Log", p_open);
}
