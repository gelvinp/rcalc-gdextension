#pragma once

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector4.hpp>

#include "app/stack.h"

using namespace godot;

class RCalcMat2 : public RefCounted {
    GDCLASS(RCalcMat2, RefCounted);

public:
    const Vector2& get_row0() const { return row0; }
    const Vector2& get_row1() const { return row1; }

    Vector2 row0, row1;

protected:
    static void _bind_methods();
};

class RCalcMat3 : public RefCounted {
    GDCLASS(RCalcMat3, RefCounted);

public:
    const Vector3& get_row0() const { return row0; }
    const Vector3& get_row1() const { return row1; }
    const Vector3& get_row2() const { return row2; }

    Vector3 row0, row1, row2;

protected:
    static void _bind_methods();
};

class RCalcMat4 : public RefCounted {
    GDCLASS(RCalcMat4, RefCounted);

public:
    const Vector4& get_row0() const { return row0; }
    const Vector4& get_row1() const { return row1; }
    const Vector4& get_row2() const { return row2; }
    const Vector4& get_row3() const { return row3; }

    Vector4 row0, row1, row2, row3;

protected:
    static void _bind_methods();
};
