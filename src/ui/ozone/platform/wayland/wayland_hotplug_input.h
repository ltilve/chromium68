// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_OZONE_PLATFORM_WAYLAND_WAYLAND_HOTPLUG_INPUT_H_
#define UI_OZONE_PLATFORM_WAYLAND_WAYLAND_HOTPLUG_INPUT_H_

#include "base/macros.h"
#include "ui/events/ozone/evdev/event_dispatch_callback.h"

namespace ui {

class WaylandHotplugInput {
 public:
  WaylandHotplugInput(const EventDispatchCallback& callback);
  virtual ~WaylandHotplugInput() = default;

  virtual void set_connection(WaylandConnection* connection) {
    connection_ = connection;
  }
  int GetId() const { return id_; }
  void SetName(const std::string& name) { name_ = name; }
  std::string GetName() const { return name_; }

 protected:
  WaylandConnection* connection() const {
    return connection_;
  }

 private:
  int id_;
  std::string name_;
  WaylandConnection* connection_ = nullptr;
  EventDispatchCallback callback_;

  DISALLOW_COPY_AND_ASSIGN(WaylandHotplugInput);
};

}  // namespace ui

#endif  // UI_OZONE_PLATFORM_WAYLAND_WAYLAND_HOTPLUG_INPUT_H_
