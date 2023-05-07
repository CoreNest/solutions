#pragma once

#include "function.h"
triangle T2{

    std::array<vec2, 3>{vec2{10.0f, 30.0f},
                        vec2{40.0f, 0.0f},
                        vec2{80.0f, 100.0f}}

};

extern triangle T1;


namespace ImGui
{
    static inline bool SliderFloat6(const char *label, float v[6], float v_min, float v_max, const char *format = "%.3f", ImGuiSliderFlags flags = 0) { return SliderScalarN(label, ImGuiDataType_Float, v, 6, &v_min, &v_max, format, flags); }

}
static void ShowDemoWindowWidgets()
{
    {
        static char str0[128] = "Hello, world!";
        static float vec4a[] = {10.10f, 20.20f, 30.30f};

        float *tmp1 = &(T2.points[0].x);
        float *tmp2 = &(T2.points[0].y);
        ImGui::SliderFloat6("slider float2", &(T2.points[0].x), 0.0f, 1000.0f);
        int t[3] = {T2.color.r, T2.color.b, T2.color.g};
        ImGui::InputInt3("", t);
        if (isColliding(T1, T2))
            ImGui::Text("coliding");
        else
            ImGui::Text("no colision");
    }
}