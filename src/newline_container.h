// This file is modified from `flow_container.h`, from the Godot source.
// The license for the original file is included below:

/**************************************************************************/
/*  flow_container.h                                                      */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#pragma once

#include <godot_cpp/classes/container.hpp>

using namespace godot;

class RCalcNewlineFlowContainer : public Container {
	GDCLASS(RCalcNewlineFlowContainer, Container);

public:
	enum AlignmentMode {
		ALIGNMENT_BEGIN,
		ALIGNMENT_CENTER,
		ALIGNMENT_END
	};

private:
	int cached_size = 0;
	int cached_line_count = 0;

	bool vertical = false;
	AlignmentMode alignment = ALIGNMENT_BEGIN;

	struct ThemeCache {
		int h_separation = 0;
		int v_separation = 0;
	} theme_cache;

	void _resort();

protected:
	bool is_fixed = false;

	void _notification(int p_what);
	void _validate_property(PropertyInfo &p_property) const;
	static void _bind_methods();

public:
	int get_line_count() const;

	void set_alignment(AlignmentMode p_alignment);
	AlignmentMode get_alignment() const;

	void set_vertical(bool p_vertical);
	bool is_vertical() const;

	void set_h_separation(int p_separation) { theme_cache.h_separation = p_separation; }
	int get_h_separation() const { return theme_cache.h_separation; }

	void set_v_separation(int p_separation) { theme_cache.v_separation = p_separation; }
	int get_v_separation() const { return theme_cache.v_separation; }

	virtual Size2 _get_minimum_size() const override;

	virtual PackedInt32Array _get_allowed_size_flags_horizontal() const override;
	virtual PackedInt32Array _get_allowed_size_flags_vertical() const override;

	RCalcNewlineFlowContainer(bool p_vertical = false);
};

VARIANT_ENUM_CAST(RCalcNewlineFlowContainer::AlignmentMode);
