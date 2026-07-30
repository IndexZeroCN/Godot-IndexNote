#include "speed_event.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void SpeedEvent::_bind_methods() {
	ClassDB::bind_static_method("SpeedEvent",
		D_METHOD("create", "units_start", "units_end", "speed_start", "speed_end"), &SpeedEvent::create);
		
	ClassDB::bind_method(D_METHOD("get_units_start"), &SpeedEvent::get_units_start);
	ClassDB::bind_method(D_METHOD("get_units_end"), &SpeedEvent::get_units_end);
	ClassDB::bind_method(D_METHOD("get_speed_start"), &SpeedEvent::get_speed_start);
	ClassDB::bind_method(D_METHOD("get_speed_end"), &SpeedEvent::get_speed_end);

	ClassDB::bind_method(D_METHOD("get_pre_displacement"), &SpeedEvent::get_pre_displacement);
	ClassDB::bind_method(D_METHOD("get_displacement", "delta_secs"), &SpeedEvent::get_displacement);
	ClassDB::bind_method(D_METHOD("get_displacement_at", "delta_secs", "secs"), &SpeedEvent::get_displacement_at);
}

Ref<SpeedEvent> SpeedEvent::create(int p_units_start, int p_units_end, double p_speed_start, double p_speed_end) {
	if (p_units_start < 0) {
		UtilityFunctions::push_error("units_start < 0");
		return Variant();
	}
	
	if (p_units_end < 0) {
		UtilityFunctions::push_error("units_end < 0");
		return Variant();
	}

	if (p_units_start > p_units_end) {
		UtilityFunctions::push_error("units_start > units_end");
		return Variant();
	}

	Ref<SpeedEvent> event;

	event.instantiate();
	event->units_start = p_units_start;
	event->units_end = p_units_end;
	event->speed_start = p_speed_start;
	event->speed_end = p_speed_end;
	
	return event;
}

double SpeedEvent::get_displacement(double p_delta_secs) const {
	if (p_delta_secs < 0) {
		UtilityFunctions::push_error("delta_secs < 0");
		return 0;
	}

	return (speed_start + speed_end) * p_delta_secs / 2;
}

double SpeedEvent::get_displacement_at(double p_delta_secs, double p_secs) const {
	if (p_delta_secs <= 0) {
		UtilityFunctions::push_error("delta_secs <= 0");
		return 0;
	}

	if (p_secs < 0) {
		UtilityFunctions::push_error("secs < 0");
		return 0;
	}

	if (p_secs <= p_delta_secs) {
		double k = (speed_end - speed_start) / p_delta_secs;
		return (2 * speed_start + p_secs * k) * p_secs / 2;
	}
	
	return get_displacement(p_delta_secs) + speed_end * (p_secs - p_delta_secs);
}