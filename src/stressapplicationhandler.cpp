#include "stressapplicationhandler.h"

namespace Stress
{
    void StressHandler::IncreaseStress(float a_amount)
    {
        Settings::Forms::stress_total_value->value += a_amount;
        CapStress(true);
    }
    void StressHandler::DecreaseStress(float a_amount)
    {
        Settings::Forms::stress_total_value->value -= a_amount;
        CapStress(false);
    }

    void StressHandler::ApplyStress()
    {
        if (IsStressActive())
        {
            IncreaseStress(Settings::Values::stress_increase_value.GetValue());
            if (Settings::Values::show_stress_message.GetValue())
            {
                ShowStressMessage(Settings::Values::stress_increase_message.GetValue().c_str());
            }
        }
    }

    void StressHandler::ReduceStress()
    {
        if (IsStressActive())
        {
            DecreaseStress(Settings::Values::stress_increase_value.GetValue());
            if (Settings::Values::show_stress_message.GetValue())
            {
                ShowStressMessage(Settings::Values::stress_decrease_message.GetValue().c_str());
            }
        }
    }

    void StressHandler::ShowStressMessage(std::string a_text)
    {
        RE::DebugNotification(a_text.c_str(), nullptr, true);
    }

    void StressHandler::CapStress(bool a_upper)
    {
        if (a_upper)
        {
            if (Settings::Forms::stress_total_value->value > 100)
            {
                Settings::Forms::stress_total_value->value = 100.0f;
            }
        }
        else
        {
            if (Settings::Forms::stress_total_value->value < 1.0)
            {
                Settings::Forms::stress_total_value->value = 0.0f;
            }
        }
    }

    bool StressHandler::IsStressActive() const
    {
        bool result = false;
        if (Settings::Forms::stress_enabled && Settings::Forms::stress_enabled->value != 0)
        {
            result = true;
        }
        return result;
    }
}