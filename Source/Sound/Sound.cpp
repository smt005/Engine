
#include "Sound.h"

#include <AL/al.h>
#include <AL/alc.h>

#include <string>
#include <iostream>

namespace {
	ALCdevice* device = nullptr;
}

void Sound::Init() {
	device = alcOpenDevice(NULL);
	if (!device) {
		std::cout << "Sound::Init ERROR!" << std::endl;
		return;
	}

	std::cout << "Sound::Init OK" << std::endl;
}

void Sound::Deinit() {
	if (device) {
		alcCloseDevice(device);
		device = nullptr;
	}
}
