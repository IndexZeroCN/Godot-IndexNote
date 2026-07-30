#ifndef SPEED_EVENT_H
#define SPEED_EVENT_H

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

namespace godot {

class SpeedEvent : public RefCounted {
	GDCLASS(SpeedEvent, RefCounted)

protected:
	static void _bind_methods();

public:
	SpeedEvent() = default;
	static Ref<SpeedEvent> create(int p_units_start, int p_units_end, double p_speed_start, double p_speed_end);

	int units_start;
	int units_end;
	double speed_start;
	double speed_end;
	double pre_displacement;

	int get_units_start() const { return units_start; }
	int get_units_end() const { return units_end; }
	double get_speed_start() const { return speed_start; }
	double get_speed_end() const { return speed_end; }

	double get_pre_displacement() const { return pre_displacement; }
	double get_displacement(double p_delta_secs) const;
	double get_displacement_at(double p_delta_secs, double p_secs) const;
};

} // namespace godot

#endif // SPEED_EVENT_H