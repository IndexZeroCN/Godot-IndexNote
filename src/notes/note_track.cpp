#include "note_track.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <algorithm>

using namespace godot;

void NoteTrack::_bind_methods() {
    ClassDB::bind_static_method("NoteTrack",
        D_METHOD("create", "bpm_events", "speed_events", "sort_note_by_displacement"),
        &NoteTrack::create, DEFVAL(false));

    ClassDB::bind_method(D_METHOD("add_note", "note"), &NoteTrack::add_note);
    ClassDB::bind_method(D_METHOD("remove_note", "note"), &NoteTrack::remove_note);
    ClassDB::bind_method(D_METHOD("remove_note_at", "index"), &NoteTrack::remove_note_at);
    
    ClassDB::bind_method(D_METHOD("get_note_index", "units", "first"), &NoteTrack::get_note_index, DEFVAL(false));
    ClassDB::bind_method(D_METHOD("get_note", "units"), &NoteTrack::get_note);
    ClassDB::bind_method(D_METHOD("get_note_at", "index"), &NoteTrack::get_note_at);

    ClassDB::bind_method(D_METHOD("get_note_index_by_displacement", "displacement"), &NoteTrack::get_note_index_by_displacement);
    ClassDB::bind_method(D_METHOD("get_note_by_displacement", "displacement"), &NoteTrack::get_note_by_displacement);
    ClassDB::bind_method(D_METHOD("get_note_by_displacement_at", "index"), &NoteTrack::get_note_by_displacement_at);

    ClassDB::bind_method(D_METHOD("get_bpm_events"), &NoteTrack::get_bpm_events);
    ClassDB::bind_method(D_METHOD("get_speed_events"), &NoteTrack::get_speed_events);

    ClassDB::bind_method(D_METHOD("size"), &NoteTrack::size);

    ClassDB::bind_method(D_METHOD("add_bpm_event", "event"), &NoteTrack::add_bpm_event);
    ClassDB::bind_method(D_METHOD("remove_bpm_event", "event"), &NoteTrack::remove_bpm_event);
    ClassDB::bind_method(D_METHOD("remove_bpm_event_at", "index"), &NoteTrack::remove_bpm_event_at);
    ClassDB::bind_method(D_METHOD("add_bpm", "units", "bpm"), &NoteTrack::add_bpm);

    ClassDB::bind_method(D_METHOD("add_speed_event", "event"), &NoteTrack::add_speed_event);
    ClassDB::bind_method(D_METHOD("remove_speed_event", "event"), &NoteTrack::remove_speed_event);
    ClassDB::bind_method(D_METHOD("remove_speed_event_at", "index"), &NoteTrack::remove_speed_event_at);
    ClassDB::bind_method(D_METHOD("add_speed", "units_start", "units_end", "speed_start", "speed_end"), &NoteTrack::add_speed);

    ClassDB::bind_method(D_METHOD("update_displacements_and_secs"), &NoteTrack::update_displacements_and_secs, DEFVAL(true));
}

Ref<NoteTrack> NoteTrack::create(
    Ref<BpmEventList> p_bpm_events, Ref<SpeedEventList> p_speed_events,
    bool p_sort_note_by_displacement) {
    if (p_bpm_events.is_null()) {
        UtilityFunctions::push_error("bpm_events is null");
        return Variant();
    }

    if (p_speed_events.is_null()) {
        UtilityFunctions::push_error("speed_events is null");
        return Variant();
    }

    Ref<NoteTrack> track;

    p_speed_events->bpm_events = p_bpm_events;
    p_speed_events->update_displacements(0);

    track.instantiate();
    track->bpm_events = p_bpm_events;
    track->speed_events = p_speed_events;
    track->sort_note_by_displacement = p_sort_note_by_displacement;

    return track;
}

void NoteTrack::add_note(Ref<NoteKey> p_note) {
    if (p_note.is_null()) {
        UtilityFunctions::push_error("note is null");
        return;
    }

    int units = p_note->units;
    int i = _notes.size() - 1;
    double secs = bpm_events->get_secs(p_note->units);

    p_note->secs = secs;
    p_note->displacement = speed_events->get_displacement(secs);

    _notes.push_back(p_note);

    while (i >= 0 && _notes[i]->units > units) {
        _notes[i + 1] = _notes[i];
        i--;
    }

    int insert_index = i + 1;
    _notes[insert_index] = p_note;

    if (sort_note_by_displacement)
        _notes_sorted_by_displacement.push_back(p_note);

    update_displacements_and_secs(insert_index, false);
}

bool NoteTrack::remove_note(Ref<NoteKey> p_note) {
    int size = _notes.size();
    int index = -1;

    for (int i = 0; i < size; i++) {
        if (_notes[i] == p_note) {
            index = i;
            break;
        }
    }

    if (index == -1) return false;

    _notes.erase(_notes.begin() + index);

    if (sort_note_by_displacement) {
        int sorted_size = _notes_sorted_by_displacement.size();

        for (int i = 0; i < sorted_size; i++) {
            if (_notes_sorted_by_displacement[i] == p_note) {
                _notes_sorted_by_displacement.erase(_notes_sorted_by_displacement.begin() + i);
                break;
            }
        }
    }

    return true;
}

