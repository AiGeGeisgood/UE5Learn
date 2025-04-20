#pragma once

class XGThread : public FRunnable
{
public:
	XGThread();
	virtual ~XGThread() override;

public:
	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;


	void CreateSafeThread();
	void WaitAndCompleted();

protected:
	FThreadSafeCounter StopTaskCounter;
	FEvent* StartUpEvent;
	FRunnableThread* Thread;
};
