#ifndef AI_H
#define AI_H
#include <iostream>
using namespace std;

#include <iostream>
#include <set>
#include <string>
#include <cassert>
#include <cstring> // 注意memset是cstring里的
#include <algorithm>
#include <vector>

using namespace std;

using std::set;
using std::sort;
using std::string;
using std::unique;
using std::vector;
using std::max;
using std::min;
constexpr int PLAYER_COUNT = 3;
//序号0，1，2

//游戏开始阶段修改
vector<int> own1;//我的牌
vector<int> own2;//我的牌
int landlord;//地主序号
int pos1;//我的序号
int pos2;//我的序号
int publiccard[3];//地主公开的牌
//每一轮修改
vector<int> last1;//上一家出牌
vector<int> last2;//上一家出牌





//不用修改
int turn1 = 0;
int turn2 = 0;
enum class Stage
{
	BIDDING, // 叫分阶段
	PLAYING	 // 打牌阶段
};

enum class CardComboType
{
	PASS,		// 过
	SINGLE,		// 单张
	PAIR,		// 对子
	TRIPLET,	// 三条
	TRIPLET1,	// 三带一
	TRIPLET2,	// 三带二
	STRAIGHT,	// 顺子
	BOMB,		// 炸弹
	QUADRUPLE2, // 四带二（只）
	QUADRUPLE4, // 四带二（对）
	PLANE,		// 飞机
	PLANE1,		// 飞机带小翼
	PLANE2,		// 飞机带大翼
	STRAIGHT2,	// 双顺
	SSHUTTLE,	// 航天飞机
	SSHUTTLE2,	// 航天飞机带小翼
	SSHUTTLE4,	// 航天飞机带大翼
	ROCKET,		// 火箭
	INVALID		// 非法牌型
};

int cardComboScores[] = {
	0,	// 过
	1,	// 单张
	2,	// 对子
	6,	// 顺子
	6,	// 双顺
	4,	// 三条
	4,	// 三带一
	4,	// 三带二
	10, // 炸弹
	8,	// 四带二（只）
	8,	// 四带二（对）
	8,	// 飞机
	8,	// 飞机带小翼
	8,	// 飞机带大翼
	10, // 航天飞机（需要特判：二连为10分，多连为20分）
	10, // 航天飞机带小翼
	10, // 航天飞机带大翼
	16, // 火箭
	0	// 非法牌型
};

#ifndef _BOTZONE_ONLINE
string cardComboStrings[] = {
	"PASS",
	"SINGLE",
	"PAIR",
	"STRAIGHT",
	"STRAIGHT2",
	"TRIPLET",
	"TRIPLET1",
	"TRIPLET2",
	"BOMB",
	"QUADRUPLE2",
	"QUADRUPLE4",
	"PLANE",
	"PLANE1",
	"PLANE2",
	"SSHUTTLE",
	"SSHUTTLE2",
	"SSHUTTLE4",
	"ROCKET",
	"INVALID" };
#endif

// 将1-13表示方式的序号转换为3333表示方式的序号
int convert_encoding1_to_encoding2(int n) {
	if (n >= 0 && n <= 51) {
		int rank = n % 13 + 1;
		int suit = n / 13;
		if (rank == 1)
			return 44 + suit;
		else if (rank == 2)
			return  48 + suit;
		else
			return (rank - 3) * 4 + suit;
	}
	else if (n == 52) {
		return 52;
	}
	else if (n == 53) {
		return 53;
	}
	else {
		return -1;
	}
}

// 将3333表示方式的序号转换为1-13表示方式的序号
int convert_encoding2_to_encoding1(int n) {
	if (n >= 0 && n <= 51) {
		int rank = n / 4 + 3;
		int suit = n % 4;
		if (n >= 44 && n <= 47)
			return (n % 4) * 13;
		else if (n >= 48 && n <= 51)
			return (n % 4) * 13 + 1;
		return rank - 1 + suit * 13;
	}
	else if (n == 52) {
		return 52;
	}
	else if (n == 53) {
		return 53;
	}
	else {
		return -1;
	}
}

