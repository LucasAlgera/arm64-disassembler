#pragma once
#include <utility>
#include <unordered_map>
#include <string>

enum LabelNames
{
	RETURN,
	RESTORE_STACK_PTR, 
	SET_STACK_PTR,
	SAVE_FRAME_PTR, 
	SET_RETURN,
	B, 
	BL, 
	Bdot
};

struct LabelsToScan
{
	std::unordered_map<std::string, LabelNames> labelsToScan =
	{
		{ "ret", RETURN },
		{ "stp x29, x30, [sp", SET_RETURN },
		{ "add x29, sp, #0", SAVE_FRAME_PTR },
		{ "sub sp, sp", SET_STACK_PTR }, 
		{ "b ", B },
		{ "bl ", BL },
		{ "b.", Bdot },

	};
};
class LabelInserter
{
public:
	void ScanInstructions(std::vector<std::pair<std::string, uint32_t>>& dIVec, size_t idx, uint32_t startAdress);
	void InsertFunctionLabels(std::vector<std::pair<std::string, uint32_t>>& dIVec, size_t idx, uint32_t startAdress);

private: 
	std::vector<std::pair<uint64_t, std::string>> m_references;
};
