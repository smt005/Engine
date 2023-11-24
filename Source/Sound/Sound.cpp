
// https://habr.com/ru/articles/488744/

#include "Sound.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <bit>

namespace {
    std::int32_t convert_to_int(char* buffer, std::size_t len) {
        std::int32_t a = 0;
        std::memcpy(&a, buffer, len);
        return a;
    }

    bool load_wav_file_header(std::ifstream& file, std::uint8_t& channels, std::int32_t& sampleRate, std::uint8_t& bitsPerSample, ALsizei& size) {
        char buffer[4];
        if (!file.is_open())
            return false;

        // the RIFF
        if (!file.read(buffer, 4).good()) {
            std::cerr << "ERROR: could not read RIFF" << std::endl;
            return false;
        }
        if (std::strncmp(buffer, "RIFF", 4) != 0) {
            std::cerr << "ERROR: file is not a valid WAVE file (header doesn't begin with RIFF)" << std::endl;
            return false;
        }

        // the size of the file
        if (!file.read(buffer, 4).good()) {
            std::cerr << "ERROR: could not read size of file" << std::endl;
            return false;
        }

        // the WAVE
        if (!file.read(buffer, 4).good()) {
            std::cerr << "ERROR: could not read WAVE" << std::endl;
            return false;
        }
        if (std::strncmp(buffer, "WAVE", 4) != 0) {
            std::cerr << "ERROR: file is not a valid WAVE file (header doesn't contain WAVE)" << std::endl;
            return false;
        }

        // "fmt/0"
        if (!file.read(buffer, 4).good()) {
            std::cerr << "ERROR: could not read fmt/0" << std::endl;
            return false;
        }

        // this is always 16, the size of the fmt data chunk
        if (!file.read(buffer, 4).good()) {
            std::cerr << "ERROR: could not read the 16" << std::endl;
            return false;
        }

        // PCM should be 1?
        if (!file.read(buffer, 2).good()) {
            std::cerr << "ERROR: could not read PCM" << std::endl;
            return false;
        }

        // the number of channels
        if (!file.read(buffer, 2).good()) {
            std::cerr << "ERROR: could not read number of channels" << std::endl;
            return false;
        }
        channels = convert_to_int(buffer, 2);

        // sample rate
        if (!file.read(buffer, 4).good()) {
            std::cerr << "ERROR: could not read sample rate" << std::endl;
            return false;
        }
        sampleRate = convert_to_int(buffer, 4);

        // (sampleRate * bitsPerSample * channels) / 8
        if (!file.read(buffer, 4).good()) {
            std::cerr << "ERROR: could not read (sampleRate * bitsPerSample * channels) / 8" << std::endl;
            return false;
        }

        // ?? dafaq
        if (!file.read(buffer, 2).good()) {
            std::cerr << "ERROR: could not read dafaq" << std::endl;
            return false;
        }

        // bitsPerSample
        if (!file.read(buffer, 2).good()) {
            std::cerr << "ERROR: could not read bits per sample" << std::endl;
            return false;
        }
        bitsPerSample = convert_to_int(buffer, 2);

        // data chunk header "data"
        if (!file.read(buffer, 4).good()) {
            std::cerr << "ERROR: could not read data chunk header" << std::endl;
            return false;
        }
        if (std::strncmp(buffer, "data", 4) != 0) {
            std::cerr << "ERROR: file is not a valid WAVE file (doesn't have 'data' tag)" << std::endl;
            return false;
        }

        // size of data
        if (!file.read(buffer, 4).good()) {
            std::cerr << "ERROR: could not read data size" << std::endl;
            return false;
        }
        size = convert_to_int(buffer, 4);

        /* cannot be at the end of file */
        if (file.eof()) {
            std::cerr << "ERROR: reached EOF on the file" << std::endl;
            return false;
        }
        if (file.fail()) {
            std::cerr << "ERROR: fail state set on the file" << std::endl;
            return false;
        }

        return true;
    }

    char* load_wav(const std::string& filename, std::uint8_t& channels, std::int32_t& sampleRate, std::uint8_t& bitsPerSample, std::vector<char>& soundData) {
        ALsizei size = 0;
        std::ifstream in(filename, std::ios::binary);
        if (!in.is_open()) {
            std::cerr << "ERROR: Could not open \"" << filename << "\"" << std::endl;
            return nullptr;
        }
        if (!load_wav_file_header(in, channels, sampleRate, bitsPerSample, size)) {
            std::cerr << "ERROR: Could not load wav header of \"" << filename << "\"" << std::endl;
            return nullptr;
        }

        soundData.resize(size);

        in.read(soundData.data(), size);

        return soundData.data();
    }


    void PlayWav() {
        ALCdevice* openALDevice = alcOpenDevice(nullptr);
        if (!openALDevice)
            return;

        ALCcontext* openALContext = alcCreateContext(openALDevice, (ALCint*)nullptr);
        ALCboolean contextMadeCurrent = false;
        alcMakeContextCurrent(openALContext);


        std::uint8_t channels;
        std::int32_t sampleRate;
        std::uint8_t bitsPerSample;
        std::vector<char> soundData;

        std::string wavFile = "C:/Work/My/CommonProject/Executable/debug/stereo.wav";
        if (!load_wav(wavFile,
            channels,
            sampleRate,
            bitsPerSample,
            soundData)) {
            std::cerr << "ERROR: Could not load wav" << std::endl;
            return;
        }

        alGetError(); // clear error code 
        ALuint buffer;
        alGenBuffers(1, &buffer);
        if (alGetError() != AL_NO_ERROR) {
            std::cout << "ERRRR\n";
            return;
        }

        ALenum format;
        if (channels == 1 && bitsPerSample == 8)
            format = AL_FORMAT_MONO8;
        else if (channels == 1 && bitsPerSample == 16)
            format = AL_FORMAT_MONO16;
        else if (channels == 2 && bitsPerSample == 8)
            format = AL_FORMAT_STEREO8;
        else if (channels == 2 && bitsPerSample == 16)
            format = AL_FORMAT_STEREO16;
        else {
            std::cerr << "ERROR: unrecognised wave format: " << channels << " channels, " << bitsPerSample << " bps"
                << std::endl;
            return;
        }

        alGetError(); // clear error code 
        alBufferData(buffer, format, soundData.data(), soundData.size(), sampleRate);
        //soundData.clear(); // erase the sound in RAM
        if (alGetError() != AL_NO_ERROR) {
            std::cout << "ERRRR\n";
            return;
        }

        ALuint source;
        alGenSources(1, &source);
        alSourcef(source, AL_PITCH, 1);
        alSourcef(source, AL_GAIN, 1.0f);
        alSource3f(source, AL_POSITION, 0, 0, 0);
        alSource3f(source, AL_VELOCITY, 0, 0, 0);
        alSourcei(source, AL_LOOPING, AL_FALSE);
        alSourcei(source, AL_BUFFER, buffer);
        ALfloat maxGain = 1.0f;
        alSourcef(source, AL_GAIN, maxGain);
        alSourcePlay(source);

        ALint state = AL_PLAYING;

        while (state == AL_PLAYING && alGetError() == AL_NO_ERROR) {
            alGetSourcei(source, AL_SOURCE_STATE, &state);
        }

        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);

        alcCloseDevice(openALDevice);
    }
}

void Sound::Init() {
    PlayWav();
}

void Sound::Deinit() {
    //...
}
