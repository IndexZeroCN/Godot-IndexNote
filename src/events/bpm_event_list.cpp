#include "bpm_event_list.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void BpmEventList::_bind_methods() {
    ClassDB::bind_static_method("BpmEventList", D_METHOD("create", "init_bpm", "units_abeat"), &BpmEventList::create);

	ClassDB::bind_method(D_METHOD("get_init_bpm"), &BpmEventList::get_init_bpm);
	ClassDB::bind_method(D_METHOD("set_init_bpm", "init_bpm"), &BpmEventList::set_init_bpm);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "init_bpm"), "set_init_bpm", "get_init_bpm");
    
	ClassDB::bind_method(D_METHOD("get_units_abeat"), &BpmEventList::get_units_abeat);

    ClassDB::bind_method(D_METHOD("add_event", "event"), &BpmEventList::add_event);
    ClassDB::bind_method(D_METHOD("remove_event", "event"), &BpmEventList::remove_event);
    ClassDB::bind_method(D_METHOD("remove_event_at", "index"), &BpmEventList::remove_event_at);
    ClassDB::bind_method(D_METHOD("get_event_index", "units"), &BpmEventList::get_event_index);
    ClassDB::bind_method(D_METHOD("get_event", "units"), &BpmEventList::get_event);
    ClassDB::bind_method(D_METHOD("get_event_at", "index"), &BpmEventList::get_event_at);
    ClassDB::bind_method(D_METHOD("size"), &BpmEventList::size);
    
    ClassDB::bind_method(D_METHOD("add_bpm", "units", "bpm"), &BpmEventList::add_bpm);
    ClassDB::bind_method(D_METHOD("get_bpm", "units"), &BpmEventList::get_bpm);

    ClassDB::bind_method(D_METHOD("get_secs", "units"), &BpmEventList::get_secs);
    ClassDB::bind_method(D_METHOD("get_units", "secs"), &BpmEventList::get_units);
    ClassDB::bind_method(D_METHOD("get_beats", "secs"), &BpmEventList::get_beats);

    ClassDB::bind_method(D_METHOD("beats_to_units", "beats"), &BpmEventList::beats_to_units);
    ClassDB::bind_method(D_METHOD("units_to_beats", "units"), &BpmEventList::units_to_beats);

    ClassDB::bind_method(D_METHOD("update_secs", "from"), &BpmEventList::update_secs);
}

Ref<BpmEventList> BpmEventList::create(double p_init_bpm, int p_units_abeat) {
    if (p_init_bpm <= 0) {
		UtilityFunctions::push_error("init_bpm <= 0");
		return Variant();
	}

    if (p_units_abeat <= 0) {
		UtilityFunctions::push_error("units_abeat <= 0");
		return Variant();
    }

    Ref<BpmEventList> event_list;

    event_list.instantiate();
    event_list->init_bpm = p_init_bpm;
    event_list->units_abeat = p_units_abeat;

    return event_list;
}

void BpmEventList::set_init_bpm(double p_init_bpm) {
    if (p_init_bpm <= 0) {
		UtilityFunctions::push_error("init_bpm <= 0");
		return;
	}

    init_bpm = p_init_bpm;
    update_secs(0);
}

void BpmEventList::add_event(Ref<BpmEvent> p_event) {
    if (p_event.is_null()) {
        UtilityFunctions::push_error("event is null");
        return;
    }

    int units = p_event->units;
    int i = _events.size() - 1;

    _events.push_back(p_event);

    while (i >= 0 && _events[i]->units >= units) {
        _events[i + 1] = _events[i];
        i--;
    }

    _events[i + 1] = p_event;
    update_secs(i + 1);
}

bool BpmEventList::remove_event(Ref<BpmEvent> p_event) {
    int size = _events.size();

    for (int i = 0; i < size; i++) {
        if (_events[i] == p_event) {
            _events.erase(_events.begin() + i);
            update_secs(i);
            return true;
        }
    }

    return false;
}

void BpmEventList::remove_event_at(int p_index) {
    if (p_index < 0 || p_index >= _events.size()) {
        UtilityFunctions::push_error("index out of range");
        return;
    }

    _events.erase(_events.begin() + p_index);
    update_secs(p_index);
}

int BpmEventList::get_event_index(double p_units) const {
    if (_events.empty()) return -1;

    int left = 0;
    int right = _events.size() - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (_events[mid]->units <= p_units) {
            result = mid; left = mid + 1;
        }

        else right = mid - 1;
    }

    return result;
}

Ref<BpmEvent> BpmEventList::get_event(double p_units) const {
    int index = get_event_index(p_units);

    if (index == -1) return Variant();

    return _events[index];
}

Ref<BpmEvent> BpmEventList::get_event_at(int p_index) const {
    if (p_index < 0 || p_index >= _events.size()) {
		UtilityFunctions::push_error("index out of range");
		return Variant();
    }

    return _events[p_index];
}

void BpmEventList::add_bpm(int p_units, double p_bpm) {
	if (p_units < 0) {
		UtilityFunctions::push_error("units < 0");
		return;
	}

	if (p_bpm <= 0) {
		UtilityFunctions::push_error("bpm <= 0");
		return;
	}

    add_event(BpmEvent::create(p_units, p_bpm));
}

double BpmEventList::get_bpm(double p_units) const {
    int index = get_event_index(p_units);

    if (index == -1) return init_bpm;

    return _events[index]->bpm;
}

double BpmEventList::get_secs(double p_units) const {
    int index = get_event_index(p_units);

    if (_events.empty() || index == -1)
        return 60.0 / init_bpm * p_units / units_abeat;

    Ref<BpmEvent> event = _events[index];

    return event->pre_secs + 60.0 / event->bpm * (p_units - event->units) / units_abeat;
}

double BpmEventList::get_units(double p_secs) const {
    int left = 0;
    int right = _events.size() - 1;
    int index = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (_events[mid]->pre_secs <= p_secs) {
            index = mid; left = mid + 1;
        }

        else right = mid - 1;
    }

    if (index == -1)
        return p_secs * init_bpm / 60.0 * units_abeat;

    Ref<BpmEvent> event = _events[index];

    return event->units + (p_secs - event->pre_secs) * event->bpm / 60.0 * units_abeat;
}

void BpmEventList::update_secs(int from) {
    int size = _events.size();

    if (size == 0) return;

    if (from == 0) {
        Ref<BpmEvent> event = _events[0];
        event->pre_secs = 60.0 / init_bpm * event->units / units_abeat;
        from++;
    }

    for (int i = from; i < size; i++) {
        Ref<BpmEvent> pre_event = _events[i - 1];
        Ref<BpmEvent> event = _events[i];

        event->pre_secs = pre_event->pre_secs
            + 60.0 / pre_event->bpm * (event->units - pre_event->units) / units_abeat;
    }
}