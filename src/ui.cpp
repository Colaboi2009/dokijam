#include "ui.hpp"

#include <format>
#include "globals.hpp"
#include "engine/raw_text.hpp"

void showDeathScreen(TTF_Font *font) {
	RawText::Render("YOU DIED", {sdl.getWindowSize().x / 2.f, sdl.getWindowSize().y / 2.f - 20}, false, {255, 50, 50, 255}, font);
	RawText::Render("r to restart", {sdl.getWindowSize().x / 2.f, sdl.getWindowSize().y / 2.f + 30}, false, {200, 255, 200, 255}, font, 0.25f);
	RawText::Render("w for what", {sdl.getWindowSize().x / 2.f, sdl.getWindowSize().y / 2.f + 80}, false, {200, 255, 200, 255}, font, 0.25f);
}

void showQuestionScreen(TTF_Font *font) {
	float y = sdl.getWindowSize().y / 2.f - 100;
	RawText::Render("lmao", {sdl.getWindowSize().x / 2.f, y}, false, {50, 255, 50, 255}, font);
	y += 80;
	RawText::Render("understable you are here, this game questionable", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 20;
	RawText::Render("this game was made in sdl, with a lot of hiccups, and mostly was a trial experience, soooo, ofcourse its in this state", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 20;
	RawText::Render("anyways, to win: attack each crystal line before the sheild of the crystal forms", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 20;
	RawText::Render("you must do this with the right dragoon type", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 20;
	RawText::Render("once you do it for the 4 at the top, the final lightning river will enable", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 50;
	RawText::Render("w to return", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
}

void showWonScreen(TTF_Font *font, float timeTaken) {
	float y = sdl.getWindowSize().y / 2.f - 300;
	RawText::Render("YOU WON!", {sdl.getWindowSize().x / 2.f, y}, false, {50, 255, 50, 255}, font);
	y += 80;
	RawText::Render("woooo", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 80;
	std::string timeText = std::format("btw you took {}; ", timeTaken);
	if (timeTaken > 120) {
		timeText += "damn your slow";
	} else if (timeTaken < 10) {
		timeText += "you found the easter egg finish :D. Good job!";
	} else if (timeTaken < 30) {
		timeText += "damnnn thats really fast";
	} else if (timeTaken < 60) {
		timeText += "thats pretty quick";
	} else if (timeTaken < 120) {
		timeText += "pretty average";
	}
	RawText::Render(timeText, {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 80;
	RawText::Render("Credits: ", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.3f);
	y += 20;
	RawText::Render("Game planner: Alphawolfplasma", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 20;
	RawText::Render("Developers: Colaboi, Skulaurun", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 20;
	RawText::Render("Artist: Blastosis", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 80;
	RawText::Render("most questionable things are Colaboi's (this writer's) fault", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.25f);
	y += 80;
	RawText::Render("btw this game uses a recursive main lmao", {sdl.getWindowSize().x / 2.f, y}, false, {200, 50, 50, 255}, font, 0.25f);
	y += 30;
	RawText::Render("check it out on github, Colaboi2009/dokijam, if you are curious about spaghetti", {sdl.getWindowSize().x / 2.f, y}, false, {50, 50, 50, 255}, font, 0.25f);
	y += 150;
	RawText::Render("Music: https://opengameart.org/content/the-cave-of-ancient-warriors-1a-0", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.2f);
	y += 20;
	RawText::Render("Font: sourcecodevf from OnlineWebFonts", {sdl.getWindowSize().x / 2.f, y}, false, {200, 255, 200, 255}, font, 0.2f);
}
