#include <Windows.h>
#include <string>

#include "auth.hpp"
#include "skStr.h"
#include "lazy.h"
std::string tm_to_readable_time(tm ctx);
static std::time_t string_to_timet(std::string timestamp);
static std::tm timet_to_tm(time_t timestamp);

using namespace KeyAuth;

std::string name = skCrypt("").decrypt(); // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = skCrypt("").decrypt(); // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = skCrypt("").decrypt(); // app secret, the blurred text on licenses tab and other tabs
std::string version = skCrypt("1.0").decrypt(); // leave alone unless you've changed version on website
std::string url = skCrypt("https://keyauth.win/api/1.2/").decrypt(); // change if you're self-hosting

/*
	Video on what ownerid and secret are https://youtu.be/uJ0Umy_C6Fg

	Video on how to add KeyAuth to your own application https://youtu.be/GB4XW_TsHqA

	Video to use Web Loader (control loader from customer panel) https://youtu.be/9-qgmsUUCK4
*/

api KeyAuthApp(name, ownerid, secret, version, url);

int main()
{
	LI_FN(SetConsoleTitleA).get()(skCrypt("Loader"));
	LI_FN(printf).get()(skCrypt("\n\n Connecting.."));
	KeyAuthApp.init();
	if (!KeyAuthApp.data.success)
	{
		LI_FN(printf).get()(skCrypt("\n Status: %s"), KeyAuthApp.data.message.c_str());
		LI_FN(Sleep).get()(1500);
		LI_FN(abort).get()();
	}

	/*
		Optional - check if HWID or IP blacklisted

	if (KeyAuthApp.checkblack()) {
		LI_FN(abort).get()();
	}
	*/

	LI_FN(printf).get()(skCrypt("\n\n App data:"));
	LI_FN(printf).get()(skCrypt("\n Number of users: %s"), KeyAuthApp.data.numUsers.c_str());
	LI_FN(printf).get()(skCrypt("\n Number of online users: %s"), KeyAuthApp.data.numOnlineUsers.c_str());
	LI_FN(printf).get()(skCrypt("\n Number of keys: %s"), KeyAuthApp.data.numKeys.c_str());
	LI_FN(printf).get()(skCrypt("\n Application Version: %s"), KeyAuthApp.data.version.c_str());
	LI_FN(printf).get()(skCrypt("\n Customer panel link: %s"), KeyAuthApp.data.customerPanelLink.c_str());
	LI_FN(printf).get()(skCrypt("\n Checking session validation status (remove this if causing your loader to be slow)"));
	KeyAuthApp.check();
	LI_FN(printf).get()(skCrypt("\n Current Session Validation Status: %s"), KeyAuthApp.data.message.c_str());

	LI_FN(printf).get()(skCrypt("\n\n [1] Login\n [2] Register\n [3] Upgrade\n [4] License key only\n\n Choose option: "));
	
	int option;
	std::string username;
	std::string password;
	std::string key;
	
	std::cin >> option;
	switch (option)
	{
	case 1:
		LI_FN(printf).get()(skCrypt("\n\n Enter username: "));
		std::cin >> username;
		LI_FN(printf).get()(skCrypt("\n Enter password: "));
		std::cin >> password;
		KeyAuthApp.login(username, password);
		break;
	case 2:
		LI_FN(printf).get()(skCrypt("\n\n Enter username: "));
		std::cin >> username;
		LI_FN(printf).get()(skCrypt("\n Enter password: "));
		std::cin >> password;
		LI_FN(printf).get()(skCrypt("\n Enter license: "));
		std::cin >> key;
		KeyAuthApp.regstr(username, password, key);
		break;
	case 3:
		LI_FN(printf).get()(skCrypt("\n\n Enter username: "));
		std::cin >> username;
		LI_FN(printf).get()(skCrypt("\n Enter license: "));
		std::cin >> key;
		KeyAuthApp.upgrade(username, key);
		break;
	case 4:
		LI_FN(printf).get()(skCrypt("\n Enter license: "));
		std::cin >> key;
		KeyAuthApp.license(key);
		break;
	default:
		LI_FN(printf).get()(skCrypt("\n\n Status: Failure: Invalid Selection"));
		LI_FN(Sleep).get()(3000);
		LI_FN(abort).get()();
	}
	
	if (!KeyAuthApp.data.success)
	{
		LI_FN(printf).get()(skCrypt("\n Status: %s"), KeyAuthApp.data.message.c_str());
		LI_FN(Sleep).get()(1500);
		LI_FN(abort).get()();
	}
	
	LI_FN(printf).get()(skCrypt("\n User data:"));
	LI_FN(printf).get()(skCrypt("\n Username: %s"), KeyAuthApp.data.username.c_str());
	LI_FN(printf).get()(skCrypt("\n IP address: %s"), KeyAuthApp.data.ip.c_str());
	LI_FN(printf).get()(skCrypt("\n Hardware-Id: %s"), KeyAuthApp.data.hwid.c_str());
	LI_FN(printf).get()(skCrypt("\n Create date: %s"), tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.createdate.c_str()))).c_str());
	LI_FN(printf).get()(skCrypt("\n Last login: %s"), tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.lastlogin.c_str()))).c_str());
	LI_FN(printf).get()(skCrypt("\n Subscription name(s): "));
	std::string subs;
	for (std::string value : KeyAuthApp.data.subscriptions)subs += value +" ";
	LI_FN(printf).get()(subs.c_str());
	LI_FN(printf).get()(skCrypt("\n Subscription expiry: %s"), tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.expiry.c_str()))).c_str());
	LI_FN(printf).get()(skCrypt("\n Checking session validation status (remove this if causing your loader to be slow)"));
	KeyAuthApp.check();
	LI_FN(printf).get()(skCrypt("\n Current Session Validation Status: %s"), KeyAuthApp.data.message.c_str());

	LI_FN(printf).get()(skCrypt("\n\n Closing in ten seconds..."));
	LI_FN(Sleep).get()(10000);
	LI_FN(abort).get()();
}

std::string tm_to_readable_time(tm ctx) {
	char buffer[80];

	LI_FN(strftime).get()(buffer, sizeof(buffer), "%a %m/%d/%y %H:%M:%S %Z", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp) {
	auto cv = LI_FN(strtol).get()(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}
