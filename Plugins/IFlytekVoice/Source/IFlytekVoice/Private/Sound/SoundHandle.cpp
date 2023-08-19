#include "Sound/SoundHandle.h"
#include "IFlytekVoicePlatform.h"

namespace SoundHandle
{
	bool PlaySoundByFile(const FString& InFilePath)
	{
		FString FullPath = FPaths::ConvertRelativePathToFull(InFilePath);

#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
		/**
		 * Windows自带播放函数，只支持wav
		 * SND_APPLICATION 用应用程序指定的关联来播放声音。
		 * SND_ASYNC 异步播放，播放背景声。
		 * SND_LOOP 循环播放
		 * SND_NODEFAULT 警告音，则为失败
		 * SND_FILENAME //文件名
		 * SND_MEMORY //内存的声音
		 * SND_NOSTOP PlaySound不打断原来的声音播出并立即返回FALSE。
		 * SND_NOWAIT 如果驱动程序正忙则函数就不播放声音并立即返回。
		 * SND_PURGE 停止所有与调用任务有关的声音。若参数pszSound为NULL，就停止所有的声音，否则，停止pszSound指定的声音。
		 * SND_RESOURCE  pszSound参数是WAVE资源的标识符，这时要用到hmod参数。
		 */
		return PlaySound(*FullPath, NULL, SND_FILENAME | SND_ASYNC);
#endif
		return false;
	}

	bool PlaySoundByMemory(const uint8* SoundPtr)
	{
#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
		return PlaySound((LPCWSTR)SoundPtr, NULL, SND_MEMORY | SND_ASYNC);
#endif
		return false;
	}

	bool StopSound()
	{
#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
		return PlaySound(NULL, NULL, 0);
#endif
	}
}
