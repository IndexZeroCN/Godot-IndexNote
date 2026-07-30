#ifndef NOTE_TRACK_H
#define NOTE_TRACK_H

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <vector>

#include "note_key.h"
#include "bpm_event_list.h"
#include "speed_event_list.h"

namespace godot {

class NoteTrack : public RefCounted {
	GDCLASS(NoteTrack, RefCounted)

private:
    std::vector<Ref<NoteKey>> _notes;
    std::vector<Ref<NoteKey>> _notes_sorted_by_displacement;

protected:
	static void _bind_methods();

public:
    NoteTrack() = default;
    static Ref<NoteTrack> create(
        Ref<BpmEventList> p_bpm_events, Ref<SpeedEventList> p_speed_events,
        bool p_sort_note_by_displacement = false);

    Ref<BpmEventList> bpm_events;
    Ref<SpeedEventList> speed_events;
    bool sort_note_by_displacement;

    Ref<BpmEventList> get_bpm_events() const { return bpm_events; }
    Ref<SpeedEventList> get_speed_events() const { return speed_events; }

    void add_note(Ref<NoteKey> p_note);
    bool remove_note(Ref<NoteKey> p_note);
    void remove_note_at(int index);
    
    int get_note_index(double p_units, bool p_first = false) const;
    Ref<NoteKey> get_note(double p_units) const;
    Ref<NoteKey> get_note_at(int index) const;

    int get_note_index_by_displacement(double p_displacement) const;
    Ref<NoteKey> get_note_by_displacement(double p_displacement) const;
    Ref<NoteKey> get_note_by_displacement_at(int p_index) const;

    int size() const { return _notes.size(); }

    void add_bpm_event(Ref<BpmEvent> p_event);
    bool remove_bpm_event(Ref<BpmEvent> p_event);
    void remove_bpm_event_at(int p_index);
    void add_bpm(int p_units, double p_bpm);

    void add_speed_event(Ref<SpeedEvent> p_event);
    bool remove_speed_event(Ref<SpeedEvent> p_event);
    void remove_speed_event_at(int p_index);
    void add_speed(int p_units_start, int p_units_end, double p_speed_start, double p_speed_end);

    void update_displacements_and_secs(int from = 0);
        
};

} // namespace godot

#endif // NOTE_TRACK_H