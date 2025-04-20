#pragma once

class FXGThread : public FRunnable
{
public:
	FXGThread();
	virtual ~FXGThread() override;

public:
	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;


	void CreateSafeThread();
	void WaitAndCompleted();

protected:
	FThreadSafeCounter StopTaskCounter;
	FRunnableThread* Thread;
	FEvent* StartUpEvent;

};

struct MyStruct
{
public:
	void SetA(int32 i)
	{
		FScopeLock ScopeLock(&Mutex);
		a = i;
	}

	int32 GetA()
	{
		FScopeLock ScopeLock(&Mutex);
		return a;
	}

private:
	int a = 1;
	FCriticalSection Mutex;
};
