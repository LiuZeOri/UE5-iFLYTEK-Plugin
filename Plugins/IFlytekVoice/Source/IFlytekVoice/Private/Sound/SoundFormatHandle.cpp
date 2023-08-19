#include "Sound/SoundFormatHandle.h"
#include "IFlytekVoiceType.h"
#include "IFlytekVoicePlatform.h"

namespace SoundFormatHandle
{
	int32 PCMToWAVByMemory(int32 InChannels, int32 InSampleRate, const TArray<uint8>& InPCMData, TArray<uint8>& OutWAVData)
	{
		FWaveHeard HeardDescribe;
		FWaveFmt FmtDescribe;
		FWaveData  DataDescribe;

		// Heard
		FMemory::Memcpy(HeardDescribe.ChunkID, "RIFF", strlen("RIFF"));
		FMemory::Memcpy(HeardDescribe.Format, "WAVE", strlen("WAVE"));
		HeardDescribe.ChunkSize = 36 + InPCMData.Num() * sizeof(uint8);

		// fmt
		FMemory::Memcpy(FmtDescribe.SubChunkID, "fmt ", strlen("fmt "));
		FmtDescribe.SubChunkSize = 16;
		FmtDescribe.AudioFormat = WAVE_FORMAT_PCM;
		FmtDescribe.NumChannel = InChannels;
		FmtDescribe.SampleRate = InSampleRate;
		FmtDescribe.BitsForSample = 16;
		FmtDescribe.ByteRate = FmtDescribe.SampleRate * FmtDescribe.NumChannel * FmtDescribe.BitsForSample / 8;
		FmtDescribe.BlockAlign = FmtDescribe.NumChannel * FmtDescribe.BitsForSample / 8;
	
		// Data
		FMemory::Memcpy(DataDescribe.DataChunkID, "data", strlen("data"));
		DataDescribe.DataChunkSize = InPCMData.Num() * sizeof(uint8);

		// 组合WAV格式数据
		// heard
		OutWAVData.AddUninitialized(sizeof(FWaveHeard));
		FMemory::Memcpy(OutWAVData.GetData(), &HeardDescribe, sizeof(FWaveHeard));

		// fmt
		int32 Index = OutWAVData.AddUninitialized(sizeof(FWaveFmt));
		FMemory::Memcpy(&OutWAVData[Index], &FmtDescribe, sizeof(FWaveFmt));

		// Data
		Index = OutWAVData.AddUninitialized(sizeof(FWaveData));
		FMemory::Memcpy(&OutWAVData[Index], &DataDescribe, sizeof(FWaveData));

		// 合并PCM
		OutWAVData.Append(InPCMData);

		return 0;
	}
}
