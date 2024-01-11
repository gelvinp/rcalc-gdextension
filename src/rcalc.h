#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/string.hpp>

#include "app/application.h"
#include "app/autocomplete.h"
#include "help.h"
#include "core/memory/cowvec.h"

using namespace godot;

class RCalcEngine : public RefCounted {
    GDCLASS(RCalcEngine, RefCounted);

public:
    bool try_renderer_command(const String& cmd);

    void submit_text(const String& text);

    void init_suggestions(const String& gd_str);
    String get_next_suggestion();
    String get_previous_suggestion();
    void cancel_suggestion() { autocomp.cancel_suggestion(); }

    bool suggestions_active() const { return autocomp.suggestions_active(); }

    TypedArray<RCalcHelpCommand> get_help_commands();
    void bind_commands(TypedArray<RCalcHelpCommand> commands);

    static String get_version_string();
    static String get_version_hash();
    static String get_program_info();

protected:
    static void _bind_methods();

private:
    RCalc::Application* p_application = nullptr;
    RCalc::AutocompleteManager autocomp;
    bool commands_bound = false;

    struct BoundCommands {
        TypedArray<RCalcHelpCommand> source;
        RCalc::CowVec<RCalc::CommandMeta> arr_commands;
        RCalc::CowVec<const RCalc::CommandMeta*> arr_command_ptrs;
        std::vector<RCalc::CowVec<const char*>> alias_arrs;
        RCalc::CowVec<std::string> arr_strings;
    };
    BoundCommands bound_commands;

public:
    RCalcEngine();
    ~RCalcEngine();
};