// 用0~53这54个整数表示唯一的一张牌
using Card = short;
constexpr Card card_joker = 52;
constexpr Card card_JOKER = 53;
int lastplayer = -1;
// 除了用0~53这54个整数表示唯一的牌，
// 这里还用另一种序号表示牌的大小（不管花色），以便比较，称作等级（Level）
// 对应关系如下：
// 3 4 5 6 7 8 9 10	J Q K	A	2	小王	大王
// 0 1 2 3 4 5 6 7	8 9 10	11	12	13	14
using Level = short;
constexpr Level MAX_LEVEL = 15;
constexpr Level MAX_STRAIGHT_LEVEL = 11;
constexpr Level level_joker = 13;
constexpr Level level_JOKER = 14;
bool maynotHave[2][20];
/**
* 将Card变成Level
*/
constexpr Level card2level(Card card)
{
	return card / 4 + card / 53;
}

// 牌的组合，用于计算牌型
struct CardCombo
{
	// 表示同等级的牌有多少张
	// 会按个数从大到小、等级从大到小排序
	struct CardPack
	{
		Level level;
		short count;
		CardPack() {}
		CardPack(int a, int b) :level(a), count(b) {}
		bool operator<(const CardPack& b) const
		{
			if (count == b.count)
				return level > b.level;
			return count > b.count;
		}
	};
	vector<Card> cards;		 // 原始的牌，未排序
	vector<CardPack> packs;	 // 按数目和大小排序的牌种
	CardComboType comboType; // 算出的牌型
	Level comboLevel = 0;	 // 算出的大小序

	/**
						  * 检查个数最多的CardPack递减了几个
						  */
						  //找出数量最多的牌能组成的最大连续长度
	int findMaxSeq() const
	{
		for (unsigned c = 1; c < packs.size(); c++)
			if (packs[c].count != packs[0].count ||
				packs[c].level != packs[c - 1].level - 1)
				return c;
		return packs.size();
	}

	/**
	* 这个牌型最后算总分的时候的权重
	*/
	int getWeight() const
	{
		if (comboType == CardComboType::SSHUTTLE ||
			comboType == CardComboType::SSHUTTLE2 ||
			comboType == CardComboType::SSHUTTLE4)
			return cardComboScores[(int)comboType] + (findMaxSeq() > 2) * 10;
		return cardComboScores[(int)comboType];
	}

	// 创建一个空牌组
	CardCombo() : comboType(CardComboType::PASS) {}

