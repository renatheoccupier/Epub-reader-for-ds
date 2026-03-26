#include "renderer.h"
#include "epub.h"
#include "settings.h"
#include "button.h"
#include "file_browser.h"
#include <sys/dir.h>
#include <fstream>
#include <stdio.h>

#include <nds.h>
#include <filesystem.h>
#include <fat.h>

namespace
{

const string kMenuTitle("Rena");
const string kMenuSubtitle("EPUB Reader");

void drawMenuTopScreen()
{
	const int width = screens::layoutX();
	const int titleWidth = renderer::strWidth(eUtf8, kMenuTitle, 0, 0, 24);
	const int subtitleWidth = renderer::strWidth(eUtf8, kMenuSubtitle, 0, 0, 12);
	const int titleX = MAX(10, (width - titleWidth) / 2);
	const int subtitleX = MAX(10, (width - subtitleWidth) / 2);

	renderer::printStr(eUtf8, top_scr, titleX, 34, kMenuTitle, 0, 0, 24);
	renderer::fillRect(MAX(10, titleX - 6), 42, MIN(width - 10, titleX + titleWidth + 6), 44, Blend(112), top_scr);
	renderer::printStr(eUtf8, top_scr, subtitleX, 64, kMenuSubtitle, 0, 0, 12);
}

} // namespace


bool file_ok(const string& file_name)
{ return std::ifstream(file_name.c_str()).good(); }

static bool book_ok(const string& file_name)
{ return !file_name.empty() && file_ok(file_name) && extention(file_name) == "epub"; }

static grid menu(0);

void applyBrightness()
{
	static const int levels[] = {-12, -8, -4, 0};
	int level = settings::brightness;
	clamp(level, 0, 3);
	setBrightness(0, levels[level]);
	setBrightness(1, levels[level]);
}

void drawMenu()
{
	menu = grid(0);
	if(book_ok(settings::recent_book)) menu.push(SAY(resume), 1);
	menu.push(SAY(files), 2);
	menu.push(SAY(light), 3);
	renderer::clearScreens(settings::bgCol);
	menu.draw();
	drawMenuTopScreen();
	setBacklightMode(blOverlay);
}

void openBook(const string& file)
{
	string ext = extention(file);
	if ("epub" != ext) bsod("main:Unsupported format.\n\nOnly EPUB files are supported.");
	epub_book(file).read();
}

string browseForBook()
{
	file_browser browser;
	return browser.run();
}

void browseLoop()
{
	while(pumpPowerManagement()) {
		const string file = browseForBook();
		if(file.empty()) {
			if(appShouldExit()) return;
			drawMenu();
			return;
		}
		openBook(file);
		if(appShouldExit()) return;
	}
}

int main(int argc, char *argv[])
{
	powerOff(PM_SOUND_AMP | PM_SOUND_MUTE);	//save battery life
	renderer::initVideo();

	string binname = "iku", argfile;
	if(argc) {
		binname = argv[0];
		if(binname.length() > 4 && !binname.compare(binname.length() - 4, 4, ".nds")) binname.erase(binname.length() - 4);
		u32 found = binname.find_last_of('/');
		if(found != string::npos) binname.erase(0, found + 1);
		if(argc >= 2) argfile = argv[1];
	}
	settings::binname = binname;

	iprintf("loading file system... ");
	if (!fatInitDefault()) bsod("main:error\n\ntried DLDI patch?");
	consoleClear();
	DIR* dir = opendir("/data/ikureader/");
	if(!dir) bsod("main: \nFolder data/ikureader not found.\nCopy it to the root of your\nflash card from the installation package.");
	closedir(dir);
	settings::load();
	applyBrightness();
	initPowerManagement();
	iprintf("loading fonts... ");
	renderer::initFonts();
	consoleClear();
	string trans = transPath + settings::translname;
	if(file_ok(trans)) loadTrans(trans);
	consoleClear();
	
	if(book_ok(argfile)) openBook(argfile);
	if(appShouldExit()) return 0;
	if(!book_ok(settings::recent_book)) browseLoop();
	if(appShouldExit()) return 0;
	
	drawMenu();
	while(pumpPowerManagement()) {
		swiWaitForVBlank();
		scanKeys();
		if(!(keysDown() & KEY_TOUCH)) continue;
		const string* t = menu.update();
		if(SAY(files) == t) browseLoop();
		else if(SAY(light) == t) cycleBacklight();
		else if(SAY(resume) == t && book_ok(settings::recent_book)) {
			openBook(settings::recent_book);
			drawMenu();
		}
	}
	return 0;
}

void bsod(const char* msg)
{
	renderer::clearScreens(0);
	iprintf(msg);
	while(1) swiWaitForVBlank();
}

void cycleBacklight()
{
	int& b = settings::brightness;
	b++;
	b %= 4;
	applyBrightness();
}
