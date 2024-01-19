#include "rcalc.h"
#include "renderer_proxy.h"
#include "stack_item.h"
#include "app/help_text.h"
#include "core/version.h"
#include "core/memory/allocator.h"

#include "core/memory/allocator.h"

using namespace godot;

void RCalcEngine::_bind_methods() {
    ClassDB::bind_method(D_METHOD("submit_text", "text"), &RCalcEngine::submit_text);

    ClassDB::bind_method(D_METHOD("init_suggestions", "pattern"), &RCalcEngine::init_suggestions);
    ClassDB::bind_method(D_METHOD("get_next_suggestion"), &RCalcEngine::get_next_suggestion);
    ClassDB::bind_method(D_METHOD("get_previous_suggestion"), &RCalcEngine::get_previous_suggestion);
    ClassDB::bind_method(D_METHOD("cancel_suggestion"), &RCalcEngine::cancel_suggestion);
    ClassDB::bind_method(D_METHOD("suggestions_active"), &RCalcEngine::suggestions_active);
    
    ClassDB::bind_method(D_METHOD("get_help_commands"), &RCalcEngine::get_help_commands);
    ClassDB::bind_method(D_METHOD("bind_commands", "commands"), &RCalcEngine::bind_commands);

    ClassDB::bind_static_method("RCalcEngine", D_METHOD("get_version_string"), &RCalcEngine::get_version_string);
    ClassDB::bind_static_method("RCalcEngine", D_METHOD("get_version_hash"), &RCalcEngine::get_version_hash);
    ClassDB::bind_static_method("RCalcEngine", D_METHOD("get_program_info"), &RCalcEngine::get_program_info);

    ClassDB::bind_static_method("RCalcEngine", D_METHOD("global_set_precision", "precision"), &RCalcEngine::global_set_precision);
    ClassDB::bind_static_method("RCalcEngine", D_METHOD("global_set_verbosity", "verbosity"), &RCalcEngine::global_set_verbosity);

    ADD_SIGNAL(MethodInfo("display_info", PropertyInfo(Variant::STRING, "info")));
    ADD_SIGNAL(MethodInfo("display_error", PropertyInfo(Variant::STRING, "error")));

    ADD_SIGNAL(MethodInfo("add_stack_item", PropertyInfo(Variant::OBJECT, "item", PROPERTY_HINT_RESOURCE_TYPE, "RCalcStackItem")));
    ADD_SIGNAL(MethodInfo("remove_stack_item"));
    ADD_SIGNAL(MethodInfo("replace_stack_items", PropertyInfo(Variant::ARRAY, "items", PROPERTY_HINT_ARRAY_TYPE, "RCalcStackItem")));
}

RCalcEngine::RCalcEngine() {
    p_application = RCalc::Allocator::create<RCalc::Application>();
    p_application->set_max_stack_size(10000);
    p_application->early_init(RCalc::AppConfig());
    p_application->init();
    p_application->run();
    reinterpret_cast<RCalc::RCalcRendererProxy*>(p_application->get_renderer())->p_engine = this;
}

void RCalcEngine::submit_text(const String& text) {
    std::string str { text.utf8().ptr() };
    reinterpret_cast<RCalc::RCalcRendererProxy*>(p_application->get_renderer())->cb_submit_text(str);
}

bool RCalcEngine::try_renderer_command(const String& cmd) {
    if (!commands_bound) { return false; }
    String without_slash = cmd.substr(1);

    for (int index = 0; index < bound_commands.source.size(); ++index) {
        Ref<RCalcHelpCommand> gd_cmd = Object::cast_to<RCalcHelpCommand>(bound_commands.source[index]);
        if (without_slash.nocasecmp_to(gd_cmd->get_name()) == 0) {
            gd_cmd->execute();
            return true;
        }
        TypedArray<String> aliases = Object::cast_to<RCalcHelpCommand>(bound_commands.source[index])->get_aliases();

        if (!aliases.is_empty()) {
            for (size_t alias_index = 0; alias_index < aliases.size(); ++alias_index) {
                if (without_slash.nocasecmp_to(aliases[alias_index]) == 0) {
                    gd_cmd->execute();
                    return true;
                }
            }
        }
    }

    return false;
}

void RCalcEngine::init_suggestions(const String& gd_str) {
    std::string str { gd_str.utf8().ptr() };
    autocomp.init_suggestions(str, *p_application);
}

