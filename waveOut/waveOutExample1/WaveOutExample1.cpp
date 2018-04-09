#include <iostream>
#include <windows.h>
#include <mmsystem.h>

using namespace std;

int main(int argc, char*argv[])
{
	HWAVEOUT waveOuthandle;       // device handle
	WAVEFORMATEX wfx;             // structure that describes the format of the audio
	int result;

	wfx.nSamplesPerSec = 44100;   // sample rate
	wfx.wBitsPerSample = 16;      // sample size ;)
	wfx.nChannels = 2;            // channels

	/*
	* WAVEFORMATEX also has other fields which need filling.
	* as long as the three fields above are filled this should
	* work for any PCM (pulse code modulation) format.
	*/
	wfx.cbSize = 0; /* size of _extra_ info */
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

	result = waveOutOpen(&waveOuthandle, 0x0000, &wfx, 0, 0, CALLBACK_NULL);

	if (result != MMSYSERR_NOERROR)
	{
		if (result == WAVERR_BADFORMAT)
		{
			printf("%s\r\n", "Attempted to open with an unsupported waveform-audio format.");
		}
		else if (result == MMSYSERR_NOMEM)
		{
			printf("%s\r\n", "Unable to allocate or lock memory.");
		}
		else if (result == MMSYSERR_NODRIVER)
		{
			printf("%s\r\n", "No device driver is present.");
		}
		else if (result == MMSYSERR_BADDEVICEID)
		{
			printf("%s\r\n", "Specified device identifier is out of range.");
		}
		else if (result == MMSYSERR_ALLOCATED)
		{
			printf("%s\r\n", "Specified resource is already allocated");
		}
		else if (result == WAVERR_SYNC)
		{
			printf("%s\r\n", "The device is synchronous but waveOutOpen was called without using the WAVE_ALLOWSYNC flag.");
		}
		ExitProcess(1);
	}

	printf("%s\r\n", "The wave device was opened successfully");
	waveOutClose(waveOuthandle);
	return 0;
}