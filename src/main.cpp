#include "Engine.h"
#include <iostream>
#include "ConfigLoader.h"


int main() {
    const std::string filename = "/home/satch/Projects/Artillery/cfg/config.json";
    ConfigLoader loader(filename);
    if (!loader.isLoaded()) {
        std::cerr << "A konfiguráció betöltése meghiúsult! " << filename  << std::endl;
        return 1;
    } else {
        std::cout << "A konfiguráció betöltése sikeres! " << filename  << std::endl;
    }

    // Referencia lekérése a belső Config objektumra
    const Config& config = loader.getConfig();

    // Használat:

    Engine engine(config);
    if( !engine.initialize() )
        return 1;

    engine.run();
    engine.shutdown();

    return 0;
}
