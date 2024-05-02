#include "QuestMgr.h"
#include <sstream>
#include "Game.h"
#include "Item.h"
#include <fstream>
using std::ifstream;
using std::getline;

extern CGame * G_pGame;
//extern void PutLogList(const char * cMsg);

using std::istringstream;

bool QuestMgr::Read(string file_name)
{
	ifstream in(file_name);
	if(!in.good() || !in.is_open())
		return false;

	vector<string> lines;

	{
		string line;

		while (getline(in, line))
		{
			lines.emplace_back(line);
		}
	}

	string str;
	QuestUnit * quest = nullptr;

	enum class QuestDataID
	{
		None,
		QuestID,
		QuestMob,
		QuestMobName,
		QuestMobAmount,
		QuestMobMapName,
		QuestReward,
		QuestRewardItemName,
		QuestRewardAttribute,
		QuestRewardAmount,
		QuestRewardTime,
		QuestSide,
	};

	for (auto & line : lines)
	{
		QuestMob quest_mob;
		QuestReward quest_reward;
		QuestDataID data_id = QuestDataID::None;
		istringstream istr(line);
		while(getline(istr, str, ','))
		{
			switch(data_id)
			{
			case QuestDataID::None:
				if (str.starts_with("quest_id"))
				{
					data_id = QuestDataID::QuestID;
					continue;
				}
				else if(str.starts_with("quest_mob"))
				{
					data_id = QuestDataID::QuestMobName;
				}
				else if(str.starts_with("quest_reward"))
				{
					data_id = QuestDataID::QuestRewardItemName;
				}
				else if(str.starts_with("side"))
				{
					data_id = QuestDataID::QuestSide;
				}
				break;

			case QuestDataID::QuestID:
			{
				QuestID id = (QuestID)std::stoul(str);

				if(At(id))
					return false;

				At(id) = make_unique<QuestUnit>();
				At(id)->_id = id;
				quest = At(id).get();

				data_id = QuestDataID::None;
				continue;
			}

			case QuestDataID::QuestMobName:
			{
				quest_mob._name = str;

				bool found = false;
				for(auto npc : G_pGame->m_pNpcConfigList)
				{
					if(!npc)
						continue;

					if(npc->m_cNpcName == str)
					{
						found = true;
						break;
					}
				}

				if(!found)
				{
					//PutLogList(format("Npc {} for quest {} not found", str, quest->_id).c_str());
				}

				data_id = QuestDataID::QuestMobAmount;
				break;
			}

			case QuestDataID::QuestMobAmount:
			{
				quest_mob._count = std::stoul(str);
				data_id = QuestDataID::QuestMobMapName;
				break;
			}

			case QuestDataID::QuestMobMapName:
			{
				bool found = false;
				for(auto map : G_pGame->m_pMapList)
				{
					if(!map)
						continue;

					if(map->m_cName == str)
					{
						found = true;
						break;
					}
				}

				if(!found)
				{
					//PutLogList(format("Map {} for quest {} not found", str, quest->_id).c_str());
				}

				quest_mob._map = str;

				quest->_mobs.push_back(quest_mob);

				data_id = QuestDataID::None;
				break;
			}

			case QuestDataID::QuestRewardItemName:
			{
				data_id = QuestDataID::QuestRewardAmount;
				if(str == "Gold")
				{
					quest_reward._id = QuestRewardID::Gold;
				}
				else if(str == "Contribution")
				{
					quest_reward._id = QuestRewardID::Contribution;
				}
				else if(str == "Exp")
				{
					quest_reward._id = QuestRewardID::Experience;
				}
				else
				{
					quest_reward._item_name = str;

					bool found = false;
					for(auto item : G_pGame->m_pItemConfigList)
					{
						if(!item)
							continue;

						if(item->m_cName == str)
						{
							quest_reward._item = new CItem();
							*quest_reward._item = *item;
							quest_reward._item->m_dwAttribute = 0;//quest_reward._item_attr;
							found = true;
							break;
						}
					}

					if(!found)
					{
						found = found;
						//PutLogList(format("Item {} for quest {} not found", str, quest->_id).c_str());
					}

					quest_reward._id = QuestRewardID::Item;
					data_id = QuestDataID::QuestRewardAttribute;
				}
				break;
			}

			case QuestDataID::QuestRewardAttribute:
				quest_reward._item_attr = stoul(str);
				quest->_rewards.push_back(quest_reward);
				//data_id = QuestDataID::QuestRewardTime;
				data_id = QuestDataID::None;
				break;

			case QuestDataID::QuestRewardTime:
				//quest_reward._tm = stoul(str);
				//quest->_rewards.push_back(quest_reward);
				//data_id = QuestDataID::None;
				break;

			case QuestDataID::QuestRewardAmount:
				quest_reward._count = stoul(str);
				quest->_rewards.push_back(quest_reward);
				data_id = QuestDataID::None;
				break;

			case QuestDataID::QuestSide:
			{
				string side = str;
				if(side == "elvine")
					quest->_side = DEF_ELVINE;
				else if(side == "aresden")
					quest->_side = DEF_ARESDEN;
				break;
			}
			}
		}
	}

	int i = 0;
	i++;

	return true;
}