void NoteTrack::remove_note_at(int p_index) {
    if (p_index < 0 || p_index >= _notes.size()) {
        UtilityFunctions::push_error("index out of range");
        return;
    }
    
    if (sort_note_by_displacement) {
        Ref<NoteKey> note = _notes[p_index];
        int sorted_size = _notes_sorted_by_displacement.size();
        
        for (int i = 0; i < sorted_size; i++) {
            if (_notes_sorted_by_displacement[i] == note) {
                _notes_sorted_by_displacement.erase(_notes_sorted_by_displacement.begin() + i);
                break;
            }
        }
    }

    _notes.erase(_notes.begin() + p_index);
}

int NoteTrack::get_note_index(double p_units, bool p_first) const {
    if (_notes.empty()) return -1;

    int left = 0;
    int right = _notes.size() - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (_notes[mid]->units <= p_units) {
            result = mid; left = mid + 1;
        }

        else right = mid - 1;
    }

    if (result != -1 && p_first) {
        double units = _notes[result]->units;
        while (result > 0 && _notes[result - 1]->units == units) result--;
    }

    return result;
}

Ref<NoteKey> NoteTrack::get_note(double p_units) const {
    int index = get_note_index(p_units);

    if (index == -1) return Variant();

    return _notes[index];
}

Ref<NoteKey> NoteTrack::get_note_at(int p_index) const {
    if (p_index < 0 || p_index >= _notes.size()) {
		UtilityFunctions::push_error("index out of range");
		return Variant();
    }

    return _notes[p_index];
}

Ref<NoteKey> NoteTrack::get_note_by_displacement_at(int p_index) const {
    if (p_index < 0 || p_index >= _notes_sorted_by_displacement.size()) {
		UtilityFunctions::push_error("index out of range");
		return Variant();
    }

    return _notes_sorted_by_displacement[p_index];
}

int NoteTrack::get_note_index_by_displacement(double p_displacement) const {
    if (_notes_sorted_by_displacement.empty()) return -1;

    int left = 0;
    int right = _notes_sorted_by_displacement.size() - 1;
    int result = -1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (_notes_sorted_by_displacement[mid]->displacement <= p_displacement) {
            result = mid; left = mid + 1;
        }

        else right = mid - 1;
    }

    return result;
}

Ref<NoteKey> NoteTrack::get_note_by_displacement(double p_displacement) const {
    int index = get_note_index_by_displacement(p_displacement);

    if (index == -1) return Variant();

    return _notes_sorted_by_displacement[index];
}

void NoteTrack::add_bpm_event(Ref<BpmEvent> p_event) {
    bpm_events->add_event(p_event);
    speed_events->update_displacements(0);
    update_displacements_and_secs();
}

bool NoteTrack::remove_bpm_event(Ref<BpmEvent> p_event) {
    bool result = bpm_events->remove_event(p_event);
    speed_events->update_displacements(0);
    update_displacements_and_secs();

    return result;
}

void NoteTrack::remove_bpm_event_at(int p_index) {
    bpm_events->remove_event_at(p_index);
    speed_events->update_displacements(0);
    update_displacements_and_secs();
}

void NoteTrack::add_bpm(int p_units, double p_bpm) {
    bpm_events->add_bpm(p_units, p_bpm);
    speed_events->update_displacements(0);
    update_displacements_and_secs();
}

void NoteTrack::add_speed_event(Ref<SpeedEvent> p_event) {
    speed_events->add_event(p_event);
    update_displacements_and_secs();
}

bool NoteTrack::remove_speed_event(Ref<SpeedEvent> p_event) {
    bool result = speed_events->remove_event(p_event);
    update_displacements_and_secs();

    return result;
}

void NoteTrack::remove_speed_event_at(int p_index) {
    speed_events->remove_event_at(p_index);
    update_displacements_and_secs();
}

void NoteTrack::add_speed(int p_units_start, int p_units_end, double p_speed_start, double p_speed_end) {
    speed_events->add_speed(p_units_start, p_units_end, p_speed_start, p_speed_end);
    update_displacements_and_secs();
}

void NoteTrack::update_displacements_and_secs(int p_from, bool p_update_ori) {
    if (speed_events.is_null()) {
        UtilityFunctions::push_error("speed_events is null");
        return;
    }

    int size = _notes.size();

    if (p_update_ori) {
        for (int i = p_from; i < size; i++) {
            Ref<NoteKey> note = _notes[i];
            double secs = bpm_events->get_secs(note->units);

            note->secs = secs;
            note->displacement = speed_events->get_displacement(secs);
        }
    }

    if (sort_note_by_displacement)
        std::sort(
            _notes_sorted_by_displacement.begin(),
            _notes_sorted_by_displacement.end(),
            [](const Ref<NoteKey> &a, const Ref<NoteKey> &b) {
                return a->displacement < b->displacement;
            });
}