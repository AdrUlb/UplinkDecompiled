#pragma once

namespace IRCInterface
{
	static void CloseConnection()
	{
		static auto called = false;
		if (!called)
		{
			puts("TODO: implement IRCInterface::CloseConnection()");
			called = true;
		}
	}

	static void UpdateMessages()
	{
		static auto called = false;
		if (!called)
		{
			puts("TODO: implement IRCInterface::UpdateMessages()");
			called = true;
		}
	}
} // namespace IRCInterface