void QuestMgr::Send(CClient* player)
{
	char cData[64000];
	char * data = cData;
	Push(data, u32(MSG_NOTIFY_QUESTS));
	Push(data, u16(0));

	u16 * total = (u16*)data;
	Push(data, u16(0));

	for(auto & quest : _list)
	{
		if(!quest)
			continue;

		++(*total);

		Push(data, quest->_id);
		Push(data, quest->_side);

		Push(data, u8(quest->_mobs.size()));
		for(auto & mob : quest->_mobs)
		{
			Push(data, mob._count);
			Push(data, mob._map);
			Push(data, mob._name);
		}
		
		Push(data, u8(quest->_rewards.size()));
		for(auto & reward : quest->_rewards)
		{
			Push(data, reward._count);
			Push(data, reward._id);
			Push(data, reward._item_attr);
			Push(data, reward._item_name);
			Push(data, reward._tm);

			if(reward._id == QuestRewardID::Item)
			{	
				auto item = reward._item;
				item->m_dwCount = 1;
				Push(data, item->m_dwCount);
				Push(data, item->m_cItemType);
				Push(data, item->m_cEquipPos);
				Push(data, item->m_sLevelLimit);
				Push(data, item->m_cGenderLimit);
				Push(data, u16(item->m_wCurLifeSpan));
				Push(data, item->m_wWeight);
				Push(data, item->m_sSprite);
				Push(data, item->m_sSpriteFrame);
				Push(data, item->m_cItemColor);
				Push(data, item->m_sItemSpecEffectValue2);
				Push(data, item->m_dwAttribute);
				Push(data, item->m_sItemSpecEffectValue3);

			}
		}
	}

	player->m_pXSock->iSendMsg(cData, data - cData);
}

void QuestMgr::Reward(CClient * player, char* data)
{
	if(!player)
		return;

	if(player->_quest._id == kNullQuestID)
		return;

	int idx;
	Pop(data, idx);

	auto & quest = At(player->_quest._id);
	if(!quest)
		return;

	if(idx >= quest->_rewards.size())
		return;

	auto & reward = quest->_rewards[idx];
	switch(reward._id)
	{
	case QuestRewardID::Item:
	{
		auto pItem = new class CItem;
		if (G_pGame->_bInitItemAttr(pItem, reward._item_name.data()) == FALSE) {
			delete pItem;
			return;
		}
		else pItem->m_dwAttribute = (DWORD)reward._item_attr;
		//pItem->_expire_tm = time(0) + (reward._tm * 60 * 60 * 24);
		G_pGame->bAddItem(player->_handle, pItem, NULL);
		break;
	}

	case QuestRewardID::Gold:
	{
		auto pItem = new class CItem;
		if (G_pGame->_bInitItemAttr(pItem, "Gold") == FALSE) {
			delete pItem;
			return;
		}
		else pItem->m_dwCount = (DWORD)reward._count;
		G_pGame->bAddItem(player->_handle, pItem, NULL);
		break;
	}
	case QuestRewardID::Contribution:
		player->m_iContribution += reward._count;
		G_pGame->SendNotifyMsg(NULL, player->_handle, DEF_NOTIFY_CONTRIBUTION, player->m_iContribution, NULL, NULL, NULL);
		break;

	case QuestRewardID::Experience:
		player->m_iExpStock += reward._count;
		G_pGame->CalcExpStock(player->_handle);
		break;
	}
	
	player->_quest = QuestHandler();
	SendQuestData(player);
}

void QuestMgr::SendQuestData(CClient * player)
{
	if(!player)
		return;

	char data[512];
	auto p = data;

	Push(p, u32(MSG_QUEST_DATA));
	Push(p, u16(0));
	Push(p, player->_quest._id);

	if(player->_quest._id != kNullQuestID)
	{
		Push(p, u8(player->_quest._mobs.size()));
		for(auto & mob : player->_quest._mobs)
		{
			Push(p, mob._count);
			Push(p, mob._map);
			Push(p, mob._name);
			Push(p, mob._total);
		}
	}

	player->m_pXSock->iSendMsg(data, p - data);
}


QuestMgr::~QuestMgr()
{
	for(auto & quest : _list)
	{
		if(!quest)
			continue;

		for(auto & reward : quest->_rewards)
		{
			if(reward._id == QuestRewardID::Item && reward._item)
			{
				delete reward._item;
			}
		}
	}
}