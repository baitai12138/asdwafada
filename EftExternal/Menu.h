#pragma once
#include <vector>
#include <time.h>
#include "Overlay/Overlay.h"
#include "MenuCfg.h"
float menux = 0;
float menuy = 400;
int get_frame_rate()
{
	static int i_fps, i_last_fps;
	static float fl_last_tick_count, fl_tick_count;
	fl_tick_count = clock() * 0.001f;
	i_fps++;
	if ((fl_tick_count - fl_last_tick_count) >= 1.0f)
	{
		fl_last_tick_count = fl_tick_count;
		i_last_fps = i_fps;
		i_fps = 0;
	}
	return i_last_fps;
};

class c_menu_framework
{
private:
public:
	float menu_x = 250, menu_y = 290;
	enum item_type : int {
		type_bool = 0,
		type_int = 1,
		type_float = 2
	};
	enum tab_type : int {
		type_on_off = 0,
		type_tab = 1,
		type_dont_draw = 2
	};
	static struct menu_item_t {
		std::string m_title;

		float* m_float;
		int* m_int;
		bool* m_bool = 0;
		int increment = 1;
		int minimum = 0;
		int maximum = 1;
		float m_float_step;
		int m_int_step;
		int m_type;
		int tab_type;

		float m_float_min;
		int m_int_min;

		float m_float_max;
		int m_int_max;

		std::string tool_tip;
	};

	int m_current_pos = 0;
	int menu_page = 0;
	std::vector<menu_item_t> items;
public:

	void add_entry(std::string title, bool* value, int is_tab = 0, std::string tooltip = "") {
		menu_item_t m_item;
		m_item.m_title = title;
		m_item.m_bool = value;
		m_item.m_type = item_type::type_bool;
		m_item.tab_type = is_tab;
		m_item.tool_tip = tooltip;
		items.emplace_back(m_item);
	}

	template <class t>
	void add_entry(std::string title, t* value, t min, t max, t step) {
		menu_item_t m_item;
		m_item.m_title = title;
		if (std::is_same<t, float>()) {
			m_item.m_float = (float*)value;
			m_item.m_float_min = min;
			m_item.m_float_max = max;
			m_item.m_float_step = step;
			m_item.m_type = item_type::type_float;
		}
		else if (std::is_same<t, int>()) {
			m_item.m_int = (int*)value;
			m_item.m_int_min = min;
			m_item.m_int_max = max;
			m_item.m_int_step = step;
			m_item.m_type = item_type::type_int;
		}
		items.emplace_back(m_item);
	}

