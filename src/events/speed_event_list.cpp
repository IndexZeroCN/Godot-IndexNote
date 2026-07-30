#include "speed_event_list.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <cmath>

using namespace godot;

void SpeedEventList::_bind_methods() {
    ClassDB::bind_static_method("SpeedEventList", D_METHOD("create", "init_speed", "bpm_events", "allow_negative_speed"), &SpeedEventList::create, DEFVAL(Variant()), DEFVAL(false));

    ClassDB::bind_method(D_METHOD("add_event", "event"), &SpeedEventList::add_event);
    ClassDB::bind_method(D_METHOD("remove_event", "event"), &SpeedEventList::remove_event);
    ClassDB::bind_method(D_METHOD("remove_event_at", "index"), &SpeedEventList::remove_event_at);
    ClassDB::bind_method(D_METHOD("get_event_index", "units"), &SpeedEventList::get_event_index);
    ClassDB::bind_method(D_METHOD("get_event", "units"), &SpeedEventList::get_event);
    ClassDB::bind_method(D_METHOD("get_event_at", "index"), &SpeedEventList::get_event_at);
    ClassDB::bind_method(D_METHOD("size"), &SpeedEventList::size);

    ClassDB::bind_method(D_METHOD("add_speed", "units_start", "units_end", "speed_start", "speed_end"), &SpeedEventList::add_speed);
    ClassDB::bind_method(D_METHOD("get_speed", "secs"), &SpeedEventList::get_speed);

    ClassDB::bind_method(D_METHOD("get_displacement", "secs"), &SpeedEventList::get_displacement);
    ClassDB::bind_method(D_METHOD("get_secs_by_displacement", "displacement"), &SpeedEventList::get_secs_by_displacement);

    ClassDB::bind_method(D_METHOD("update_displacements", "from"), &SpeedEventList::update_displacements);
}

Ref<SpeedEventList> SpeedEventList::create(
    double p_init_speed, Ref<BpmEventList> p_bpm_events, bool p_allow_negative_speed) {
    Ref<SpeedEventList> event_list;

    event_list.instantiate();
    event_list->_init_speed = p_init_speed;
    event_list->bpm_events = p_bpm_events;
    event_list->allow_negative_speed = p_allow_negative_speed;

    return event_list;
}

void SpeedEventList::add_event(Ref<SpeedEvent> p_event) {
    if (p_event.is_null()) {
        UtilityFunctions::push_error("event is null");
        return;
    }

    if ((p_event->speed_start < 0 || p_event->speed_end < 0) && !allow_negative_speed) {
        UtilityFunctions::push_error("allow_negative_speed = false & speed < 0");
        return;
    }

    int units = p_event->units_start;
    int i = _events.size() - 1;

    _events.push_back(p_event);

    while (i >= 0 && _events[i]->units_start >= units) {
        _events[i + 1] = _events[i];
        i--;
    }

    _events[i + 1] = p_event;
    update_displacements(i + 1);
}

bool SpeedEventList::remove_event(Ref<SpeedEvent> p_event) {
    int size = _events.size();

    for (int i = 0; i < size; i++) {
        if (_events[i] == p_event) {
            _events.erase(_events.begin() + i);
            update_displacements(i);
            return true;
        }
    }

    return false;
}

void SpeedEventList::remove_event_at(int p_index) {
    if (p_index < 0 || p_index >= _events.size()) {
        UtilityFunctions::push_error("index out of range");
        return;
    }

    _events.erase(_events.begin() + p_index);
    update_displacements(p_index);
}

int SpeedEventList::get_event_index(double p_units) const {
    if (_events.empty()) return -1;

    int left = 0;
    int right = _events.size() - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (_events[mid]->units_start <= p_units) {
            result = mid; left = mid + 1;
        }

        else right = mid - 1;
    }

    return result;
}

Ref<SpeedEvent> SpeedEventList::get_event(double p_units) const {
    int index = get_event_index(p_units);

    if (index == -1) return Variant();

    return _events[index];
}

Ref<SpeedEvent> SpeedEventList::get_event_at(int p_index) const {
    if (p_index < 0 || p_index >= _events.size()) {
		UtilityFunctions::push_error("index out of range");
		return Variant();
    }

    return _events[p_index];
}

void SpeedEventList::add_speed(int p_units_start, int p_units_end, double p_speed_start, double p_speed_end) {
    if (p_units_start < 0) {
		UtilityFunctions::push_error("units_start < 0");
		return;
	}
	
	if (p_units_end < 0) {
		UtilityFunctions::push_error("units_end < 0");
		return;
	}

	if (p_units_start > p_units_end) {
		UtilityFunctions::push_error("units_start > units_end");
		return;
	}

    if ((p_speed_start < 0 || p_speed_end < 0) && !allow_negative_speed) {
        UtilityFunctions::push_error("allow_negative_speed = false & speed < 0");
        return;
    }

    add_event(SpeedEvent::create(p_units_start, p_units_end, p_speed_start, p_speed_end));
}

