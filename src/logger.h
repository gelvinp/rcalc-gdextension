#pragma once

#include "core/logging/engines.h"


class GodotRCalcLogger : public RCalc::Logging::Engine {
public:
    virtual ~GodotRCalcLogger() = default;

    virtual void set_min_severity(RCalc::Logging::Severity severity) override { min_severity = severity; }
    virtual void log(RCalc::Logging::Severity severity, std::string message) override;

private:
    RCalc::Logging::Severity min_severity;
};