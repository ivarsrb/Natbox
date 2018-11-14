#include <iostream>
#include <exception>
#include <memory>
#include <optional>
//#include "app/study/terrain/app.h"
#include "app/study/grass/app.h"

// Choose app based on parameter string
std::unique_ptr<engine::platform::IPlatformApp> AppFactory(const std::string &app_string) {
    std::unique_ptr<engine::platform::IPlatformApp> app;
    if (app_string == "terrain") {
        //app = std::make_unique<app::study::terrain::App>();
    } else if (app_string == "grass") { 
        app = std::make_unique<app::study::grass::App>();
    } else {
        throw std::runtime_error("(main) unknown app '" + app_string + "'");
    }
    return app;
}

int main(int argc, char *argv[]) {
    // App is selected as first command line argument
    std::optional<std::string> selected_app = std::nullopt;
    if (argc >= 2) {
        selected_app = argv[1];
    }

    try {  
        if (selected_app) {
            auto app = AppFactory(selected_app.value());
            app->Run();
        } else {
            throw std::runtime_error("(main) no argument for app is provided");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    
    //std::cin.ignore().get(); 
    return 0;
}