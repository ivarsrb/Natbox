#include <iostream>
#include <exception>
#include <memory>
#include "app/study/terrain/app.h"
#include "app/study/grass/app.h"

int main(int argc, char *argv[]) {
    // App is selected as first command line argument
    std::string app_type;
    if(argc >= 2) {
        app_type = argv[1];
    } else {
        app_type = "";
    }

    try {  
        std::unique_ptr<engine::platform::IPlatformApp> app;
        if (app_type == "terrain") {
            app = std::make_unique<app::study::terrain::App>();
        } else if (app_type == "grass") { 
            app = std::make_unique<app::study::grass::App>();
        } else {
            throw std::runtime_error("(main) unknown app '" + app_type + "'");
        }
        app->Run();
    }
    catch (const std::exception& e) {
        std::cerr << "EXCEPTION: " << e.what() << std::endl;
    }
    
    std::cin.ignore().get(); 
    return 0;
}