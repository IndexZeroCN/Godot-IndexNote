#include "note_key.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void NoteKey::_bind_methods() {
	ClassDB::bind_static_method("NoteKey", D_METHOD("create", "units"), &NoteKey::create);

	ClassDB::bind_method(D_METHOD("get_units"), &NoteKey::get_units);
	ClassDB::bind_method(D_METHOD("get_displacement"), &NoteKey::get_displacement);
	ClassDB::bind_method(D_METHOD("get_secs"), &NoteKey::get_secs);

	ClassDB::bind_method(D_METHOD("get_properties"), &NoteKey::get_properties);
	ClassDB::bind_method(D_METHOD("set_properties", "properties"), &NoteKey::set_properties);
	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "properties"), "set_properties", "get_properties");
}

Ref<NoteKey> NoteKey::create(int p_units) {
	if (p_units < 0) {
		UtilityFunctions::push_error("units < 0");
		return Variant();
	}

	Ref<NoteKey> event;

	event.instantiate();
	event->units = p_units;
	
	return event;
}