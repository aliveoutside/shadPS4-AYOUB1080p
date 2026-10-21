// SPDX-FileCopyrightText: Copyright 2026 shadPS4 Emulator Project
// SPDX-License-Identifier: GPL-2.0-or-later

#include <algorithm>
#include <cmath>
#include <utility>

#include "common/types.h"
#include "core/libraries/pad/pad.h"
#include "input/controller.h"

namespace Input {

using Libraries::Pad::OrbisPadButtonDataOffset;

void State::OnButton(OrbisPadButtonDataOffset button, bool is_pressed) {
    if (is_pressed) {
        buttonsState |= button;
    } else {
        buttonsState &= ~button;
    }
}

void State::OnAxis(Axis axis, int value, u64 timestamp, bool smooth) {
    const auto index = std::to_underlying(axis);
    axes[index] = axis_smoothing_end_values[index];

    axis_smoothing_start_times[index] = timestamp;
    axis_smoothing_start_values[index] = axes[index];
    axis_smoothing_end_values[index] = value;
    axis_smoothing_flags[index] = smooth;
    const auto toggle = [&](const auto button) {
        if (value > 0) {
            buttonsState |= button;
        } else {
            buttonsState &= ~button;
        }
    };
    switch (axis) {
    case Axis::TriggerLeft:
        toggle(OrbisPadButtonDataOffset::L2);
        break;
    case Axis::TriggerRight:
        toggle(OrbisPadButtonDataOffset::R2);
        break;
    default:
        break;
    }
}

void State::OnTouchpad(int touch_index, bool is_down, float x, float y) {
    touchpad[touch_index].state = is_down;
    touchpad[touch_index].x = static_cast<u16>(x * 1920);
    touchpad[touch_index].y = static_cast<u16>(y * 941);
}

void State::OnGyro(const float gyro[3]) {
    angularVelocity.x = gyro[0];
    angularVelocity.y = gyro[1];
    angularVelocity.z = gyro[2];
}

void State::OnTouchpadButton(int index, bool down, float x, u8& next_id) {
    if (index < 0 || index >= 2) return;
    auto& touch = touchpad[index];
    if (down && !touch.state) {
        touch.ID = next_id;
        next_id = next_id == 127 ? 1 : next_id + 1;
    }
    OnTouchpad(index, down, x, 0.5f);
    OnButton(OrbisPadButtonDataOffset::TouchPad, touchpad[0].state || touchpad[1].state);
}

void State::OnTwoFingerTouchpad(bool down, u8& next_id) {
    for (int i = 0; i < 2; ++i) {
        auto& touch = touchpad[i];
        if (down && !touch.state) {
            touch.ID = next_id;
            next_id = next_id == 127 ? 1 : next_id + 1;
        }
        OnTouchpad(i, down, i == 0 ? 0.25f : 0.75f, 0.5f);
    }
    OnButton(OrbisPadButtonDataOffset::TouchPad, down);
}

void State::OnAccel(const float accel[3]) {
    acceleration.x = accel[0];
    acceleration.y = accel[1];
    acceleration.z = accel[2];
}

void State::OnSwipeTouchpad(int direction, float progress) {
    const float t = std::clamp(progress, 0.0f, 1.0f);
    float x = 0.5f, y = 0.5f;
    switch (direction) {
    case 0: y = 0.85f - 0.70f * t; break; // up
    case 1: y = 0.15f + 0.70f * t; break; // down
    case 2: x = 0.85f - 0.70f * t; break; // left
    case 3: x = 0.15f + 0.70f * t; break; // right
    default: return;
    }
    OnTouchpad(0, t < 1.0f, x, y);
    touchpad[1].state = false;
    OnButton(OrbisPadButtonDataOffset::TouchPad, false);
}

void State::UpdateAxisSmoothing(u64 timestamp) {
    for (int i = 0; i < std::to_underlying(Axis::AxisMax); ++i) {
        if (!axis_smoothing_flags[i] || std::abs(axes[i] - axis_smoothing_end_values[i]) < 16) {
            axes[i] = axis_smoothing_end_values[i];
            continue;
        }
        const f32 t = std::clamp(
            (timestamp - axis_smoothing_start_times[i]) / f32{axis_smoothing_time}, 0.f, 1.f);
        axes[i] = s32(axis_smoothing_start_values[i] * (1 - t) + axis_smoothing_end_values[i] * t);
    }
}

} // namespace Input
