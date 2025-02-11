#include <iostream>
#include <Windows.h>
#include "memory.h"

constexpr auto localPlayer = 0x17E0A8;
constexpr auto entityList = 0x18AC04;
constexpr auto hp = 0xEC;
constexpr auto armor = 0xF0;
constexpr auto ammo = 0x140;


int main() {

	Memory mem = {"ac_client.exe"};
	const auto moduleBaseAddress = mem.GetModuleAddress("ac_client.exe");

	const auto localPlayerAddress = mem.Read<std::uintptr_t>(moduleBaseAddress + localPlayer);
	const auto hpAddress = localPlayerAddress + hp;
	const auto armorAddress = localPlayerAddress + armor;
	const auto ammoAddress = localPlayerAddress + ammo;

	while (true)
	{
		mem.Write<int>(hpAddress, 100);
		mem.Write<int>(armorAddress, 100);
		mem.Write<int>(ammoAddress, 20);

	}

}

