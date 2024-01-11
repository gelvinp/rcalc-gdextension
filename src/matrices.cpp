#include "matrices.h"

using namespace godot;


void RCalcMat2::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_row0"), &RCalcMat2::get_row0);
    ClassDB::bind_method(D_METHOD("get_row1"), &RCalcMat2::get_row1);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "row0"), "", "get_row0");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "row1"), "", "get_row1");
}

void RCalcMat3::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_row0"), &RCalcMat3::get_row0);
    ClassDB::bind_method(D_METHOD("get_row1"), &RCalcMat3::get_row1);
    ClassDB::bind_method(D_METHOD("get_row2"), &RCalcMat3::get_row2);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "row0"), "", "get_row0");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "row1"), "", "get_row1");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "row2"), "", "get_row2");
}

void RCalcMat4::_bind_methods() {
    ClassDB::bind_method(D_METHOD("get_row0"), &RCalcMat4::get_row0);
    ClassDB::bind_method(D_METHOD("get_row1"), &RCalcMat4::get_row1);
    ClassDB::bind_method(D_METHOD("get_row2"), &RCalcMat4::get_row2);
    ClassDB::bind_method(D_METHOD("get_row3"), &RCalcMat4::get_row3);

    ADD_PROPERTY(PropertyInfo(Variant::VECTOR4, "row0"), "", "get_row0");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR4, "row1"), "", "get_row1");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR4, "row2"), "", "get_row2");
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR4, "row3"), "", "get_row3");
}
