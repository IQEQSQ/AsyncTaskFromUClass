#pragma once


class FAsyncTaskFromUClassModule : public IModuleInterface
{
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};

IMPLEMENT_MODULE(FAsyncTaskFromUClassModule, AsyncTaskFromUClass);