	/**
	* 通过Card（即short）类型的迭代器创建一个牌型
	* 并计算出牌型和大小序等
	* 假设输入没有重复数字（即重复的Card）
	*/
	CardCombo(Card a) {
		comboType = CardComboType::SINGLE;
		cards.push_back(a);
		comboLevel = card2level(a);
		packs.push_back(CardPack(comboLevel, 1));
	}
	CardCombo(Card a, Card b) {
		comboType = CardComboType::PAIR;
		cards = { a,b };
		comboLevel = card2level(a);
		packs.push_back(CardPack(comboLevel, 2));
	}
	template <typename CARD_ITERATOR>
	CardCombo(CARD_ITERATOR begin, CARD_ITERATOR end)
	{
		// 特判：空
		if (begin == end)
		{
			comboType = CardComboType::PASS;
			return;
		}

		// 每种牌有多少个
		short counts[MAX_LEVEL + 1] = {};

		// 同种牌的张数（有多少个单张、对子、三条、四条）
		short countOfCount[5] = {};

		cards = vector<Card>(begin, end);
		for (Card c : cards)
			counts[card2level(c)]++;
		for (Level l = 0; l <= MAX_LEVEL; l++)
			if (counts[l])
			{
				packs.push_back(CardPack{ l, counts[l] });
				countOfCount[counts[l]]++;
			}
		sort(packs.begin(), packs.end());

		// 用最多的那种牌总是可以比较大小的
		comboLevel = packs[0].level;

		// 计算牌型
		// 按照 同种牌的张数 有几种 进行分类
		vector<int> kindOfCountOfCount;
		for (int i = 0; i <= 4; i++)
			if (countOfCount[i])
				kindOfCountOfCount.push_back(i);
		sort(kindOfCountOfCount.begin(), kindOfCountOfCount.end());

		int curr, lesser;

		switch (kindOfCountOfCount.size())
		{

		case 1: // 只有一类牌
			curr = countOfCount[kindOfCountOfCount[0]];
			switch (kindOfCountOfCount[0])
			{
			case 1:
				// 只有若干单张
				if (curr == 1)
				{
					comboType = CardComboType::SINGLE;
					return;
				}
				if (curr == 2 && packs[1].level == level_joker)
				{
					comboType = CardComboType::ROCKET;
					return;
				}
				if (curr >= 5 && findMaxSeq() == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::STRAIGHT;
					return;
				}
				break;
			case 2:
				// 只有若干对子
				if (curr == 1)
				{
					comboType = CardComboType::PAIR;
					return;
				}
				if (curr >= 3 && findMaxSeq() == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::STRAIGHT2;
					return;
				}
				break;
			case 3:
				// 只有若干三条
				if (curr == 1)
				{
					comboType = CardComboType::TRIPLET;
					return;
				}
				if (findMaxSeq() == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::PLANE;
					return;
				}
				break;
			case 4:
				// 只有若干四条
				if (curr == 1)
				{
					comboType = CardComboType::BOMB;
					return;
				}
				if (findMaxSeq() == curr &&
					packs.begin()->level <= MAX_STRAIGHT_LEVEL)
				{
					comboType = CardComboType::SSHUTTLE;
					return;
				}
			}
			break;
		case 2: // 有两类牌
			curr = countOfCount[kindOfCountOfCount[1]];
			lesser = countOfCount[kindOfCountOfCount[0]];
			if (kindOfCountOfCount[1] == 3)
			{
				// 三条带？
				if (kindOfCountOfCount[0] == 1)
				{
					// 三带一
					if (curr == 1 && lesser == 1)
					{
						comboType = CardComboType::TRIPLET1;
						return;
					}
					if (findMaxSeq() == curr && lesser == curr &&
						packs.begin()->level <= MAX_STRAIGHT_LEVEL)
					{
						comboType = CardComboType::PLANE1;
						return;
					}
				}
				if (kindOfCountOfCount[0] == 2)
				{
					// 三带二
					if (curr == 1 && lesser == 1)
					{
						comboType = CardComboType::TRIPLET2;
						return;
					}
					if (findMaxSeq() == curr && lesser == curr &&
						packs.begin()->level <= MAX_STRAIGHT_LEVEL)
					{
						comboType = CardComboType::PLANE2;
						return;
					}
				}
			}
			if (kindOfCountOfCount[1] == 4)
			{
				// 四条带？
				if (kindOfCountOfCount[0] == 1)
				{
					// 四条带两只 * n
					if (curr == 1 && lesser == 2)
					{
						comboType = CardComboType::QUADRUPLE2;
						return;
					}
					if (findMaxSeq() == curr && lesser == curr * 2 &&
						packs.begin()->level <= MAX_STRAIGHT_LEVEL)
					{
						comboType = CardComboType::SSHUTTLE2;
						return;
					}
				}
				if (kindOfCountOfCount[0] == 2)
				{
					// 四条带两对 * n
					if (curr == 1 && lesser == 2)
					{
						comboType = CardComboType::QUADRUPLE4;
						return;
					}
					if (findMaxSeq() == curr && lesser == curr * 2 &&
						packs.begin()->level <= MAX_STRAIGHT_LEVEL)
					{
						comboType = CardComboType::SSHUTTLE4;
						return;
					}
				}
			}
		}

		comboType = CardComboType::INVALID;
	}
	CardCombo(CardComboType type, Level begin, Level end) {
		comboType = type;
		if (comboType == CardComboType::STRAIGHT) {
			comboLevel = end;//最大牌
			for (Card i = begin; i <= end; i++) {
				cards.push_back(i);//由cards.size()判断顺子长短
			}
		}
		if (comboType == CardComboType::STRAIGHT2) {
			comboLevel = end;//最大牌
			for (Card i = begin; i <= end; i++) {
				cards.push_back(i); cards.push_back(i);//由cards.size()判断顺子长短
			}
		}
	}
	/**
	* 判断指定牌组能否大过当前牌组（这个函数不考虑过牌的情况！）
	*/
	bool canBeBeatenBy(const CardCombo& b) const
	{
		if (comboType == CardComboType::INVALID || b.comboType == CardComboType::INVALID)
			return false;
		if (b.comboType == CardComboType::ROCKET)
			return true;
		if (b.comboType == CardComboType::BOMB)
			switch (comboType)
			{
			case CardComboType::ROCKET:
				return false;
			case CardComboType::BOMB:
				return b.comboLevel > comboLevel;
			default:
				return true;
			}
		return b.comboType == comboType && b.cards.size() == cards.size() && b.comboLevel > comboLevel;
	}

