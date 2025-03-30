#pragma once

#include "settings.h"

namespace Stress{
    class StressHandler : public REX::Singleton<StressHandler>
    {
    public:
        void IncreaseStress(float a_amount);
        void DecreaseStress(float a_amount);
        void ApplyStress();
        void ReduceStress();
        void ShowStressMessage(std::string a_text);
        void CapStress(bool a_upper);
        bool IsStressActive() const;
    };
}