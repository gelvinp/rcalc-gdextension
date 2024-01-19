#include "renderer_proxy.h"
#include "stack_item.h"
#include "core/format.h"
#include "matrices.h"

#include "core/memory/allocator.h"

#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace RCalc {

Result<Renderer*> Renderer::create(const AppConfig& config, SubmitTextCallback cb_submit_text) {
	RCalcRendererProxy* p_addr = Allocator::create<RCalcRendererProxy>();
    p_addr->early_init(config, cb_submit_text);
    return Ok(reinterpret_cast<Renderer*>(p_addr));
}


void RCalcRendererProxy::early_init(const AppConfig& config, SubmitTextCallback submit_text) {
    // Set up logger
    cb_submit_text = submit_text;
}


void RCalcRendererProxy::display_info(std::string_view str) {
    godot::String info { str.data() };
    p_engine->emit_signal("display_info", info);
}

void RCalcRendererProxy::display_error(std::string_view str) {
    godot::String error { str.data() };
    p_engine->emit_signal("display_error", error);
}

bool RCalcRendererProxy::try_renderer_command(std::string_view str) {
    godot::String cmd { str.data() };
    return p_engine->try_renderer_command(cmd);
}

void RCalcRendererProxy::add_stack_item(const StackItem& item) {
    p_engine->emit_signal("add_stack_item", convert_item(item));
}

void RCalcRendererProxy::remove_stack_item() {
    p_engine->emit_signal("remove_stack_item");
}

void RCalcRendererProxy::replace_stack_items(const CowVec<StackItem>& items) {
    TypedArray<RCalcStackItem> arr;

    for (const StackItem& item : items) {
        arr.append(convert_item(item));
    }

    arr.make_read_only();
    p_engine->emit_signal("replace_stack_items", arr);
}

Ref<RCalcStackItem> RCalcRendererProxy::convert_item(const StackItem& item) {
    Ref<RCalcStackItem> gd_item = memnew(RCalcStackItem);
    std::string str;

    for (Displayable& disp : *item.p_input) {
        switch (disp.get_type()) {
            case Displayable::Type::CONST_CHAR: {
                str = reinterpret_cast<ConstCharDisplayable&>(disp).p_char;
                break;
            }
            case Displayable::Type::STRING: {
                str = reinterpret_cast<StringDisplayable&>(disp).str.c_str();
                break;
            }
            case Displayable::Type::VALUE: {
                str = reinterpret_cast<ValueDisplayable&>(disp).value.to_string(disp.tags);
                break;
            }
            case Displayable::Type::RECURSIVE:
                UNREACHABLE(); // Handled by the iterator
        }
        gd_item->inputs.append(String::utf8(str.c_str(), str.length()));
    }
    
    gd_item->inputs.make_read_only();

    str = item.result.to_string(item.p_input->tags);
    gd_item->output = String::utf8(str.c_str(), str.length());
    gd_item->copyable = convert_value(item.result);
    return gd_item;
}

Variant RCalcRendererProxy::convert_value(const Value& value) {
    switch (value.get_type()) {
        case TYPE_INT: {
            return value.operator Int();
        }
        case TYPE_REAL: {
            return value.operator Real();
        }
        case TYPE_VEC2: {
            const Vec2 vec = value.operator Vec2();
            return Vector2(vec.x, vec.y);
        }
        case TYPE_VEC3: {
            const Vec3 vec = value.operator Vec3();
            return Vector3(vec.x, vec.y, vec.z);
        }
        case TYPE_VEC4: {
            const Vec4 vec = value.operator Vec4();
            return Vector4(vec.x, vec.y, vec.z, vec.w);
        }

        default: {
            std::string str = value.to_string();
            return String::utf8(str.c_str(), str.length());
        }
    }
}

}
