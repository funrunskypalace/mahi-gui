#include "pch/compat.h"

#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>
#include "gwengine.h"

using namespace mahi::gui;
using namespace mahi::util;
using namespace cycnoches::ort;
using namespace framework;
using namespace framework::router;

// Inherit from Application
class MyApp : public Application {
    GwTraderEnginePtr trader_;
    time_t            today_start_ = {0};
    time_t            today_end_   = {0};
    double            maxy_        = 0;

public:
    // 640x480 px window
    MyApp(const GwTraderEnginePtr& trader) :
        Application(640, 480, "Dear Trader"), trader_(trader) {}
    // Override update (called once per frame)
    void update() override { ShowBasicWindow(trader_); };

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
}