	/**
	* 从指定手牌中寻找第一个能大过当前牌组的牌组
	* 如果随便出的话只出第一张
	* 如果不存在则返回一个PASS的牌组
	*/
	template <typename CARD_ITERATOR>
	CardCombo findFirstValid(CARD_ITERATOR begin, CARD_ITERATOR end) const
	{
		// 然后先看一下当前牌组是不是火箭，是的话就过
		if (comboType == CardComboType::ROCKET)
			return CardCombo();

		// 现在打算从手牌中凑出同牌型的牌
		auto deck = vector<Card>(begin, end); // 手牌
		short counts[MAX_LEVEL + 1] = {};// 统计每种等级的牌的数量

		unsigned short kindCount = 0;// 手牌里有多少种牌

		for (Card c : deck)
			counts[card2level(c)]++;

		// 手牌如果不够用，直接不用凑了，看看能不能炸吧
		if (deck.size() < cards.size())
			goto failure;

		// 再数一下手牌里有多少种牌
		for (short c : counts)
			if (c)
				kindCount++;

		// 否则不断增大当前牌组的主牌，看看能不能找到匹配的牌组
		{
			// 开始增大主牌
			int mainPackCount = findMaxSeq();
			bool isSequential =
				comboType == CardComboType::STRAIGHT ||
				comboType == CardComboType::STRAIGHT2 ||
				comboType == CardComboType::PLANE ||
				comboType == CardComboType::PLANE1 ||
				comboType == CardComboType::PLANE2 ||
				comboType == CardComboType::SSHUTTLE ||
				comboType == CardComboType::SSHUTTLE2 ||
				comboType == CardComboType::SSHUTTLE4;
			for (Level i = 1;; i++) // 主牌增大多少
			{
				for (int j = 0; j < mainPackCount; j++)
				{
					int level = packs[j].level + i;

					// 各种连续牌型的主牌不能到2，非连续牌型的主牌不能到小王，单张的主牌不能超过大王
					if ((comboType == CardComboType::SINGLE && level > MAX_LEVEL) ||
						(isSequential && level > MAX_STRAIGHT_LEVEL) ||
						(comboType != CardComboType::SINGLE && !isSequential && level >= level_joker))
						goto failure;

					// 如果手牌中这种牌不够，就不用继续增了
					if (counts[level] < packs[j].count)
						goto next;
				}

				{
					// 找到了合适的主牌，那么从牌呢？
					// 如果手牌的种类数不够，那从牌的种类数就不够，也不行
					if (kindCount < packs.size())
						continue;


					// 好终于可以了
					// 计算每种牌的要求数目吧
					short requiredCounts[MAX_LEVEL + 1] = {};
					//主牌数量
					for (int j = 0; j < mainPackCount; j++)
						requiredCounts[packs[j].level + i] = packs[j].count;
					//从牌数量
					for (unsigned j = mainPackCount; j < packs.size(); j++)
					{
						Level k;
						for (k = 0; k <= MAX_LEVEL; k++)
						{
							if (requiredCounts[k] || counts[k] < packs[j].count)
								continue;
							requiredCounts[k] = packs[j].count;
							break;
						}
						if (k == MAX_LEVEL + 1) // 如果是都不符合要求……就不行了
							goto next;
					}

					// 开始产生解
					vector<Card> solve;
					for (Card c : deck)
					{
						Level level = card2level(c);
						if (requiredCounts[level])
						{
							solve.push_back(c);
							requiredCounts[level]--;
						}
					}
					return CardCombo(solve.begin(), solve.end());
				}

			next:; // 再增大
			}
		}

	failure:
		// 实在找不到啊
		// 最后看一下能不能炸吧

		for (Level i = 0; i < level_joker; i++)
			if (counts[i] == 4 && (comboType != CardComboType::BOMB || i > packs[0].level)) // 如果对方是炸弹，能炸的过才行
			{
				Card bomb[] = { Card(i * 4), Card(i * 4 + 1), Card(i * 4 + 2), Card(i * 4 + 3) };
				return CardCombo(bomb, bomb + 4);
			}

		// 有没有火箭？
		if (counts[level_joker] + counts[level_JOKER] == 2)
		{
			Card rocket[] = { card_joker, card_JOKER };
			return CardCombo(rocket, rocket + 2);
		}

		// ……
		return CardCombo();
	}


};

