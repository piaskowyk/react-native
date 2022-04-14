/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <string>

#include <react/renderer/core/EventTarget.h>
#include <react/renderer/core/ReactEventPriority.h>
#include <react/renderer/core/ValueFactory.h>
#include <react/renderer/core/RawEvent.h>

#include <butter/mutex.h>

namespace facebook {
namespace react {

/**
 * Listener for events dispatched to JS runtime.
 * Return `true` to interrupt default dispatch to JS event emitter, `false` to
 * pass through to default handlers.
 */
using EventListener = std::function<bool(
    const EventTarget *eventTarget,
    const std::string &type,
    ReactEventPriority priority,
    const ValueFactory &payloadFactory)>;

class EventListenerHolder {
 public:
  bool willProcessEvent(RawEvent &&rawEvent, EventPriority priority);

  void addListener(const std::shared_ptr<EventListener const> &listener);
  void removeListener(const std::shared_ptr<EventListener const> &listener);

 private:
  mutable butter::shared_mutex mutex_;
  std::vector<std::shared_ptr<EventListener const>> eventListeners_;
};

} // namespace react
} // namespace facebook
