/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include "EventListener.h"

namespace facebook::react {

bool EventListenerHolder::willProcessEvent(RawEvent &&rawEvent, EventPriority priority) const {
  std::shared_lock<butter::shared_mutex> lock(mutex_);

  bool handled = false;
  const EventTarget *eventTarget = rawEvent.eventTarget.get();
  const std::string &type = rawEvent.type;
  const ValueFactory &payloadFactory = rawEvent.payloadFactory;
  for (auto const &listener : eventListeners_) {
    handled = handled ||
        listener->operator()(eventTarget, type, priority, payloadFactory);
  }
  return handled;
}

void EventListenerHolder::addListener(
    const std::shared_ptr<EventListener const> &listener) {
  std::unique_lock<butter::shared_mutex> lock(mutex_);

  eventListeners_.push_back(listener);
}

void EventListenerHolder::removeListener(
    const std::shared_ptr<EventListener const> &listener) {
  std::unique_lock<butter::shared_mutex> lock(mutex_);

  auto it = std::find(eventListeners_.begin(), eventListeners_.end(), listener);
  if (it != eventListeners_.end()) {
    eventListeners_.erase(it);
  }
}

} // namespace facebook::react
