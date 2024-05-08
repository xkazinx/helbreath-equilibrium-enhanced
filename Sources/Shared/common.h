#pragma once
#include <string>
using std::string;
#include <format>
using std::format;
#include <vector>
using std::vector;
#include <memory>
using std::unique_ptr;
#include <array>
using std::array;
using std::make_unique;

#define DEF_NEUTRAL 0
#define DEF_ARESDEN 1
#define DEF_ELVINE 2

typedef unsigned long long u64;
typedef signed long long i64;
typedef unsigned long u32;
typedef signed long i32;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned char u8;
typedef signed char i8;

template <typename T, class = typename std::enable_if<!std::is_pointer<T>::value>::type >
static void Push(char*& cp, T value) {
	auto p = (T*)cp;
	*p = (T)value;
	cp += sizeof(T);
}

template <typename T, class = typename std::enable_if<!std::is_pointer<T>::value>::type >
static void Pop(char*& cp, T& v) {
	T* p = (T*)cp;
	v = *p;
	cp += sizeof(T);
}

static void Push(char*& dest, const char* src, u32 len) {
	memcpy(dest, src, len);
	dest += len;
}

static void Push(char*& dest, const char* src) {

	strcpy(dest, src);
	dest += strlen(src) + 1;
}

//template <typename T, class = typename std::enable_if<std::is_same<T, const string>::value || std::is_same<T, string>::value>::type >
static void Push(char*& dest, const string& str) {
	strcpy(dest, str.c_str());
	dest += str.length() + 1;
}

static void Pop(char*& src, char* dest, u32 len) {
	memcpy(dest, src, len);
	src += len;
}
static void Pop(char*& src, char* dest) {

	u32 len = strlen(src) + 1;
	memcpy(dest, src, len);
	src += len;
}

static void Pop(char*& src, string& str) {
	str = src;
	src += str.length() + 1;
}


static constexpr auto kMaxQuests = 255;

using QuestID = u8;
constexpr QuestID kNullQuestID = 0;

class QuestMob
{
public:
	u32 _count;
	string _name;
	string _map;
};

enum class QuestRewardID : u8
{
	Item,
	Gold,
	Contribution,
	Experience,
	Max
};

class CItem;
class QuestReward
{
public:
	QuestRewardID _id;
	u32 _item_attr = 0;
	u32 _count = 1;
	time_t _tm = 0;
	string _item_name;
	CItem* _item = nullptr;
};

class QuestUnit
{
public:
	QuestID _id;
	vector<QuestMob> _mobs;
	vector<QuestReward> _rewards;
	char _side = 0; // DEF_NEUTRAL

	u32 GetContribution()
	{
		for (auto& reward : _rewards)
		{
			if (reward._id == QuestRewardID::Contribution)
			{
				return reward._count;
			}
		}
	}
};

class QuestHandler
{
public:
	class Mob
	{
	public:
		string _name;
		string _map;
		u32 _count, _total;
	};

	bool IsCompleted() const
	{
		if (_id == kNullQuestID)
			return false;

		for (auto& mob : _mobs)
		{
			if (mob._count > 0)
				return false;
		}

		return true;
	}

	QuestID _id = kNullQuestID;
	vector<Mob> _mobs;
};

class TradeItemUnit
{
public:
	int _contrib = 0, _majs = 0, _eks = 0;
	string _item_name;
	int _item_id;
	int _id;
};