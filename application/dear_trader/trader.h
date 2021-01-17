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

// Inherit from Application
class MyApp : public Application {
public:
    // 640x480 px window
    MyApp() : Application(640, 480, "Dear Trader"), dataeg_(std::make_shared<DataEngine>()) {}
    // Override update (called once per frame)
    void update() override { ShowBasicWindow(); };

protected:
    void ShowBasicWindow();

private:
    DataEnginePtr dataeg_;
};
