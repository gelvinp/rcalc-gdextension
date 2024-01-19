#include "register_types.h"

#include "rcalc.h"
#include "stack_item.h"
#include "matrices.h"
#include "newline_container.h"
#include "help.h"
#include "core/memory/allocator.h"
#include "modules/register_modules.h"
#include "core/logger.h"
#include "logger.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_rcalc_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) { return; }

    RCalc::initialize_modules();

    // RCalc::Allocator::AllocFuncOverrides overrides = { // Not ready yet
    //     [](size_t size) { return memalloc(size); },
    //     [](void* ptr, size_t size) { return memrealloc(ptr, size); },
    //     [](void* ptr) { memfree(ptr); }
    // };
    // RCalc::Allocator::set_func_overrides(overrides);

    std::shared_ptr<GodotRCalcLogger> logger = RCalc::Allocator::make_shared<GodotRCalcLogger>();
    logger->set_min_severity(RCalc::Logging::LOG_ERROR);
    RCalc::Logger::set_global_engine(logger);

    GDREGISTER_CLASS(RCalcEngine);
    GDREGISTER_CLASS(RCalcStackItem);
    GDREGISTER_CLASS(RCalcMat2);
    GDREGISTER_CLASS(RCalcMat3);
    GDREGISTER_CLASS(RCalcMat4);
    GDREGISTER_CLASS(RCalcNewlineFlowContainer);
    GDREGISTER_CLASS(RCalcHelpSection);
    GDREGISTER_CLASS(RCalcHelpCommand);
    GDREGISTER_CLASS(RCalcHelpOperator);
    GDREGISTER_CLASS(RCalcHelpOperatorCategory);
    GDREGISTER_CLASS(RCalcHelpUnit);
    GDREGISTER_CLASS(RCalcHelpUnitFamily);
}

void uninitialize_rcalc_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) { return; }

    RCalc::cleanup_modules();
    RCalc::Logger::set_global_engine(nullptr);
}

extern "C" {

GDExtensionBool GDE_EXPORT rcalc_gd_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(initialize_rcalc_module);
    init_obj.register_terminator(uninitialize_rcalc_module);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}

}
