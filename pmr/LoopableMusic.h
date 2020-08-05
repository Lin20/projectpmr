#include <limits>
#include <exception>
#include <sstream>
#include <iostream>

#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

class LoopableMusic : public sf::Music
{
public:
	LoopableMusic(float volume, bool loop, size_t sample_start, size_t sample_end) : current_sample(0)
	{
		loop_start = sample_start;
		loop_end = sample_end;

		this->setVolume(volume);
		this->setLoop(loop);
		this->setRelativeToListener(true);
	}

	~LoopableMusic()
	{
	}

	void play()
	{
		if (this->getStatus() == sf::SoundSource::Status::Paused)
		{
			sf::Music::play();
		}
		else if (this->getStatus() == sf::SoundSource::Status::Stopped)
		{
			current_sample = 0;

			this->setPlayingOffset(sf::seconds(0.0f));

			sf::Music::play();
		}
	}


	virtual bool onGetData(sf::Music::Chunk& data)
	{
		if (sf::Music::onGetData(data))
		{
			std::size_t next = current_sample + data.sampleCount;

			if (next <= loop_end)
			{
				current_sample = next;
				return true;
			}
			else
			{
				data.sampleCount = loop_end - current_sample;
				current_sample = loop_start;
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	void onSeek(sf::Time timeOffset)
	{
		if (!played_intro)
		{
			sf::Music::onSeek(timeOffset);
			played_intro = true;
		}
		else
		{
			current_sample = loop_start + LoopableMusic::TimeToSample(timeOffset);
			sf::Time time = LoopableMusic::SampleToTime(current_sample);
			sf::Music::onSeek(time);
		}
	}

	std::size_t TimeToSample(sf::Time time)
	{
		return static_cast<std::size_t>((time.asSeconds() * this->getSampleRate() * this->getChannelCount()) + 0.5f);
	}

	sf::Time SampleToTime(std::size_t nSample)
	{
		return sf::seconds(static_cast<float>(nSample) / (this->getSampleRate() * this->getChannelCount()));
	}

private:
	std::size_t loop_start;
	std::size_t loop_end;
	std::size_t current_sample;
	bool played_intro;
};