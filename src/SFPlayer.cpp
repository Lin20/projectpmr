#pragma once

#include "SFPlayer.h"
#include "gme/blargg_source.h"


SFPlayer::SFPlayer()
{
	samples = 0;
	emulator = 0;
	current_track = 0;
	switch_track = false;
}

SFPlayer::~SFPlayer()
{
	if (emulator)
		delete emulator;
	if (samples)
		delete samples;
}

blargg_err_t SFPlayer::Initialize(const char* filename, bool allow_overlapping, long samples)
{
	sample_rate = 44100;
	overlapping = allow_overlapping;

	RETURN_ERR(gme_open_file(filename, &emulator, sample_rate));

	sf::SoundStream::stop();
	int min_size = sample_rate * 2 / FILL_RATE;
	buffer_size = 512;
	while (buffer_size < min_size)
		buffer_size *= 2;
	this->samples = new sf::Int16[samples];

	initialize(2, sample_rate);

	return 0;
}

blargg_err_t SFPlayer::Play(int track, bool fadeout)
{
	if ((current_track == track && !overlapping) || !emulator)
		return 0;
	if (fadeout)
	{
		switch_track = true;
		current_track = track;
		emulator->set_fade(emulator->tell() + 200, 600);
		return 0;
	}
	current_track = track;
	if (track == 0)
	{
		stop();
		return 0;
	}

	if (emulator)
	{
		// Sound must not be running when operating on emulator
		sf::SoundStream::stop();
		RETURN_ERR(emulator->start_track(track));

		// Calculate track length
		if (!emulator->track_info(&track_info))
		{
			if (track_info.length <= 0)
				track_info.length = track_info.intro_length +
				track_info.loop_length * 2;
		}
		if (track_info.length <= 0)
			track_info.length = (long)(2.5 * 60 * 1000);
		emulator->set_fade(track_info.length);

		play();
		//paused = false;
		//sound_start();
	}
	return 0;
}

void SFPlayer::Queue(int track, int delay)
{
	queues.push_back(((track & 0xFF) << 16) | (delay & 0xFFFF));
}

void SFPlayer::Update()
{
	if (switch_track && TrackEnded())
	{
		bool temp = overlapping;
		overlapping = true;
		Play(current_track);
		overlapping = temp;
		switch_track = false;
	}
	for (unsigned int i = 0; i < queues.size(); i++)
	{
		int delay = queues[i] & 0xffff;
		if (delay)
			delay--;
		if (!delay)
		{
			Play((queues[i] >> 16) & 0xff);
			queues.erase(queues.begin() + i--);
		}
		else
			queues[i] = (((queues[i] >> 16) & 0xff) << 16) | (delay & 0xffff);
	}
}

void SFPlayer::Close()
{
	stop();
	if (emulator)
		delete emulator;
	if (samples)
		delete samples;
	emulator = 0;
	samples = 0;
}

bool SFPlayer::onGetData(Chunk& data)
{
	if (emulator)
	{
		data.sampleCount = buffer_size;
		emulator->play(buffer_size, (short*)samples);
		data.samples = samples;
		return true;
	}

	return false;
}

void SFPlayer::onSeek(sf::Time timeOffset)
{

}