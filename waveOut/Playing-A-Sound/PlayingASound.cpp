#include <iostream>
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

using namespace std;

char* LoadAudioBlock(const char* filename, size_t* blockSize)
{
	FILE * fd;
	fopen_s(&fd, filename, "r");
	if (fd == nullptr)
	{
		printf("%s %s\r\n", "fopen() failed: ", filename);
		return nullptr;
	}
	fseek(fd, 0L, SEEK_END);
	*blockSize = ftell(fd);
	char * buf = new char[*blockSize];
	rewind(fd);
	fread(buf, *blockSize, 1, fd);
	fclose(fd);
	return buf;
}

void ReleaseBlock(char* buf)
{
	delete[] buf;
}

void WriteAudioBlock(HWAVEOUT waveOutHandle, const char* block, size_t size)
{
	WAVEHDR blockHeader;
	memset(&blockHeader, 0x00, sizeof WAVEHDR);
	blockHeader.dwBufferLength = size;
	blockHeader.lpData = (LPSTR)block;

	//prepare block for playback
	waveOutPrepareHeader(waveOutHandle, &blockHeader, sizeof blockHeader);
	waveOutWrite(waveOutHandle, &blockHeader, sizeof blockHeader);
	Sleep(500);
	while (waveOutUnprepareHeader(waveOutHandle, &blockHeader, sizeof blockHeader) != WAVERR_STILLPLAYING)
	{
		Sleep(100);
	}
}

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
		printf("%s\r\n", "waveOutOpen() failed");
		ExitProcess(1);
	}

	printf("%s\r\n", "The wave device was opened successfully");

	size_t blockSize;
	const char* block = LoadAudioBlock("sound.wav", &blockSize);
	if (block == nullptr)
	{
		printf("%s\r\n", "LoadAudioBlock() failed");
		ExitProcess(1);
	}
	WriteAudioBlock(waveOuthandle, block, blockSize);
	waveOutClose(waveOuthandle);
	return 0;
}