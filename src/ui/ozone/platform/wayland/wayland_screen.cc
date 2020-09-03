// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/ozone/platform/wayland/wayland_screen.h"

#include "ui/display/display.h"
#include "ui/display/display_finder.h"
#include "ui/display/display_observer.h"
#include "ui/gfx/geometry/point.h"
#include "ui/gfx/geometry/size.h"
#include "ui/ozone/platform/wayland/wayland_connection.h"
#include "ui/ozone/platform/wayland/wayland_window.h"

namespace ui {

WaylandScreen::WaylandScreen(WaylandConnection* connection)
    : connection_(connection), weak_factory_(this) {
  DCHECK(connection_);
}

WaylandScreen::~WaylandScreen() = default;

void WaylandScreen::OnOutputAdded(uint32_t output_id, bool is_primary) {
  display::Display new_display(output_id);
  display_list_.AddDisplay(std::move(new_display),
                           is_primary
                               ? display::DisplayList::Type::PRIMARY
                               : display::DisplayList::Type::NOT_PRIMARY);
}

void WaylandScreen::OnOutputRemoved(uint32_t output_id) {
  display_list_.RemoveDisplay(output_id);
}

void WaylandScreen::OnOutputMetricsChanged(uint32_t output_id,
                                           const gfx::Rect& new_bounds,
                                           float device_pixel_ratio,
                                           bool is_primary) {
  display::Display changed_display(output_id);
  changed_display.set_device_scale_factor(device_pixel_ratio);
  changed_display.set_bounds(new_bounds);
  changed_display.set_work_area(new_bounds);

  display_list_.UpdateDisplay(
      changed_display, is_primary ? display::DisplayList::Type::PRIMARY
                                  : display::DisplayList::Type::NOT_PRIMARY);
}

base::WeakPtr<WaylandScreen> WaylandScreen::GetWeakPtr() {
  return weak_factory_.GetWeakPtr();
}

const std::vector<display::Display>& WaylandScreen::GetAllDisplays() const {
  return display_list_.displays();
}

display::Display WaylandScreen::GetPrimaryDisplay() const {
  auto iter = display_list_.GetPrimaryDisplayIterator();
  if (iter == display_list_.displays().end())
    return display::Display::GetDefaultDisplay();
  return *iter;
}

display::Display WaylandScreen::GetDisplayForAcceleratedWidget(
    gfx::AcceleratedWidget widget) const {
  auto* wayland_window = connection_->GetWindow(widget);
  DCHECK(wayland_window);

  const std::set<uint32_t> entered_outputs_ids =
      wayland_window->GetEnteredOutputsIds();
  // Although spec says a surface receives enter/leave surface events on
  // create/move/resize actions, this might be called right after a window is
  // created, but it has not been configured by a Wayland compositor and it has
  // not received enter surface events yet. Another case is when a user switches
  // between displays in a single output mode - Wayland may not send enter
  // events immediately, which can result in empty container of entered ids
  // (check comments in WaylandWindow::RemoveEnteredOutputId). In this case,
  // it's also safe to return the primary display.
  if (entered_outputs_ids.empty())
    return GetPrimaryDisplay();

  DCHECK(!display_list_.displays().empty());

  // A widget can be located on two or more displays. It would be better if the
  // most in pixels occupied display was returned, but it's impossible to do in
  // Wayland. Thus, return the one, which was the very first used.
  for (const auto& display : display_list_.displays()) {
    if (display.id() == *entered_outputs_ids.begin())
      return display;
  }

  NOTREACHED();
  return GetPrimaryDisplay();
}

gfx::Point WaylandScreen::GetCursorScreenPoint() const {
  NOTIMPLEMENTED_LOG_ONCE();
  return gfx::Point();
}

gfx::AcceleratedWidget WaylandScreen::GetAcceleratedWidgetAtScreenPoint(
    const gfx::Point& point) const {
  // It is safe to check only for focused windows and test if they contain the
  // point or not.
  auto* window = connection_->GetCurrentFocusedWindow();
  if (window && window->GetBounds().Contains(point))
    return window->GetWidget();
  return gfx::kNullAcceleratedWidget;
}

display::Display WaylandScreen::GetDisplayNearestPoint(
    const gfx::Point& point) const {
  NOTIMPLEMENTED_LOG_ONCE();
  return GetPrimaryDisplay();
}

display::Display WaylandScreen::GetDisplayMatching(
    const gfx::Rect& match_rect) const {
  // TODO(msisov): https://crbug.com/890272
  NOTIMPLEMENTED_LOG_ONCE();
  return GetPrimaryDisplay();
}

void WaylandScreen::AddObserver(display::DisplayObserver* observer) {
  display_list_.AddObserver(observer);
}

void WaylandScreen::RemoveObserver(display::DisplayObserver* observer) {
  display_list_.RemoveObserver(observer);
}

}  // namespace ui
