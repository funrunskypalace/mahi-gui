#include "pch/compat.h"

#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>
#include "gwengine.h"

using namespace mahi::gui;
using namespace mahi::util;
using namespace cycnoches::ort;
using namespace framework;
using namespace framework::router;

template <typename T>
inline T RandomRange(T min, T max) {
    T scale = rand() / (T)RAND_MAX;
    return min + scale * (max - min);
}

// utility structure for realtime plot
struct ScrollingBuffer {
    int              MaxSize;
    ImVector<double> DataX;
    ImVector<double> DataY;
    ScrollingBuffer() {
        MaxSize = 2000;
        DataX.reserve(MaxSize);
        DataY.reserve(MaxSize);
    }
    void AddPoint(double x, double y) {
        DataX.push_back(x);
        DataY.push_back(y);
    }
    void Erase(int maxRemain = 0) {
        if (DataX.size() > 0) {
            DataX.shrink(std::max(maxRemain, DataX.size()));
            DataY.shrink(std::max(maxRemain, DataY.size()));
        }
    }
};
using ScrollingBufferPtr = std::shared_ptr<ScrollingBuffer>;

// utility structure for realtime plot
struct RollingBuffer {
    float            Span;
    ImVector<ImVec2> Data;
    RollingBuffer() {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};

// Inherit from Application
class MyApp : public Application {
    GwTraderEnginePtr trader_;
    time_t            today_start_ = {0};
    time_t            today_end_ = {0};
    double            maxy_ = 0;

public:
    // 640x480 px window
    MyApp(const GwTraderEnginePtr& trader) : Application(640, 480, "Dear Trader"), trader_(trader) {
    }
    // Override update (called once per frame)
    void update() override {
        // App logic and/or ImGui code goes here
        ImGui::BulletText("Move your mouse to change the data!");
        ImGui::BulletText("This example assumes 60 FPS. Higher FPS requires larger buffer size.");
        static std::unordered_map<std::string, ScrollingBufferPtr> sdata2;
        //static RollingBuffer   rdata1, rdata2;
        ImVec2                 mouse = ImGui::GetMousePos();
        //static float           t     = 0;        
        //t += ImGui::GetIO().DeltaTime;
        //sdata1.AddPoint(t, mouse.x * 0.0005f);
        //rdata1.AddPoint(t, mouse.x * 0.0005f);
        //sdata2.AddPoint(t, mouse.y * 0.0005f);
        //rdata2.AddPoint(t, mouse.y * 0.0005f);
        auto data = trader_->fetch();
        for (auto it = data->cbegin(); it != data->cend(); ++it) {
            auto kmd = (*it);
            if (0 == today_start_) {
                today_start_ = parse_milliseconds(kmd->TradingDay, 90000000, false);
                today_end_ = parse_milliseconds(kmd->TradingDay, 150000000, false);
            }
            auto itemit = sdata2.find(kmd->InstrumentID);
            if (sdata2.cend() == itemit) {
                auto mmt = sdata2.emplace(std::make_pair(std::string(kmd->InstrumentID),
                                              std::make_shared<ScrollingBuffer>()));
                itemit = mmt.first;
            }
            auto tm  = parse_milliseconds(kmd->TradingDay, kmd->UpdateMillisec, false);
            (*itemit).second->AddPoint(tm, (kmd->LastPrice/kmd->PreClosePrice) - 1);
            //maxy_ = std::max(maxy_, kmd->PreClosePrice * 1.1);
        }

        static ImPlotAxisFlags xt_axis = ImPlotAxisFlags_Time;
        static ImPlotAxisFlags rt_axis = ImPlotAxisFlags_NoTickLabels;
        ImPlot::SetNextPlotLimitsX(today_start_, today_end_, ImGuiCond_Always);
        ImPlot::SetNextPlotLimitsY(-0.3, 0.3/*maxy_*/, ImGuiCond_Always);
        if (ImPlot::BeginPlot("##Scrolling", NULL, NULL, ImVec2(-1, 550), 0, xt_axis,
                              rt_axis | ImPlotAxisFlags_LockMin)) {
            for (auto it = sdata2.cbegin(); it != sdata2.cend(); ++it) {
                auto itemdata = it->second;
                if (itemdata->DataX.size() > 0) {
                    ImPlot::PlotLine<double>(
                        it->first.c_str(), 
                        itemdata->DataX.Data, 
                        itemdata->DataY.Data,
                        itemdata->DataX.size()
                        );
                }
            }
            ImPlot::EndPlot();
        }
    }
};

int main(int argc, char** argv) {
    auto dumper = gwengine_main(argc, argv);
    if (nullptr == dumper) {
        std::cerr << "failed to initialize greatwall framework." << std::endl;
        return -1;
    }
    MyApp app(dumper);
    app.run();
    dumper->stop();
    return 0;
}