/* 状态 */

// 我的牌有哪些
set<Card> myCards1;
short mycounts1[MAX_LEVEL + 1] = {};
set<Card> myCards2;
short mycounts2[MAX_LEVEL + 1] = {};
//创建一个牌池
// 地主明示的牌有哪些
set<Card> landlordPublicCards;

// 当前要出的牌需要大过谁
CardCombo lastValidCombo1;
CardCombo lastValidCombo2;

// 我是几号玩家（0-地主，1-农民甲，2-农民乙）
int myPosition1;
int myPosition2;

// 地主位置
int landlordPosition = -1;

// 阶段
Stage stage1 = Stage::BIDDING;
Stage stage2 = Stage::BIDDING;



void play_read1()
{
	// 首先处理第一回合，得知自己是谁、有哪些牌
	if (turn1 == 1) {
		for (unsigned i = 0; i < own1.size(); i++) {
			int t = own1[i];
			myCards1.insert(t);
			mycounts1[card2level(t)]++;
		}
		for (unsigned i = 0; i < 3; i++)
		{
			landlordPublicCards.insert(publiccard[i]);
			if (landlordPosition == myPosition1) {
				myCards1.insert(publiccard[i]);
				mycounts1[card2level(publiccard[i])]++;
			}
		}
	}
	else
		CardCombo lastValidCombo1(last1.begin(), last1.end());
}



/**
* 输出打牌决策，begin是迭代器起点，end是迭代器终点
* CARD_ITERATOR是Card（即short）类型的迭代器
*/
template <typename CARD_ITERATOR>
int* play1(CARD_ITERATOR begin, CARD_ITERATOR end)
{
	vector<int>myResponse;
	for (; begin != end; begin++) {
		int temp = *begin;
		myCards1.erase(temp);
		mycounts1[card2level(temp)]--;
		temp = int(*begin);
		myResponse.push_back(temp);
	}

	int length = myResponse.size();

	int* ans = new int[length];
	for (int i = 0; i < length; ++i) {
		ans[i] = myResponse[i];
	}
	return ans;
}


template <typename CARD_ITERATOR>
CardCombo& Play_Strategy1(CARD_ITERATOR begin, CARD_ITERATOR end) {
	CardCombo lastCombo(begin, end);
	return lastCombo.findFirstValid(myCards1.begin(), myCards1.end());
}


