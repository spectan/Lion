#pragma once
#include "incls.h"
class ents {
public:
	class Game {
	public:
		int view_matrix = 0x101AE8;
		int local_player = 0x10F4F4;
		int entity_list = 0x10F4F8;
		int player_count = 0x10F500;
		int player_in_crosshair = 0x101C38;
	};
	class Entity // dw_module
	{
	public:
		int v3_head_pos = 0x4;
		int v3_velocity = 0x10;
		int v3_foot_pos = 0x34;
		int v3_view_pos = 0x40;
		int f_max_speed = 0x50;
		int f_object_distance = 0x58;
		int f_height = 0x5C;
		int f_max_height = 0x60;
		int f_height_above_head = 0x64;
		int i_health = 0xF8;
		int i_armor = 0xFC;
		int i_granades = 0x158;
		int i_kills = 0x1FC;
		int i_flag_score = 0x200;
		int i_deaths = 0x204;
		int i_score = 0x208;
		int i_team_kills = 0x20C;
		int b_attack = 0x224;
		int str_name = 0x225; // char[260]
		int i_team = 0x32C;
		int i_dead = 0x338;
		int weapon_class = 0x374;
		int byte_norecoil = 0x63786;

		class Weapon // local_player + 0x374
		{
		public:
			int i_id = 0x4;
			int owner_class = 0x8;
			int info_class = 0xC;
			int clip_class = 0x10;
			int magazine_class = 0x14;
			int rapidfire_class = 0x18;

			// local_player + 0x374 + 0x8
			int entity_class = 0x0;

			// local_player + 0x374 + 0xC
			int str_weapon_name = 0x0; // char[260]
			int sh_damage = 0x10C;

			// local_player + 0x374 + 0x10
			int i_current_clip = 0x0;

			// local_player + 0x374 + 0x14
			int i_current_mag = 0x0;

			// local_player + 0x374 + 0x18
			int i_rapidfire = 0x0;
		};
	};
	public:
		auto getPlayer()->DWORD_PTR;
		auto getEntityList()->DWORD_PTR;
		auto isDead(DWORD_PTR dw_entity) -> bool;
		auto get_entity_team(DWORD_PTR dw_entity) -> bool;
		//auto get_entity_head(DWORD_PTR dw_entity) -> vec3;
		auto get_entity_head(DWORD_PTR dw_entity)->v3;
		auto get_entity_foot(DWORD_PTR dw_entity) -> v3;
		auto get_entity_view(DWORD_PTR dw_entity) -> v3;
		auto get_view_matrix() -> float;
		auto get_entity_screen(v3 v3_world) -> v2;
		auto get_entity_name(DWORD_PTR dw_entity)->std::string;
		auto get_player_count() -> int;
		auto get_entity_in_crosshair()->std::string;
		auto get_entity_weapon(DWORD_PTR dw_entity) -> int;

	public:
		bool bHealth = false;
		bool bAmmo = false;
		bool bNoRecoil = false;
		bool bRapidFire = false;
		bool bTrigger = false;
		bool bESP = false;
		bool bAim = false;
};

extern ents* entity;
extern ents::Game* gameObj;
extern ents::Entity* entityObj;
extern ents::Entity::Weapon* weaponObj;