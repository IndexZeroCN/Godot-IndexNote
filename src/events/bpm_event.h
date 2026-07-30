#ifndef BPM_EVENT_H
#define BPM_EVENT_H

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {

class BpmEvent : public RefCounted {
	GDCLASS(BpmEvent, RefCounted)

protected:
	static void _bind_methods();

public:
	BpmEvent() = default;
	static Ref<BpmEvent> create(int p_units, double p_bpm);

	int units;
	double bpm;
	double pre_secs;

	int get_units() const { return units; }
	double get_bpm() const { return bpm; }
	double get_secs_start() const { return pre_secs; }
};

} // namespace godot

#endif // BPM_EVENT_H