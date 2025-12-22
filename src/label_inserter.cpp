#include "label_inserter.h"
#include <string>
#include <iostream>

static constexpr const char* WHITE = "\033[37m";
static constexpr const char* BRIGHT_MAGENTA = "\033[95m";
static constexpr const char* RESET = "\033[0m";
static constexpr const char* BRIGHT_YELLOW = "\033[93m";

void LabelInserter::ScanInstructions(std::vector<std::pair<std::string, uint32_t>>& dIVec, size_t idx, uint32_t startAdress)
{
	const std::string& s = dIVec[idx].first;
	LabelsToScan l;
	LabelNames labelType{};

	const uint64_t currentVA = startAdress + (idx * 4);
	for (auto& it : l.labelsToScan)
	{
		if (dIVec[idx].first.find(it.first) != std::string::npos)
			labelType = it.second;
	}

	if (labelType == LabelNames::B || labelType == LabelNames::BL)
	{
		std::string s1 = s;
		std::size_t pos = s1.find("0x");
		if (pos == std::string::npos)
			return;

		uint64_t value = std::stoull(s1.substr(pos), nullptr, 16);
		int dIdx = (value - startAdress) / 4;


		std::string s2 = s;
		std::size_t pos1 = s.find("0x");
		if (pos != std::string::npos)
		{
			s2.erase(pos1 - 1);
			s2 += " <_func" + std::to_string(m_references.size()) + ">";
		}

		for (auto& it : m_references)
		{
			if (it.first == value)
			{
				dIVec[idx].first = s2;
				return;
			}
		}
		m_references.push_back(std::make_pair(value, "_func" + std::to_string(m_references.size()) + ":"));

		dIVec[idx].first = s2;
		return;
	}

	if (labelType == LabelNames::SET_RETURN)
	{
		LabelNames labelType1{};
		for (auto& it : l.labelsToScan)
		{
			if (dIVec[idx + 1].first.find(it.first) != std::string::npos)
				labelType1 = it.second;
		}
		if(labelType1 == LabelNames::SAVE_FRAME_PTR)
			m_references.push_back(std::make_pair(currentVA, "_func" + std::to_string(m_references.size()) + ":"));
	}
}

void LabelInserter::InsertFunctionLabels(std::vector<std::pair<std::string, uint32_t>>& dIVec, size_t idx, uint32_t startAdress)
{
	const std::string& s = dIVec[idx].first;
	LabelsToScan l;
	LabelNames labelType{};

	const uint64_t currentVA = startAdress + (idx * 4);

	for (auto& it : m_references)
	{
		if (it.first == currentVA)
		{
			std::cout << "\n\n" << BRIGHT_YELLOW << it.second << std::endl;
			return;
		}
	}
}
