#include <xinu.h>
#define DOGE "\033[1;36m+++////////////:::::::::::-----:++:--................:/+ss/````````````````\n\r++++////////////::::::::::-----/+o+/-.............-/+++ooss.```````````````\n\r+++++////////////:::::::::::--//::::::::---------/oo+ssyyoo-```````````````\n\r+++++++///////////::::::::::-----:://///::::://ooss+shmmyoo-```````````````\n\r+++++++////////////:::::-----...--:/+////:/::::/+ssoymmho++.```````````````\n\r+++++++++////////////:-....--....:/+/:::::///:::::+smmyoos:````````````````\n\r+++++++++///////////:...../so/--:/++:-----:///::::::/+++oo/.```````````````\n\r+++++++++++////////:-.```:sdmh:--::::--:++/:////:::--:/+syo:.``````````````\n\r++++++++++++///////:.` `.:oyo:-----:+ohsymnrdo/::::::::::+so/-``````````````\n\r+++++++++++++//////-````--..--...--:+ymsmmms/---------:::///-.`````````````\n\r++++++++++++++////:`````-://:--..---:::://:--.....-...--::/::..````````````\n\r+++++++++++++++///:```.mnnnnnd+....---.------..........---:::-..```````````\n\r++++++++++++++++//.```:mnMMMMno....--................---::/:::...``````````\n\r++++++++++++++++//.`.-+hnnnnds/:-----............-----::://:::-...`````````\n\r+++++++++++++++++/.``-+hmmmmds+/::::::-----------------://///::....````````\n\r+++++++++++++++++/.``.-ymnnnnmyo+ossyo:--::------...---://////:-.....``````\n\r++++++++++++++++++:.``.-/yhyhyhdyyys+:--::-----------:://+/+//::.....``````\n\r++++++++++++++++++/-`....-/::::/:::::::::::--------:::////+++///-......````\n\r+++++++++++++++++++:.....------::::::::::---------:::///+oo++///:.......```\n\r+++++++++++++++++++-.-...------:::::::::---------:::://++++///::-........``\n\r+++++++++++++++++++:..---------::::::::::::::::--:-:://++//::----........``\n\r+++++++++++++++++++:....--::::::://////////:-:-::-::://:---------.........`\n\r+++++++++++++++++++-`....---://///++///::---:---------..--...----..........\n\r++++++++++++++++++/.``...---::::///::::--------:----..........--:-.........\n\r+++++++++++++++++/:.`....----::::::::---------------........---::-.........\n\r++++++++++++++/:-..`....----::::://:---::-------..--.........-::/-.........\n\r++++++++++++:-...```....-----:::::::-:-----------.........---://+-.........\n\r+++++++++:-....`````...------::::-----------:::--......---:://///:.........\n\r+++++++/.....```````...---::::::--:-----------.---....------:/++/:-........\n\r+++++++:---.````````..----::::::::::-------------------...---//+/:-........\n\n\r\033[0;39m"
command xsh_doge()
{
	kprintf("%s\r\n", DOGE);
	return OK;
}