double SpeedEventList::get_speed(double p_secs) const {
    if (bpm_events.is_null()) {
        UtilityFunctions::push_error("bpm_events is null");
        return 0;
    }

    double units = bpm_events->get_units(p_secs);
    int index = get_event_index(units);

    if (index == -1) {
        if (_events.empty()) return _init_speed;

        return _events[0]->speed_start;
    }

    Ref<SpeedEvent> event = _events[index];

    if (units >= event->units_end) return event->speed_end;

    double start_secs = bpm_events->get_secs(event->units_start);
    double delta_secs = bpm_events->get_secs(event->units_end) - start_secs;

    if (delta_secs == 0) {
        UtilityFunctions::push_error("delta_secs = 0");
        return 0;
    }

    double k = (event->speed_end - event->speed_start) / delta_secs;

    return event->speed_start + k * (p_secs - start_secs);
}

double SpeedEventList::get_displacement(double p_secs) const {
    if (bpm_events.is_null()) {
        UtilityFunctions::push_error("bpm_events is null");
        return 0;
    }
    
    double units = bpm_events->get_units(p_secs);
    int index = get_event_index(units);

    if (index == -1) {
        if (_events.empty()) return _init_speed * p_secs;

        return _events[0]->speed_start * p_secs;
    }

    Ref<SpeedEvent> event = _events[index];
    double start_secs = bpm_events->get_secs(event->units_start);
    double delta_secs = bpm_events->get_secs(event->units_end) - start_secs;

    return event->pre_displacement + event->get_displacement_at(delta_secs, p_secs - start_secs);
}

// Kimi 待测试
double SpeedEventList::get_secs_by_displacement(double p_displacement) const {
    if (allow_negative_speed) {
        UtilityFunctions::push_error("allow_negative_speed = true");
        return 0;
    }

    if (bpm_events.is_null()) {
        UtilityFunctions::push_error("bpm_events is null");
        return 0;
    }

    if (p_displacement < 0) {
        UtilityFunctions::push_error("displacement < 0");
        return 0;
    }

    if (p_displacement == 0) return 0;

    int left = 0;
    int right = _events.size() - 1;
    int index = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (_events[mid]->pre_displacement <= p_displacement) {
            index = mid; left = mid + 1;
        }

        else right = mid - 1;
    }

    if (index == -1) {
        if (_events.empty()) {
            if (_init_speed == 0) {
                UtilityFunctions::push_error("init_speed = 0");
                return 0;
            }

            return p_displacement / _init_speed;
        }

        if (_events[0]->speed_start == 0) {
            UtilityFunctions::push_error("speed_start = 0");
            return 0;
        }

        return p_displacement / _events[0]->speed_start;
    }

    Ref<SpeedEvent> event = _events[index];
    double d = p_displacement - event->pre_displacement;
    double start_secs = bpm_events->get_secs(event->units_start);
    double delta_secs = bpm_events->get_secs(event->units_end) - start_secs;

    if (delta_secs == 0) {
        UtilityFunctions::push_error("delta_secs = 0");
        return 0;
    }

    double full_displacement = event->get_displacement(delta_secs);

    if (d <= full_displacement) {
        double k = (event->speed_end - event->speed_start) / delta_secs;

        if (k == 0) {
            if (event->speed_start == 0) {
                UtilityFunctions::push_error("speed = 0");
                return 0;
            }

            return start_secs + d / event->speed_start;
        }

        return start_secs + (std::sqrt(event->speed_start * event->speed_start + 2 * k * d) - event->speed_start) / k;
    }

    if (event->speed_end == 0) {
        UtilityFunctions::push_error("speed_end = 0");
        return 0;
    }

    return start_secs + delta_secs + (d - full_displacement) / event->speed_end;
}

void SpeedEventList::update_displacements(int from) {
    if (bpm_events.is_null()) return;

    int size = _events.size();

    if (size == 0) return;

    if (from == 0) {
        Ref<SpeedEvent> event = _events[0];
        event->pre_displacement = event->speed_start * bpm_events->get_secs(event->units_start);
        from++;
    }

    for (int i = from; i < size; i++) {
        Ref<SpeedEvent> pre_event = _events[i - 1];
        Ref<SpeedEvent> event = _events[i];

        double pre_start_secs = bpm_events->get_secs(pre_event->units_start);
        double pre_delta_secs = bpm_events->get_secs(pre_event->units_end) - pre_start_secs;

        event->pre_displacement = pre_event->pre_displacement
            + pre_event->get_displacement_at(pre_delta_secs, bpm_events->get_secs(event->units_start) - pre_start_secs);
    }
}