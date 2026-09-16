#include "Engine.h"
#include <iostream>
#include "ConfigLoader.h"

int main() {
    Engine engine;
    ConfigLoader loader("config.json");
    if (!loader.isLoaded()) {
        std::cerr << "A konfiguráció betöltése meghiúsult!" << std::endl;
        return 1;
    }

    // Referencia lekérése a belső Config objektumra
    const Config& config = loader.getConfig();

    // Használat:
    std::cout << "Ablak mérete: " << config.window.width << "x" << config.window.height << std::endl;
    std::cout << "Kamerák száma: " << config.cameras.size() << std::endl;
    std::cout << "Modell útvonal: " << config.scene.model << std::endl;
    if( !engine.initialize() )
        return 1;

    engine.run();
    engine.shutdown();

    return 0;
}
