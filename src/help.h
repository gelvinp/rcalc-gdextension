#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/typed_array.hpp>

#include "app/application.h"
#include "app/stack.h"
#include "stack_item.h"

using namespace godot;


class RCalcHelpSection : public RefCounted {
    GDCLASS(RCalcHelpSection, RefCounted);

public:
    String get_header() const { return header; }
    String get_text() const { return text; }

    static TypedArray<RCalcHelpSection> get_help_sections();

protected:
    static void _bind_methods();

private:
    String header;
    String text;
};


class RCalcHelpCommand : public RefCounted {
    GDCLASS(RCalcHelpCommand, RefCounted);

public:
    String get_name() const { return name; }
    String get_description() const { return description; }
    TypedArray<String> get_aliases() const { return aliases; }

    static Ref<RCalcHelpCommand> bind(String name, String description, TypedArray<String> aliases, Callable callback);

protected:
    static void _bind_methods();

private:
    String name;
    String description;
    TypedArray<String> aliases;
    Callable callback;

public:
    void execute() const;
    static TypedArray<RCalcHelpCommand> get_help_commands(RCalc::Application* p_app);
};


class RCalcHelpOperator : public RefCounted {
    GDCLASS(RCalcHelpOperator, RefCounted);

public:
    String get_name() const { return name; }
    String get_description() const { return description; }
    uint64_t get_param_count() const { return param_count; }
    Array get_allowed_types() const { return allowed_types; }
    TypedArray<RCalcStackItem> get_examples();
    bool has_examples() const;

protected:
    static void _bind_methods();

private:
    String name;
    String description;
    uint64_t param_count;
    Array allowed_types;

    TypedArray<RCalcStackItem> cached_examples;
    const std::span<const std::span<const char * const>>* example_inputs = nullptr;
    bool examples_cached = false;
    void cache_examples();

    friend class RCalcHelpOperatorCategory;
};


class RCalcHelpOperatorCategory : public RefCounted {
    GDCLASS(RCalcHelpOperatorCategory, RefCounted);

public:
    String get_name() const { return name; }
    TypedArray<RCalcHelpOperator> get_operators() const { return operators; }

    static TypedArray<RCalcHelpOperatorCategory> get_help_operator_categories();

protected:
    static void _bind_methods();

private:
    String name;
    TypedArray<RCalcHelpOperator> operators;
};


class RCalcHelpUnit : public RefCounted {
    GDCLASS(RCalcHelpUnit, RefCounted);

public:
    String get_name() const { return name; }
    String get_usage() const { return usage; }

protected:
    static void _bind_methods();

private:
    String name;
    String usage;

    friend class RCalcHelpUnitFamily;
};


class RCalcHelpUnitFamily : public RefCounted {
    GDCLASS(RCalcHelpUnitFamily, RefCounted);

public:
    String get_name() const { return name; }
    String get_base_type() const { return base_type; }
    TypedArray<RCalcHelpUnit> get_units() const { return units; }

    static TypedArray<RCalcHelpUnitFamily> get_help_unit_families();

protected:
    static void _bind_methods();

private:
    String name;
    String base_type;
    TypedArray<RCalcHelpUnit> units;
};

