#pragma once

namespace Solid
{
    class InspectorInterface
    {
    public:
        InspectorInterface()  = default;
        ~InspectorInterface() = default;

        void Draw();

        bool p_open = true;
    };

}