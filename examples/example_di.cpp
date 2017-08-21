/*
    Boost Software License - Version 1.0 - August 17, 2003

    Permission is hereby granted, free of charge, to any person or organization
    obtaining a copy of the software and accompanying documentation covered by
    this license (the "Software") to use, reproduce, display, distribute,
    execute, and transmit the Software, and to prepare derivative works of the
    Software, and to permit third-parties to whom the Software is furnished to
    do so, all subject to the following:

    The copyright notices in the Software and this entire statement, including
    the above license grant, this restriction and the following disclaimer,
    must be included in all copies of the Software, in whole or in part, and
    all derivative works of the Software, unless such copies or derivative
    works are solely in the form of machine-executable object code generated by
    a source language processor.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
    SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
    FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <reflection/api.hpp>
#include <reflection/magic.hpp>

#include <reflection/basic_types.hpp>
#include <reflection/class.hpp>

#include <reflection/di.hpp>

#include <cassert>
#include <vector>

using namespace std;

// definitions

class IRenderable {
public:
    virtual const char* getModelName() = 0;
    virtual const char* getName() = 0;
};

class IRenderingManager {
public:
    REFL_UUID(0x4ab91e0d, 0x2acd47a6, 0x98410a60, 0xe4c6baff)

    virtual void registerObject(IRenderable* object) = 0;
};

// engine

class MyRenderingManager: public IRenderingManager {
public:
    virtual void registerObject(IRenderable* object) override {
        renderables.push_back(object);
    }

    void renderAll() {
        for (auto r : renderables)
            printf("Rendering '%s' using model '%s'.\n", r->getName(), r->getModelName());
    }

private:
    vector<IRenderable*> renderables;
};

// client

class MyRenderable: public IRenderable {
public:
    MyRenderable(const char* name, const char* modelName)
            : name(name), modelName(modelName) {
        assert(reflection::di(*this));

        renderingManager->registerObject(this);
    }

    virtual const char* getModelName() override { return modelName.c_str(); }
    virtual const char* getName() override { return name.c_str(); }

    REFL_BEGIN("MyRenderable", 1)
        REFL_FIELD(name)
        REFL_FIELD(modelName)

        REFL_DEPENDENCY(renderingManager)
    REFL_END

private:
    // fields
    string name, modelName;

    // dependencies
    IRenderingManager* renderingManager;
};

int main(int argc, char** argv) {
    MyRenderingManager mgr;
    reflection::diInject<IRenderingManager>(&mgr);

    MyRenderable hero("hero", "warrior_standing.mdl");
    MyRenderable enemy("enemy", "goblin_attacking.mdl");

    mgr.renderAll();
}

/* OUTPUT:

Rendering 'hero' using model 'warrior_standing.mdl'.
Rendering 'enemy' using model 'goblin_attacking.mdl'.

*/
