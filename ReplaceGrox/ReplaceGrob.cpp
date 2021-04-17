#include "stdafx.h"
#include "ReplaceGrob.h"
#include < Spore\Properties.h >

ReplaceGrob::ReplaceGrob()
{
}


ReplaceGrob::~ReplaceGrob()
{
}

void ReplaceGrob::ParseLine(const ArgScript::Line& line)
{
	if (Simulator::IsSpaceGame()) //check if command is run in space stage. if not, print an error to the cheat console
	{
		if (line.HasFlag("revert")) //check if the grox should be reverted or not.
		{
			auto grobID = StarManager.GetEmpire(StarManager.GetGrobEmpireID()); //get Grox empire
			auto SelectedSpecies = SpeciesManager.GetSpeciesProfile({ 0x06577404, TypeIDs::crt, GroupIDs::CreatureModels }); //find Grox species
			grobID->SetSpeciesProfile(SelectedSpecies); //set Grox empire to use original Grox species
		}
		else
		{
			Sporepedia::ShopperRequest request(this); //create Sporepedia request
			request.shopperID = 0xF25AEF31; //0xF25AEF31 filter = every type of creature, including captains. only present in EP1 packages.
			request.Show(request); //open Sporepedia with said request
		}
	}
	else
	{
		App::ConsolePrintF("This cheat can only be used in the Space Stage. It will persist between saves, though.");
	}
}

void ReplaceGrob::OnShopperAccept(const ResourceKey& Selection)
{
	if (Selection != ResourceKey(0, 0, 0) && Selection.groupID == GroupIDs::CreatureModels) //check if creature was chosen
	{
		SporeDebugPrint(to_string(Selection.instanceID).c_str()); //if debug build, print instance ID of chosen creature
		auto grobID = StarManager.GetEmpire(StarManager.GetGrobEmpireID()); ///get Grox empire
		auto SelectedSpecies = SpeciesManager.GetSpeciesProfile(Selection); //find chosen species
		if (SelectedSpecies != nullptr) { grobID->SetSpeciesProfile(SelectedSpecies); } //if species is valid, replace the species used by the Grox empire
		else { App::ConsolePrintF("The selected creature is invalid."); } //if not, print an error to the cheat console
	}
}

const char* ReplaceGrob::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Replaces the Grox in the space stage with a species of your choice.";
	}
	else {
		return "ReplaceGrox: When run without any flags, this cheat opens the Sporepedia to let you choose what species should replace the Grox.\n"
			"When used with the '-revert' flag, this cheat just resets the Grox back to their default species of, well, the Grox.";
	}
}