	void draw()
	{
		this->menu_x = menux;
		this->menu_y = menuy;

		static bool aimbot_tap = false;
		static bool espteam_tap = false;
		static bool espenemy_tab = false;
		static bool esploot_tab = false;
		static bool crosshair_tab = false;
		static bool color_tab = false;
		static bool misc_tab = false;
		static bool menu_tab = false;

		this->add_entry("aimbot", &aimbot_tap, true);
		if (aimbot_tap) {
			this->add_entry("aimbot ��", &MenuConf.AimBootEx);
			this->add_entry("aimbot fov", &MenuConf.EnableFov);
			this->add_entry("fov", &MenuConf.FovRadios, 100.f, 300.f, 50.f);
		}

		this->add_entry("ESP MAIN", &espenemy_tab, true);
		if (espenemy_tab) {
			this->add_entry("EnableVisual", &MenuConf.EnableVisual);
			this->add_entry("Skeleton", &MenuConf.EspPlayerSkeleton);
			this->add_entry("Name", &MenuConf.EspPlayerName);
			this->add_entry("WeaponName", &MenuConf.EspPlayerWeaponName);
			this->add_entry("Head", &MenuConf.EspPlayerHead);
			this->add_entry("Box", &MenuConf.EspPlayerBox);


		}
		this->add_entry("ESP LOOT", &esploot_tab, true);
		if (esploot_tab) {
			this->add_entry("LootTactical", &MenuConf.LootTactical);
			this->add_entry("WeaponParts", &MenuConf.LootWeaponP);
			this->add_entry("AmmoBoxes", &MenuConf.LootAmmoBox);
			this->add_entry("Currency", &MenuConf.LootCurrency);
			this->add_entry("SpecialEquipment", &MenuConf.LootSpecialE);
			this->add_entry("Maps", &MenuConf.LootMaps);
			//and more...
		}

		this->add_entry("Other ESP", &misc_tab, true);
		if (misc_tab) {
			this->add_entry("NoSway", &MenuConf.NoSway);
			this->add_entry("NoRecoil", &MenuConf.NoRecoil);
			this->add_entry("NoFallDamage", &MenuConf.NoFallDamage);
			this->add_entry("SlowFall", &MenuConf.SlowFall);
			this->add_entry("InfiniteStamina", &MenuConf.NoStamina);
			this->add_entry("NightVision", &MenuConf.Nightvision);
			this->add_entry("SpeedHack", &MenuConf.SpeedHack);
			this->add_entry("ThermalVision", &MenuConf.ThermalVision);
		}

		this->add_entry("Menu", &menu_tab, true);
		if (menu_tab) {
			this->add_entry("  X", &menux, 0.f, 2560.f, 50.f);
			this->add_entry("  Y", &menuy, 0.f, 1440.f, 50.f);

		}

	}
	time_t lastTime;
	bool ReadKeyOnce(unsigned int keycode, unsigned int timer) {
		if ((clock() - lastTime > timer) && GetAsyncKeyState(keycode)) {
			lastTime = clock();
			return true;
		}
		return false;
	}
	void do_menu_controls() {
		static bool show = true;
		if (show) {
			this->draw();
			static int item_gap = 14;
			static float alpha = 1.f;
			DrawBox(this->menu_x - 3, this->menu_y - 17, 154, 17, 1.f, 0.1f, 0.1f, 0.1f, alpha, true);
			DrawBox(this->menu_x - 3, this->menu_y - 17, 154, 17, 1.f, 1.0f, 1.0, 1.0f, alpha, false);

			DrawString("EFT - " + std::to_string(get_frame_rate()), 12, this->menu_x + 28, this->menu_y - 18, 1.f, 1.f, 1.f, alpha);

			DrawBox(this->menu_x - 3, this->menu_y, 154, (items.size() * item_gap), 1.f, 0.1f, 0.1f, 0.1f, alpha, true);
			DrawBox(this->menu_x - 3, this->menu_y, 154, (items.size() * item_gap), 1.f, 1.0f, 1.0, 1.0f, alpha, false);

			DrawBox(this->menu_x - 3, this->menu_y + (item_gap * m_current_pos), 154, item_gap, 1.f, 0.1f, 0.1f, 0.1f, alpha, true);
			DrawBox(this->menu_x - 3, this->menu_y + (item_gap * m_current_pos), 154, item_gap, 1.f, 1.0f, 1.0, 1.0f, alpha, false);

			if (items.empty())
				return;
			if (m_current_pos > items.size())
				m_current_pos = items.size();

			if (!items[m_current_pos].tool_tip.empty()) {
				DrawBox(this->menu_x - 3, this->menu_y + (items.size() * item_gap) + 3, 1, 154, 28, 31, 31, 31, alpha, true);
				DrawBox(this->menu_x - 3, this->menu_y + (items.size() * item_gap) + 3, 1, 154, 28, 102, 0, 204, alpha, false);

				DrawString("NOTE:", 12, this->menu_x, this->menu_y + (items.size() * item_gap) + 5, 1, 1, 1, alpha);
				DrawString(items[m_current_pos].tool_tip.c_str(), 11, this->menu_x, this->menu_y + (items.size() * item_gap) + 16, 1, 1, 1, alpha);

			}

			for (auto item = 0; item < items.size(); ++item) {
				auto height = this->menu_y + (item_gap * item);
				char txt[1024];
				switch (items[item].m_type)
				{
				case item_type::type_bool: {
					D3DXCOLOR _bool = *items[item].m_bool ? D3DXCOLOR(0, 1.f, 0, alpha) : D3DXCOLOR(1.f, 0, 0, alpha);

					DrawString(items[item].m_title.c_str(), 12, this->menu_x, height, 1, 1, 1, alpha);
					if (items[item].tab_type == tab_type::type_on_off)
						DrawString(*items[item].m_bool ? "ON" : "OFF", 11, this->menu_x + 130, height, _bool.r, _bool.g, _bool.b, _bool.a);
					else if (items[item].tab_type == tab_type::type_tab)
						DrawString(*items[item].m_bool ? "[-]" : "[+]", 11, this->menu_x + 130, height, _bool.r, _bool.g, _bool.b, _bool.a);
					break;
				}
				case item_type::type_int: {
					DrawString(items[item].m_title.c_str(), 11, this->menu_x, height, 1, 1, 1, alpha);
					sprintf(txt, "%i", *items[item].m_int);
					DrawString(txt, 11, this->menu_x + 130, height, 1, 1, 1, alpha);
					break;
				}
				case item_type::type_float: {
					DrawString(items[item].m_title.c_str(), 11, this->menu_x, height, 1.f, 1.f, 1.f, alpha);
					sprintf(txt, "%.1f", *items[item].m_float);
					DrawString(txt, 11, this->menu_x + 130, height, 1, 1, 1, alpha);
					break;
				}
				default:
					break;
				}

				if (ReadKeyOnce(VK_UP, 250)) {
					this->m_current_pos -= 1;
					if (this->m_current_pos < 0)
						m_current_pos = (m_current_pos > 0) ? (m_current_pos - 1) : items.size() - 1;
				}
				if (ReadKeyOnce(VK_DOWN, 250)) {
					this->m_current_pos += 1;
					if (this->m_current_pos > items.size() - 1)
						this->m_current_pos = 0;
				}

				if (ReadKeyOnce(VK_LEFT, 250)) {
					switch (items[m_current_pos].m_type) {
					case item_type::type_bool: {
						if (*items[m_current_pos].m_bool == 1) {
							*items[m_current_pos].m_bool -= items[m_current_pos].increment;
							if (*items[m_current_pos].m_bool < items[m_current_pos].minimum)
								*items[m_current_pos].m_bool = items[m_current_pos].minimum;
						}
						break;
					}
					case item_type::type_int: {

						*items[m_current_pos].m_int -= items[m_current_pos].m_int_step;
						if (*items[m_current_pos].m_int < items[m_current_pos].m_int_min)
							*items[m_current_pos].m_int = items[m_current_pos].m_int_max;
						break;
					}
					case item_type::type_float: {
						*items[m_current_pos].m_float -= items[m_current_pos].m_float_step;
						if (*items[m_current_pos].m_float < items[m_current_pos].m_float_min)
							*items[m_current_pos].m_float = items[m_current_pos].m_float_max;
						break;
					}
					default:
						break;
					}

				}
				if (ReadKeyOnce(VK_RIGHT, 250)) {
					switch (items[m_current_pos].m_type) {
					case item_type::type_bool: {
						if (*items[m_current_pos].m_bool == 0) {
							*items[m_current_pos].m_bool += items[m_current_pos].increment;
							if (*items[m_current_pos].m_bool > items[m_current_pos].maximum)
								
								*items[m_current_pos].m_bool = items[m_current_pos].maximum;
						}
						break;
					}
					case item_type::type_int: {
						*items[m_current_pos].m_int += items[m_current_pos].m_int_step;
						if (*items[m_current_pos].m_int > items[m_current_pos].m_int_max)
							*items[m_current_pos].m_int = items[m_current_pos].m_int_min;
						break;
					}
					case item_type::type_float: {
						*items[m_current_pos].m_float += items[m_current_pos].m_float_step;
						if (*items[m_current_pos].m_float > items[m_current_pos].m_float_max)
							*items[m_current_pos].m_float = items[m_current_pos].m_float_min;
						break;
					}
					default:
						break;
					}


				}
			}
		}
		if (!items.empty())
			items.clear();
		if (GetAsyncKeyState(VK_DELETE)) {
			show = !show;
			Sleep(150);
		}
	}

};


