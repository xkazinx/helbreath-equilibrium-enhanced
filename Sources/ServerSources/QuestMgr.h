#pragma once

#include <common.h>

class QuestUnit;
class CClient;

class QuestMgr
{
public:
	~QuestMgr();
	void SendQuestData(CClient* player);
	array<unique_ptr<QuestUnit>, kMaxQuests> _list;

	bool Read(string file_name);
	void Send(CClient * player);
	void Reward(CClient * player, char * data);

	unique_ptr<QuestUnit> & At(QuestID id)
	{
		return _list[(int)id];
	}
};