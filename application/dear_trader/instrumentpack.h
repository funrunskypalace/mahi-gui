#pragma once

#include <unordered_map>
#include <mutex>
#include "implot.h"

#include "ort/ORDataStruct.h"

using namespace cycnoches::ort;

// utility structure for realtime plot
struct RollingBuffer {
    ImVector<ImVec2> Data;
    RollingBuffer() {
        Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
        Data.push_back(ImVec2(x, y));
    }
};
using RollingBufferPtr = std::shared_ptr<RollingBuffer>;

struct InstrumentPack {
    std::mutex                           mux_;
    std::string                          exid;
    std::string                          insid;
    std::list<ORDepthMarketDataFieldPtr> market_data;
    RollingBuffer                        buffer;
};
using InstrumentPackPtr = std::shared_ptr<InstrumentPack>;