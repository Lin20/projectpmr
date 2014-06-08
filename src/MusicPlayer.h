// Simple game music file player

// Game_Music_Emu 0.5.2
#pragma once

#include "gme/Music_Emu.h"

class MusicPlayer
{
public:
	// Initialize player and set sample rate
	blargg_err_t Initialize(long sample_rate = 44100);

	// Load game music file. NULL on success, otherwise error string.
	blargg_err_t LoadFile(const char* path);

	// (Re)start playing track. Tracks are numbered from 0 to track_count() - 1.
	blargg_err_t Play(int track, bool fadeout = false);

	// Stop playing current file
	void Stop();
	
	//used to process fades and switch tracks automatically
	void Update();

	// Optional functions

	// Number of tracks in current file, or 0 if no file loaded.
	int TrackCount() const;

	// Info for current track
	track_info_t const& track_info() const { return track_info_; }

	// Pause/resume playing current track.
	void Pause(int);

	// True if track ended
	bool TrackEnded() const;

	// Pointer to emulator
	Music_Emu* emu() const { return emu_; }

	// Set stereo depth, where 0.0 = none and 1.0 = maximum
	void SetStereoDepth(double);

	// Set tempo, where 0.5 = half speed, 1.0 = normal, 2.0 = double speed
	void SetTempo(double);

	// Set voice muting bitmask
	void MuteVoices(int);

	// Set buffer to copy samples from each buffer into, or NULL to disable
	typedef short sample_t;
	void SetScopeBuffer(sample_t* buf, int size) { scope_buf = buf; scope_buf_size = size; }

	MusicPlayer();
	~MusicPlayer();

private:
	Music_Emu* emu_;
	sample_t* scope_buf;
	long sample_rate;
	int scope_buf_size;
	bool paused;
	track_info_t track_info_;
	int current_track;
	int switch_track;

	void Suspend();
	void Resume();
	static void FillBuffer(void*, sample_t*, int);
};