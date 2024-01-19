#include "help.h"

#include "app/help_text.h"
#include "app/commands/commands.h"
#include "app/operators/operators.h"
#include "core/units/units.h"
#include "app/stack.h"
#include "core/filter.h"
#include "renderer_proxy.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;


TypedArray<RCalcHelpSection> RCalcHelpSection::get_help_sections() {
    TypedArray<RCalcHelpSection> gd_sections;
    gd_sections.resize(RCalc::HelpText::sections.size());

    for (size_t index = 0; index < RCalc::HelpText::sections.size(); ++index) {
        const RCalc::HelpText::HelpSection& section = RCalc::HelpText::sections[index];
        Ref<RCalcHelpSection> gd_section = memnew(RCalcHelpSection);

        gd_section->header = String::utf8(section.header);
        gd_section->text = String::utf8(section.text);

        gd_sections[index] = gd_section;
    }

    gd_sections.make_read_only();
    return gd_sections;
}

void RCalcHelpSection::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_header"), &RCalcHelpSection::get_header);
    ClassDB::bind_method(D_METHOD("get_text"), &RCalcHelpSection::get_text);

    ClassDB::bind_static_method("RCalcHelpSection", D_METHOD("get_help_sections"), &RCalcHelpSection::get_help_sections);
}


TypedArray<RCalcHelpCommand> RCalcHelpCommand::get_help_commands(RCalc::Application* p_app) {
    TypedArray<RCalcHelpCommand> gd_commands;
    const std::vector<RCalc::CommandMeta const *>& commands = p_app->get_command_map().get_alphabetical();
    gd_commands.resize(commands.size());

    for (size_t index = 0; index < commands.size(); ++index) {
        RCalc::CommandMeta const * const command = commands[index];
        Ref<RCalcHelpCommand> gd_command = memnew(RCalcHelpCommand);

        gd_command->name = String::utf8(command->name);
        gd_command->description = String::utf8(command->description);
        
        for (const char* const alias : command->aliases) {
            gd_command->aliases.append(String::utf8(alias));
        }

        gd_commands[index] = gd_command;
    }

    gd_commands.make_read_only();
    return gd_commands;
}

Ref<RCalcHelpCommand> RCalcHelpCommand::bind(String name, String description, TypedArray<String> aliases, Callable callback) {
    Ref<RCalcHelpCommand> command = memnew(RCalcHelpCommand);
    command->name = name;
    command->description = description;
    command->aliases = aliases;
    command->callback = callback;
    return command;
}

void RCalcHelpCommand::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &RCalcHelpCommand::get_name);
    ClassDB::bind_method(D_METHOD("get_description"), &RCalcHelpCommand::get_description);
    ClassDB::bind_method(D_METHOD("get_aliases"), &RCalcHelpCommand::get_aliases);

    ClassDB::bind_static_method("RCalcHelpCommand", D_METHOD("bind", "name", "description", "aliases", "callback"), &RCalcHelpCommand::bind);
}

void RCalcHelpCommand::execute() const {
    if (callback.is_valid()) {
        callback.call();
    }
}


TypedArray<RCalcStackItem> RCalcHelpOperator::get_examples() {
    if (!examples_cached) {
        cache_examples();
    }
    return cached_examples;
}

void RCalcHelpOperator::cache_examples() {
    if (example_inputs == nullptr) { return; }
    cached_examples.clear();

    RCalc::RPNStack stack;
    RCalc::OperatorMap& op_map = RCalc::OperatorMap::get_operator_map();

    for (const std::span<const char * const>& example_params : *example_inputs) {
        stack.clear();

        for (const char* param : example_params) {
            RCalc::Value value = RCalc::Value::parse(param).value();
            stack.try_push_item(RCalc::StackItem { RCalc::create_displayables_from(value), std::move(value), false });
        }

        std::string op_name = RCalc::filter_name(name.utf8().get_data());
        RCalc::Result<std::optional<size_t>> err = op_map.evaluate(op_name, stack);

        if (!err) {
            UtilityFunctions::printerr("Cannot display example: ", err.unwrap_err().get_message().c_str());
            continue;
        }

        bool first = true;
        std::stringstream ss;

        for (const RCalc::StackItem& item : stack.get_items()) {
            if (first) {
                first = false;
            }
            else {
                ss << ", ";
            }

            ss << item.result.to_string();
        }

        RCalc::CowVec<RCalc::StackItem> _items = stack.pop_items(1);
        const RCalc::StackItem& res = _items[0];

        Ref<RCalcStackItem> gd_item = RCalc::RCalcRendererProxy::convert_item(res);
        gd_item->output = String::utf8(ss.str().c_str());
        cached_examples.append(gd_item);
    }
}

bool RCalcHelpOperator::has_examples() const {
    if (example_inputs == nullptr) { return false; }
    return example_inputs->size();
}

