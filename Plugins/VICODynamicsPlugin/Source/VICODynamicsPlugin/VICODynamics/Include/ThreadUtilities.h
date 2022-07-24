// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "VICODynamics.h"

#if defined(_MSC_VER) && !defined(__NX__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif
#define NOMINMAX
#include <windows.h>
#ifdef _DEBUG
#define STR(str) L#str
#endif
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__NX__)  || defined(__arm__)
#include <pthread.h>
#include <sys/types.h>
#if !defined(__ORBIS__) && !defined(__NX__) &&  !defined(__arm__)
#include <sys/sysctl.h>
#endif
#if defined(__NX__)
#include <nn/os/os_ThreadApi.h>
#endif
#ifdef _DEBUG
#include <stdio.h>
#endif
#endif

#if defined(_MSC_VER) && !defined(__NX__)
#define GET_THREAD_ID() GetCurrentThreadId()
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__arm__)
#define GET_THREAD_ID() size_t(pthread_self())
#elif defined(__NX__)
#define GET_THREAD_ID() (size_t)nn::os::GetCurrentThread()->_handle
#endif

namespace vicoDynamics
{
	namespace threading
	{
		namespace platform
		{
			struct FMutex
			{
				FMutex()
				{
#if defined(_MSC_VER) && !defined(__NX__)
					InitializeCriticalSection(&Locker);
					Owner = 0;
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__NX__) || defined(__arm__)
					pthread_mutexattr_t attr;
					pthread_mutexattr_init(&attr);
					pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
					pthread_mutex_init(&Locker, &attr);
					pthread_mutexattr_destroy(&attr);
#endif
				}

				~FMutex()
				{
#if defined(_MSC_VER) && !defined(__NX__)
					DeleteCriticalSection(&Locker);
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__NX__) || defined(__arm__)
					pthread_mutex_destroy(&Locker);
#endif
				}

				void Lock()
				{
#if defined(_MSC_VER) && !defined(__NX__)
					EnterCriticalSection(&Locker);

#ifdef _DEBUG
					Owner = GET_THREAD_ID();
#endif
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__NX__) || defined(__arm__)
					pthread_mutex_lock(&Locker);
#ifdef _DEBUG
					Owner = GET_THREAD_ID();
#endif
#endif
				}

				bool TryLock()
				{
#if defined(_MSC_VER) && !defined(__NX__)
					bool success = TryEnterCriticalSection(&Locker) != 0;
#ifdef _DEBUG
					if (success)
						Owner = GET_THREAD_ID();
#endif
					return success;
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__NX__) || defined(__arm__)
					bool success = !pthread_mutex_trylock(&Locker);
#ifdef _DEBUG
					if (success)
						Owner = GET_THREAD_ID();
#endif
					return success;
#endif
				}

				void Unlock()
				{
#if defined(_MSC_VER) && !defined(__NX__)
#ifdef _DEBUG
					if (Owner != GET_THREAD_ID())
					{
						OutputDebugString(STR("Attempting to unlock the Mutex from a thread that is not the thread which locked it!"));
						return;
					}
#endif
					LeaveCriticalSection(&Locker);
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__NX__) || defined(__arm__)
#ifdef _DEBUG
					if (Owner != GET_THREAD_ID())
					{
						printf("Attempting to unlock the Mutex from a thread that is not the thread which locked it!");
						return;
					}
#endif								
					pthread_mutex_unlock(&Locker);
#endif
				}

#if defined(_MSC_VER) && !defined(__NX__)
				CRITICAL_SECTION Locker;
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__NX__) || defined(__arm__)
				pthread_mutex_t Locker;
#endif
				size_t Owner;
			};
		}
#if defined(__NX__)
		// Easier to just proxy the built-in type
#include <nn/os/os_ReaderWriterLock.h>
		struct FReadWriteLock
		{
			FReadWriteLock()
			{}

			void LockReader()
			{
				nnReaderWriterLock.AcquireReadLock();
			}

			void LockWriter()
			{
				nnReaderWriterLock.AcquireWriteLock();
			}

			void UnlockReader()
			{
				nnReaderWriterLock.ReleaseReadLock();
			}

			void UnlockWriter()
			{
				nnReaderWriterLock.ReleaseWriteLock();
			}

		private:
			nn::os::ReaderWriterLock nnReaderWriterLock;
		};
#else
		struct FReadWriteLock
		{
			FReadWriteLock() : ReaderCounter(0)
			{}

			void LockReader()
			{
				Mutex.Lock();
#if defined(_MSC_VER) && !defined(__NX__)
				InterlockedIncrement(&ReaderCounter);
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__arm__)
				__sync_add_and_fetch(&ReaderCounter, 1);
#endif
				Mutex.Unlock();
			}

			void LockWriter()
			{
				Mutex.Lock();

				while (ReaderCounter); // Wait until readers are done
			}

			void UnlockReader()
			{
#if defined(_MSC_VER) && !defined(__NX__)
				InterlockedDecrement(&ReaderCounter);
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__arm__)
				__sync_sub_and_fetch(&ReaderCounter, 1);
#endif
			}

			void UnlockWriter()
			{
				Mutex.Unlock();
			}

			platform::FMutex Mutex;
#ifdef _MSC_VER
			volatile long ReaderCounter;
#elif defined(__APPLE__) || defined(__ORBIS__) || defined(__arm__)
			volatile int ReaderCounter;
#endif
		};
#endif
		struct FScopeLock
		{
			FScopeLock(platform::FMutex& InMutex) : Mutex(InMutex)
			{
				Mutex.Lock();
			}

			~FScopeLock()
			{
				Mutex.Unlock();
			}

		private:
			platform::FMutex& Mutex;
		};

		struct FScopeReaderLock
		{
			FScopeReaderLock(FReadWriteLock& InMutex) : Mutex(InMutex)
			{
				Mutex.LockReader();
			}

			~FScopeReaderLock()
			{
				Mutex.UnlockReader();
			}

		private:
			FReadWriteLock& Mutex;
		};

		struct FScopeWriterLock
		{
			FScopeWriterLock(FReadWriteLock& InMutex) : Mutex(InMutex)
			{
				Mutex.LockWriter();
			}

			~FScopeWriterLock()
			{
				Mutex.UnlockWriter();
			}

		private:
			FReadWriteLock& Mutex;
		};
	}
}