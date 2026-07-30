#ifndef NOTE_KEY_H
#define NOTE_KEY_H

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/dictionary.hpp>

namespace godot {

class NoteKey : public RefCounted {
	GDCLASS(NoteKey, RefCounted)

protected:
	static void _bind_methods();

public:
    NoteKey() = default;
    static Ref<NoteKey> create(int p_units);

    int units;
    Dictionary properties;
    
    double secs;
    double displacement;

	int get_units() const { return units; }
	double get_displacement() const { return displacement; }
	double get_secs() const { return secs; }

	Dictionary get_properties() const { return properties; }
	void set_properties(const Dictionary &p_properties) { properties = p_properties; }
};

} // namespace godot

#endif // NOTE_KEY_H