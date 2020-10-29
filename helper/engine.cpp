#include "engine.h"
#include "page.h"

Engine::Engine(QObject *parent)
{    
    qmlRegisterType<Page>("Custom", 1, 0, "Wrapper");
}
