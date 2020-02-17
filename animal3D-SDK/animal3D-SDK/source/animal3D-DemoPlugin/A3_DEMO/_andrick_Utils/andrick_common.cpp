#include <A3_DEMO/_andrick_Utils/andrick_common.h>
#include <A3_DEMO/_andrick_Utils/andrick_text_formatter.h>
#include <A3_DEMO/_andrick_Event/andrick_eventsystem.h>

EventSystem& gEventSystem = EventSystem::get();
TextFormatter& gTextFormatter = TextFormatter::get();
a3_DemoState* gDemoState = nullptr;