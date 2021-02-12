#pragma once

namespace Solid
{
    class InspectorInterface
    {
    private:
        bool p_open = true;
    public:
        InspectorInterface()  = default;
        ~InspectorInterface() = default;

        void Draw();
    };

}