String RCalcEngine::get_next_suggestion() {
    std::optional<std::string> res = autocomp.get_next_suggestion();
    if (res) {
        return String::utf8(res->c_str(), res->length());
    }
    return String();
}

String RCalcEngine::get_previous_suggestion() {
    std::optional<std::string> res = autocomp.get_previous_suggestion();
    if (res) {
        return String::utf8(res->c_str(), res->length());
    }
    return String();
}

TypedArray<RCalcHelpCommand> RCalcEngine::get_help_commands() {
    return RCalcHelpCommand::get_help_commands(p_application);
}

void RCalcEngine::bind_commands(TypedArray<RCalcHelpCommand> commands) {
    if (commands_bound) {
        ERR_PRINT("This instance of RCalcEngine has already had its commands bound!");
        return;
    }
    commands_bound = true;
    bound_commands.source = commands;

    RCalc::CowVec<RCalc::CommandMeta> arr_commands;
    arr_commands.reserve(commands.size());

    RCalc::CowVec<const RCalc::CommandMeta*> arr_command_ptrs;
    arr_command_ptrs.reserve(commands.size());

    size_t alias_count = 0;
    for (size_t index = 0; index < commands.size(); ++index) {
        alias_count += Object::cast_to<RCalcHelpCommand>(commands[index])->get_aliases().size();
    }

    RCalc::CowVec<std::string> arr_strings;
    arr_strings.reserve(commands.size() * 2 + alias_count);

    for (size_t index = 0; index < commands.size(); ++index) {
        std::string& cmd_name = arr_strings.emplace_back(Object::cast_to<RCalcHelpCommand>(commands[index])->get_name().utf8().get_data());
        std::string& cmd_description = arr_strings.emplace_back(Object::cast_to<RCalcHelpCommand>(commands[index])->get_description().utf8().get_data());

        TypedArray<String> aliases = Object::cast_to<RCalcHelpCommand>(commands[index])->get_aliases();
        std::span<const char * const> cmd_aliases;

        if (!aliases.is_empty()) {
            RCalc::CowVec<const char*> alias_arr;
            alias_arr.reserve(aliases.size());

            for (size_t alias_index = 0; alias_index < aliases.size(); ++alias_index) {
                std::string& alias = arr_strings.emplace_back(aliases[alias_index].operator godot::String().utf8().get_data());
                alias_arr.push_back(alias.c_str());
            }

            cmd_aliases = std::span<const char * const> { alias_arr.begin(), alias_arr.end() };
            bound_commands.alias_arrs.push_back(alias_arr);
        }

        arr_commands.emplace_back(cmd_name.c_str(), cmd_description.c_str(), cmd_aliases);
    }

    for (size_t index = 0; index < commands.size(); ++index) {
        arr_command_ptrs.push_back(&arr_commands.at(index));
    }

    bound_commands.arr_commands = arr_commands;
    bound_commands.arr_command_ptrs = arr_command_ptrs;
    bound_commands.arr_strings = arr_strings;

    const char* scope_name = "RCalcEngine";
    std::span<RCalc::CommandMeta const * const> scope_cmds { arr_command_ptrs.begin(), arr_command_ptrs.end() };

    RCalc::ScopeMeta scope { scope_name, scope_cmds };

    p_application->get_command_map().register_scope(scope);
}

String RCalcEngine::get_version_string() {
    return String(VERSION_FULL_BUILD);
}

String RCalcEngine::get_version_hash() {
    return String(VERSION_HASH);
}

String RCalcEngine::get_program_info() {
    return String(RCalc::HelpText::program_info);
}

void RCalcEngine::global_set_precision(int precision) {
    RCalc::Value::set_precision(precision);
}

std::shared_ptr<GodotRCalcLogger> RCalcEngine::logger = nullptr;
void RCalcEngine::global_set_verbosity(int verbosity) {
    if (logger) {
        switch (verbosity) {
            case 0:
                logger->set_min_severity(RCalc::Logging::LOG_VERBOSE);
                break;
            case 1:
                logger->set_min_severity(RCalc::Logging::LOG_STANDARD);
                break;
            case 2:
                logger->set_min_severity(RCalc::Logging::LOG_ERROR);
                break;
        }
    }
}

RCalcEngine::~RCalcEngine() {
    p_application->cleanup();
    RCalc::Allocator::destroy(p_application);
}