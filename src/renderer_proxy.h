#pragma once

#include "app/renderers/renderer.h"
#include "rcalc.h"
#include "stack_item.h"

namespace RCalc {

class RCalcRendererProxy : public Renderer {
public:
    virtual void early_init(const AppConfig& config, SubmitTextCallback submit_text) override;
    virtual Result<> init() override { return Ok(); }
    virtual void render_loop() override {}
    virtual void cleanup() override {}

    virtual void display_info(std::string_view str) override;
    virtual void display_error(std::string_view str) override;

    virtual bool try_renderer_command(std::string_view str) override;

    virtual void add_stack_item(const StackItem& item) override;
    virtual void remove_stack_item() override;
    virtual void replace_stack_items(const CowVec<StackItem>& items) override;

    SubmitTextCallback cb_submit_text;
    RCalcEngine* p_engine = nullptr;

    static Ref<RCalcStackItem> convert_item(const StackItem& item);
    static Variant convert_value(const Value& value);
};

}