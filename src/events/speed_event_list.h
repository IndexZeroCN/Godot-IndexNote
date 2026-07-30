#ifndef SPEED_EVENT_LIST_H
#define SPEED_EVENT_LIST_H

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <vector>

#include "speed_event.h"
#include "bpm_event_list.h"

namespace godot {

class SpeedEventList : public RefCounted {
    GDCLASS(SpeedEventList, RefCounted)

private:
    double _init_speed;
    std::vector<Ref<SpeedEvent>> _events;
protected:
    static void _bind_methods();

public:
    SpeedEventList() = default;
    static Ref<SpeedEventList> create(
        double p_init_speed, Ref<BpmEventList> p_bpm_event_list = Variant(), bool p_allow_negative_speed = false);

    Ref<BpmEventList> bpm_events;
    bool allow_negative_speed;

    void add_event(Ref<SpeedEvent> p_event);
    bool remove_event(Ref<SpeedEvent> p_event);
    void remove_event_at(int p_index);
    int get_event_index(double p_units) const;
    Ref<SpeedEvent> get_event(double p_units) const;
    Ref<SpeedEvent> get_event_at(int p_index) const;
    int size() const { return _events.size(); }

    void add_speed(int p_units_start, int p_units_end, double p_speed_start, double p_speed_end);
    double get_speed(double p_secs) const;

    double get_displacement(double p_secs) const;
    double get_secs_by_displacement(double p_displacement) const;
    
    void update_displacements(int from);
};

} // namespace godot

#endif // SPEED_EVENT_LIST_H