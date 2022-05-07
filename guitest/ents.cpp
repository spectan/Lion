#include "incls.h"

auto ents::getPlayer() -> DWORD_PTR
{
	return process->Read<DWORD_PTR>(process->moduleBase + gameObj->local_player);
}
auto ents::getEntityList() -> DWORD_PTR
{
	return process->Read<DWORD_PTR>(process->moduleBase + gameObj->entity_list);
}

auto ents::isDead(DWORD_PTR dw_entity) -> bool
{
	return process->Read<int>(dw_entity + entityObj->i_dead);
}

auto ents::get_entity_team(DWORD_PTR dw_entity) -> bool
{
	return process->Read<int>(dw_entity + entityObj->i_team);
}

auto ents::get_entity_head(DWORD_PTR dw_entity) -> v3
{
	v3 v3_head_pos;
	for (auto i = 0; i < 3; i++)
	{
		v3_head_pos[i] = process->Read<float>(dw_entity + entityObj->v3_head_pos + 0x4 * i);
	}
	return v3_head_pos;
}

auto ents::get_entity_foot(DWORD_PTR dw_entity) -> v3
{
	v3 v3_entity_pos;
	for (auto i = 0; i < 3; i++)
	{
		v3_entity_pos[i] = process->Read<float>(dw_entity + entityObj->v3_foot_pos + 0x4 * i);
	}
	return v3_entity_pos;
}

auto ents::get_entity_view(DWORD_PTR dw_entity)-> v3
{
	v3 v3_view_pos;
	for (auto i = 0; i < 3; i++)
	{
		v3_view_pos[i] = process->Read<float>(dw_entity + entityObj->v3_view_pos + 0x4 * i);
	}
	return v3_view_pos;
}

auto ents::get_view_matrix() -> float
{
	return process->Read<DWORD_PTR>(process->moduleBase + gameObj->view_matrix);
}

v3 ents::get_entity_screen(v3 v3_world) -> v2
{
	float f_matrix[16];
	for (auto i = 0; i < 16; i++) // index is always 16 in 3D games
	{
		f_matrix[i] = process->Read<float>(process->moduleBase + gameObj->view_matrix + 0x4 * i);
	}

	return math->world_to_screen(v3_world, f_matrix, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
}

auto ents::get_entity_name(DWORD_PTR dw_entity) -> std::string
{
	char name[32];
	process->Read(dw_entity + entityObj->str_name, &name, 32);
	char c[32];
	sprintf(c, "%s", name);
	return std::string(c);
}

auto ents::get_player_count() -> int
{
	return process->Read<int>(process->moduleBase + gameObj->player_count);
}

auto ents::get_entity_in_crosshair() -> std::string
{
	char name[32];
	process->Read(process->moduleBase + gameObj->player_in_crosshair, &name, 32);
	char c[32];
	sprintf(c, "%s", name);
	return std::string(c);
}

auto ents::get_entity_weapon(DWORD_PTR dw_entity) -> int
{
	std::vector<int> v3_offsets = { entityObj->weapon_class, weaponObj->i_id };
	return process->Read_multi<int>(dw_entity, v3_offsets);
}

ents* Ents = new ents();
ents::Game* gameObj = new ents::Game();
ents::Entity* entityObj = new ents::Entity();
ents::Entity::Weapon* weaponObj = new ents::Entity::Weapon();