//判断是否合法
template <typename CARD_ITERATOR>
bool checkValid(CARD_ITERATOR begin1, CARD_ITERATOR end1, CARD_ITERATOR begin2, CARD_ITERATOR end2) {
	CardCombo solve(begin1, end1);
	CardCombo lastCombo(begin2, end2);
	if ((solve.comboType != CardComboType::INVALID) ||// 是合法牌
		(lastCombo.comboType != CardComboType::PASS && solve.comboType == CardComboType::PASS) ||// 在上家没过牌的时候过牌
		(lastCombo.comboType != CardComboType::PASS && lastCombo.canBeBeatenBy(solve)) ||// 在上家没过牌的时候出打得过的牌
		(lastCombo.comboType == CardComboType::PASS && solve.comboType != CardComboType::INVALID))// 在上家过牌的时候出合法牌
		return true;
	else
		return false;

}






void play_read2()
{
	// 首先处理第一回合，得知自己是谁、有哪些牌
	if (turn2 == 1) {
		for (unsigned i = 0; i < own2.size(); i++) {
			int t = own2[i];
			myCards2.insert(t);
			mycounts2[card2level(t)]++;
		}
		for (unsigned i = 0; i < 3; i++)
		{
			landlordPublicCards.insert(publiccard[i]);
			if (landlordPosition == myPosition2) {
				myCards2.insert(publiccard[i]);
				mycounts2[card2level(publiccard[i])]++;
			}
		}
	}
	else
		CardCombo lastValidCombo2(last2.begin(), last2.end());
}



/**
* 输出打牌决策，begin是迭代器起点，end是迭代器终点
* CARD_ITERATOR是Card（即short）类型的迭代器
*/
template <typename CARD_ITERATOR>
int* play2(CARD_ITERATOR begin, CARD_ITERATOR end)
{
	vector<int>myResponse;
	for (; begin != end; begin++) {
		int temp = *begin;
		myCards2.erase(temp);
		mycounts2[card2level(temp)]--;
		temp = int(*begin);
		myResponse.push_back(temp);
	}

	int length = myResponse.size();

	int* ans = new int[length];
	for (int i = 0; i < length; ++i) {
		ans[i] = myResponse[i];
	}
	return ans;
}


template <typename CARD_ITERATOR>
CardCombo& Play_Strategy2(CARD_ITERATOR begin, CARD_ITERATOR end) {
	CardCombo lastCombo(begin, end);
	return lastCombo.findFirstValid(myCards2.begin(), myCards2.end());
}



//true叫 false不叫
bool bid_decide1() {
	stage1 = Stage::BIDDING;
	int flag = rand() / double(RAND_MAX);
	if (flag > 0.5)
		return true;
	else
		return false;
}

int* play_decide1()
{
	turn1++;
	srand(time(nullptr));
	stage1 = Stage::PLAYING;
	play_read1();

	CardCombo myAction = Play_Strategy1(lastValidCombo1.cards.begin(), lastValidCombo1.cards.end());
	if ((myAction.comboType == CardComboType::PASS) || !checkValid(myAction.cards.begin(), myAction.cards.end(), lastValidCombo1.cards.begin(), lastValidCombo1.cards.end()))
		return nullptr;
	else
		return play1(myAction.cards.begin(), myAction.cards.end());

}

//true叫 false不叫
bool bid_decide2() {
	stage2 = Stage::BIDDING;
	int flag = rand() / double(RAND_MAX);
	if (flag > 0.5)
		return true;
	else
		return false;
}

int* play_decide2()
{
	turn2++;
	srand(time(nullptr));
	stage2 = Stage::PLAYING;
	play_read2();

	CardCombo myAction = Play_Strategy2(lastValidCombo2.cards.begin(), lastValidCombo2.cards.end());
	if ((myAction.comboType == CardComboType::PASS) || !checkValid(myAction.cards.begin(), myAction.cards.end(), lastValidCombo2.cards.begin(), lastValidCombo2.cards.end()))
		return nullptr;
	else
		return play2(myAction.cards.begin(), myAction.cards.end());

}

#endif
