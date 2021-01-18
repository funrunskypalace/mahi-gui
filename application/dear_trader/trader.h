#include "pch/compat.h"

#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>
#include "gwengine.h"
#include "dataengine.h"

using namespace mahi::gui;
using namespace mahi::util;
using namespace cycnoches::ort;
using namespace framework;
using namespace framework::router;

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct ExampleAppLog {
    ImGuiTextBuffer Buf;
    ImGuiTextFilter Filter;
    ImVector<int> LineOffsets;  // Index to lines offset. We maintain this with AddLog() calls.
    bool AutoScroll;            // Keep scrolling if already at the bottom.

    ExampleAppLog();

    void Clear();

    void AddLog(const char* fmt, ...) IM_FMTARGS(2);

    void Draw(const char* title, bool* p_open = NULL);
};

// Inherit from Application
class MyApp : public Application {
public:
    // 640x480 px window
    MyApp() : Application(640, 480, "Dear Trader"), dataeg_(std::make_shared<DataEngine>()) {}
    // Override update (called once per frame)
    void update() override { ShowBasicWindow(); };

protected:
    void ShowBasicWindow();
    void ShowExampleAppLog();

private:
    DataEnginePtr dataeg_;
};
