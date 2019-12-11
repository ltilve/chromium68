// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/ozone/platform/wayland/wayland_hotplug_input.h"

#include "base/atomic_sequence_num.h"

namespace {
base::AtomicSequenceNumber id_seq_;
};

namespace ui {

WaylandHotplugInput::WaylandHotplugInput(const EventDispatchCallback& callback)
    : id_(id_seq_.GetNext()),
      callback_(callback) {
}

}  // namespace ui
