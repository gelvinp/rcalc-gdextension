#include "stack_item.h"

#include <godot_cpp/classes/editor_interface.hpp>

using namespace godot;

void RCalcStackItem::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_inputs"), &RCalcStackItem::get_inputs);
    ClassDB::bind_method(D_METHOD("get_output"), &RCalcStackItem::get_output);
    ClassDB::bind_method(D_METHOD("get_copyable"), &RCalcStackItem::get_copyable);

    ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "inputs", PROPERTY_HINT_ARRAY_TYPE, "String"), "", "get_inputs");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "output"), "", "get_output");
    ADD_PROPERTY(PropertyInfo(Variant::STRING, "copyable"), "", "get_copyable");
}
