#pragma once


class FK2NodeAsyncTaskModule : public IModuleInterface
{
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};

IMPLEMENT_MODULE(FK2NodeAsyncTaskModule, K2NodeAsyncTask);