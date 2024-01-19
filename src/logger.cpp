#include "logger.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;
using namespace RCalc;
using namespace RCalc::Logging;


void GodotRCalcLogger::log(Severity severity, std::string message) {
    if (severity < min_severity) { return; }

    switch (severity) {
        case LOG_ERROR:
            UtilityFunctions::push_error(String::utf8(message.c_str()));
            break;
        case LOG_STANDARD:
            UtilityFunctions::print(String::utf8(message.c_str()));
            break;
        case LOG_VERBOSE:
            UtilityFunctions::print_verbose(String::utf8(message.c_str()));
            break;
    }
}