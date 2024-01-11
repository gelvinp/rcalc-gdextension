#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/array.hpp>

#include "app/stack.h"

using namespace godot;

class RCalcStackItem : public RefCounted {
    GDCLASS(RCalcStackItem, RefCounted);

public:
    const Array& get_inputs() const { return inputs; }
    const String& get_output() const { return output; }
    const Variant& get_copyable() const { return copyable; }

    Array inputs;
    String output;
    Variant copyable;

protected:
    static void _bind_methods();
};
