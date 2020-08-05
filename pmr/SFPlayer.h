#pragma once

#include <SFML/Audio.hpp>
#include <vector>

#include "gme/Music_Emu.h"

class SFPlayer : protected sf::SoundStream
{
public:
	SFPlayer();

	~SFPlayer();

	blargg_err_t Initialize(const char* filename, bool allow_overlapping = false, long samples = 44100);

	blargg_err_t Play(int track, bool fadeout = false);
	void Queue(int track, int delay);
	void SetVolume(float value) { setVolume(value); }
	void Update();
	void Close();

	bool TrackEnded() const { if (emulator) return emulator->track_ended(); return false; }

private:

	virtual bool onGetData(Chunk& data);

	virtual void onSeek(sf::Time timeOffset);

	const int FILL_RATE = 45;
	long sample_rate;
	int buffer_size;

	int current_track;
	bool switch_track;
	bool overlapping;

	Music_Emu* emulator;
	track_info_t track_info;
	sf::Int16* samples;

	std::vector<unsigned int> queues;
};