void RCalcHelpOperator::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &RCalcHelpOperator::get_name);
    ClassDB::bind_method(D_METHOD("get_description"), &RCalcHelpOperator::get_description);
    ClassDB::bind_method(D_METHOD("get_param_count"), &RCalcHelpOperator::get_param_count);
    ClassDB::bind_method(D_METHOD("get_allowed_types"), &RCalcHelpOperator::get_allowed_types);
    ClassDB::bind_method(D_METHOD("get_examples"), &RCalcHelpOperator::get_examples);
    ClassDB::bind_method(D_METHOD("has_examples"), &RCalcHelpOperator::has_examples);
}


TypedArray<RCalcHelpOperatorCategory> RCalcHelpOperatorCategory::get_help_operator_categories() {
    TypedArray<RCalcHelpOperatorCategory> gd_categories;
    const std::span<RCalc::OperatorCategory const * const>& categories = RCalc::OperatorMap::get_operator_map().get_alphabetical();
    gd_categories.resize(categories.size());

    for (size_t index = 0; index < categories.size(); ++index) {
        RCalc::OperatorCategory const * const category = categories[index];
        Ref<RCalcHelpOperatorCategory> gd_category = memnew(RCalcHelpOperatorCategory);

        if (category->category_name) {
            gd_category->name = String::utf8(category->category_name.value());
        }

        gd_category->operators.resize(category->category_ops.size());

        for (size_t op_index = 0; op_index < category->category_ops.size(); ++op_index) {
            RCalc::Operator const * const op = category->category_ops[op_index];
            Ref<RCalcHelpOperator> gd_op = memnew(RCalcHelpOperator);
            
            gd_op->name = String::utf8(op->name);
            gd_op->description = String::utf8(op->description);
            gd_op->param_count = op->param_count;

            for (const std::span<const RCalc::Type>& typeset : op->allowed_types) {
                TypedArray<String> gd_typeset;
                for (const RCalc::Type& type : typeset) {
                    gd_typeset.append(String::utf8(RCalc::Value::get_type_name(type)));
                }
                gd_typeset.make_read_only();
                gd_op->allowed_types.append(gd_typeset);
            }
            gd_op->allowed_types.make_read_only();

            gd_op->example_inputs = &op->examples;

            gd_category->operators[op_index] = gd_op;
        }

        gd_category->operators.make_read_only();
        gd_categories[index] = gd_category;
    }

    gd_categories.make_read_only();
    return gd_categories;
}

void RCalcHelpOperatorCategory::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &RCalcHelpOperatorCategory::get_name);
    ClassDB::bind_method(D_METHOD("get_operators"), &RCalcHelpOperatorCategory::get_operators);

    ClassDB::bind_static_method("RCalcHelpOperatorCategory", D_METHOD("get_help_operator_categories"), &RCalcHelpOperatorCategory::get_help_operator_categories);
}


void RCalcHelpUnit::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &RCalcHelpUnit::get_name);
    ClassDB::bind_method(D_METHOD("get_usage"), &RCalcHelpUnit::get_usage);
}


TypedArray<RCalcHelpUnitFamily> RCalcHelpUnitFamily::get_help_unit_families() {
    TypedArray<RCalcHelpUnitFamily> gd_families;
    const std::span<RCalc::UnitFamily const * const>& families = RCalc::UnitsMap::get_units_map().get_alphabetical();
    gd_families.resize(families.size());

    for (size_t index = 0; index < families.size(); ++index) {
        RCalc::UnitFamily const * const family = families[index];
        Ref<RCalcHelpUnitFamily> gd_family = memnew(RCalcHelpUnitFamily);

        gd_family->name = String::utf8(family->p_name);
        gd_family->base_type = String::utf8(RCalc::Value::get_type_name(family->base_type));
        gd_family->units.resize(family->units.size());

        for (size_t unit_index = 0; unit_index < family->units.size(); ++unit_index) {
            RCalc::Unit const * const unit = family->units[unit_index];
            Ref<RCalcHelpUnit> gd_unit = memnew(RCalcHelpUnit);
            
            gd_unit->name = String::utf8(unit->p_name);
            gd_unit->usage = String::utf8(unit->p_usage);

            gd_family->units[unit_index] = gd_unit;
        }

        gd_family->units.make_read_only();
        gd_families[index] = gd_family;
    }

    gd_families.make_read_only();
    return gd_families;
}

void RCalcHelpUnitFamily::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_name"), &RCalcHelpUnitFamily::get_name);
    ClassDB::bind_method(D_METHOD("get_units"), &RCalcHelpUnitFamily::get_units);
    ClassDB::bind_method(D_METHOD("get_base_type"), &RCalcHelpUnitFamily::get_base_type);

    ClassDB::bind_static_method("RCalcHelpUnitFamily", D_METHOD("get_help_unit_families"), &RCalcHelpUnitFamily::get_help_unit_families);
}