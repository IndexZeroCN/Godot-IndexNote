#include "bpm_event.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void BpmEvent::_bind_methods() {
	ClassDB::bind_static_method("BpmEvent", D_METHOD("create", "units", "bpm"), &BpmEvent::create);

	ClassDB::bind_method(D_METHOD("get_units"), &BpmEvent::get_units);
	ClassDB::bind_method(D_METHOD("get_bpm"), &BpmEvent::get_bpm);
	ClassDB::bind_method(D_METHOD("get_secs_start"), &BpmEvent::get_secs_start);
}

Ref<BpmEvent> BpmEvent::create(int p_units, double p_bpm) {
	if (p_units < 0) {
		UtilityFunctions::push_error("units < 0");
		return Variant();
	}

	if (p_bpm <= 0) {
		UtilityFunctions::push_error("bpm <= 0");
		return Variant();
	}

	Ref<BpmEvent> event;

	event.instantiate();
	event->units = p_units;
	event->bpm = p_bpm;
	
	return event;
}