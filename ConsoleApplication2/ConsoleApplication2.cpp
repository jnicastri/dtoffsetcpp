#include "stdafx.h"
#include <chrono>
#include <ctime>
//#include <iostream>
//#include <iomanip>
#include <comdef.h> // _bstr_t

// http://www.cplusplus.com/reference/chrono/system_clock/from_time_t/
// https://en.cppreference.com/w/cpp/chrono/c/wcsftime
// https://en.cppreference.com/w/cpp/chrono/time_point
// https://en.cppreference.com/w/cpp/chrono/c/gmtime



using namespace System;
using namespace std;
using namespace std::chrono;

static system_clock::time_point getTimePointFromParams(__int32 y, __int32 m, __int32 d, __int32 h, __int32 mn, __int32 s, __int32 ms);

int main(cli::array<System::String ^> ^args)
{
	DateTimeOffset dt = DateTimeOffset::Now;

	DateTimeOffset dtEpoch = DateTimeOffset((long long)0, TimeSpan(0));

	// Get 'Now' as a chrono time point - it'll always be local
	system_clock::time_point timepointNowLocal = system_clock::now();
	//std::time_t now_tomorrow = std::chrono::system_clock::to_time_t(timepointNow + std::chrono::hours(22));
	
	// Convert it to a time_t
	std::time_t now_c = std::chrono::system_clock::to_time_t(timepointNowLocal);

	// Get a chrono time point of the UTC time from the local chrono Now time
	system_clock::time_point timePointUtc2 = system_clock::from_time_t(mktime(gmtime(&now_c)));

	// buffers to store the formatted string versions of each time
	wchar_t localstr[30];
	wchar_t utcstr[30];

	// Write to the buffers a formatted string (note the use of gmtime (UTC) and localtime (local)
	wcsftime(localstr, 30, L"%F %T", localtime(&now_c));
	wcsftime(utcstr, 30, L"%F %T", gmtime(&now_c));

	// Optionally wrap them as managed strings
	//_bstr_t localBstrString(localstr);
	//_bstr_t utcTimeString(utcstr);

	// Printing the strings
	Console::WriteLine("time_point - utc - " + gcnew String(utcstr) + ".");
	Console::WriteLine("time_point - local - " + gcnew String(localstr) + ".");

	Console::WriteLine("system_clock now().count() (UTC) = " + timePointUtc2.time_since_epoch().count());
	Console::WriteLine("system_clock now().count() (Local) = " + timepointNowLocal.time_since_epoch().count());

	hours diff = duration_cast<hours>(timepointNowLocal - timePointUtc2);
	Console::WriteLine("Diff in hours 1: " + diff.count());

	system_clock::time_point constrTime = getTimePointFromParams(1982, 6, 30, 13, 10, 0, 0);
	std::time_t constime_c = std::chrono::system_clock::to_time_t(constrTime);
	wchar_t localconststr[30];
	wcsftime(localconststr, 30, L"%F %T", localtime(&constime_c));

	Console::WriteLine("time_point - custom local - " + gcnew String(localconststr) + ".");

	Console::WriteLine(".NET DateTimeOffset Now.ToString() = " + dt.ToString());
	Console::WriteLine(".NET DateTimeOffset Now.ToUniversalTime.ToString() = " + dt.ToUniversalTime().ToString());
	Console::WriteLine();
	Console::WriteLine(".NET DateTimeOffset Now.Offset.ToString() = " + dt.Offset.ToString());
	Console::WriteLine(".NET DateTimeOffset Now.Offset.Ticks = " + dt.Offset.Ticks.ToString());
	Console::WriteLine(".NET DateTimeOffset Now.Ticks " + dt.Ticks.ToString());
	Console::WriteLine();

	Console::WriteLine(".NET DateTimeOffset Computed difference in ticks = " + (dt.LocalDateTime - dt.ToUniversalTime()).Ticks);

	Console::WriteLine();




	Console::ReadKey();

    return 0;
}


system_clock::time_point getTimePointFromParams(__int32 y, __int32 m, __int32 d, __int32 h, __int32  mn, __int32 s, __int32 ms)
{
	std::tm timeinfo = std::tm();
	timeinfo.tm_year = y - 1900;
	timeinfo.tm_mon = m - 1; //0 indexed
	timeinfo.tm_mday = d;
	timeinfo.tm_hour = h;
	timeinfo.tm_min = mn;
	timeinfo.tm_sec = s;
	
	std::time_t tt = std::mktime(&timeinfo);

	return system_clock::from_time_t(tt);
}
