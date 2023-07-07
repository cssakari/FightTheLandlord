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
int bid_history[2];//我的上上家，上家的叫分决策
vector<int> own;//我最开始的牌
int landlord;//地主序号
int pos;//我的序号
int publiccard[3];//地主公开的牌

struct info {
	vector<int>up;
	vector<int>upup;
};
vector<info> history;//历史输入
vector<vector<int> > response;//我的输出

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
	template <typename CARD_ITERATOR>
	void GenerateNotPassValid(CARD_ITERATOR begin, CARD_ITERATOR end) const;
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
				// 还真可以啊……
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
set<Card> myCards;
short mycounts[MAX_LEVEL + 1] = {};
//创建一个牌池
// 地主明示的牌有哪些
set<Card> landlordPublicCards;

// 大家从最开始到现在都出过什么
vector<vector<Card>> whatTheyPlayed[PLAYER_COUNT];
vector<CardCombo> whatTheyPlayed_Combo[PLAYER_COUNT];
// 当前要出的牌需要大过谁
CardCombo lastValidCombo;

// 大家还剩多少牌
short cardRemaining[PLAYER_COUNT] = { 17, 17, 17 };
// 我是几号玩家（0-地主，1-农民甲，2-农民乙）
int myPosition;

// 地主位置
int landlordPosition = -1;

// 地主叫分
int landlordBid = -1;

// 阶段
Stage stage = Stage::BIDDING;

// 自己的第一回合收到的叫分决策 上上家，上家
vector<int> bidInput;
//牌的状态
short cardstatus[55] = { 0 }; //0未出,1已出,-1为自己的未出牌,-3为明牌

//还没出且不在自己手上的牌
vector<Card> Remaining;

bool ifICanBeat(CardCombo b)
{
	if (b.comboType >= CardComboType::PLANE) {
		return 0;
	}
	CardCombo temp = b.findFirstValid(Remaining.begin(), Remaining.end());
	if (temp.comboType != CardComboType::PASS)
		return true;
	else return false;
}

void bid_read() {
	//第一回合有哪些牌
	for (unsigned i = 0; i < own.size(); i++) {
		int t = own[i];
		myCards.insert(t);
		Remaining.erase(remove(Remaining.begin(), Remaining.end(), t), Remaining.end());
		mycounts[card2level(t)]++;
		cardstatus[t] = -1;
	}
	for (unsigned i = 0; i < 2; i++)
		bidInput.push_back(bid_history[i]);
}

void play_read()
{
	// 首先处理第一回合，得知自己是谁、有哪些牌
	for (unsigned i = 0; i < own.size(); i++) {
		int t = own[i];
		myCards.insert(t);
		Remaining.erase(remove(Remaining.begin(), Remaining.end(), t), Remaining.end());
		mycounts[card2level(t)]++;
		cardstatus[t] = -1;
	}
	//叫分
	for (unsigned i = 0; i < 2; i++)
		bidInput.push_back(bid_history[i]);


	// history里第一项（上上家）和第二项（上家）分别是谁的决策
	int whoInHistory[] = { (myPosition - 2 + PLAYER_COUNT) % PLAYER_COUNT, (myPosition - 1 + PLAYER_COUNT) % PLAYER_COUNT };
	int turn = history.size();
	landlordPosition = landlord;
	myPosition = pos;
	whoInHistory[0] = (myPosition - 2 + PLAYER_COUNT) % PLAYER_COUNT;
	whoInHistory[1] = (myPosition - 1 + PLAYER_COUNT) % PLAYER_COUNT;
	cardRemaining[landlordPosition] += 3;
	for (unsigned i = 0; i < 3; i++)
	{
		landlordPublicCards.insert(publiccard[i]);
		cardstatus[publiccard[i]] = -3;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (landlordPosition == myPosition) {
			myCards.insert(publiccard[i]);
			mycounts[card2level(publiccard[i])]++;
		}
	}


	stage = Stage::PLAYING;

	for (int i = 0; i < turn; i++)
	{
		for (int ii = 0; ii < 2; ++ii)
			for (int j = 0; j < 20; ++j)
			{
				maynotHave[ii][j] = false;
			}
		if (history[i].up.empty() && history[i].upup.size() == 0)
			continue;



		// 逐次恢复局面到当前
		int howManyPass = 0;
		int cnt = 0;
		int passPlayer[2] = { 0,0 };


		//上上家
		int player = whoInHistory[0];	// 是谁出的牌
		vector<Card> playedCards;
		for (unsigned _ = 0; _ < history[i].upup.size(); _++) // 循环枚举这个人出的所有牌
		{
			int card = history[i].upup[_]; // 这里是出的一张牌
			Remaining.erase(remove(Remaining.begin(), Remaining.end(), card), Remaining.end());
			cardstatus[card] = 1;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			playedCards.push_back(card);
		}
		whatTheyPlayed_Combo[player].push_back(CardCombo(playedCards.begin(), playedCards.end()));
		whatTheyPlayed[player].push_back(playedCards); // 记录这段历史
		cardRemaining[player] -= history[i].upup.size();
		if (history[i].upup.empty()) {
			howManyPass++;
			for (int ii = 0; ii <= 2 && ii != player; ++ii)
			{

				if (ii < 2)
				{
					CardCombo temp(playedCards.begin(), playedCards.end());
					if (temp.comboType != CardComboType::PASS)
					{
						maynotHave[player][(int)temp.comboType] = true; break;
					}

				}
				else
				{

					passPlayer[cnt] = player;
					cnt++;
				}
			}
		}
		else
		{
			lastValidCombo = CardCombo(playedCards.begin(), playedCards.end());
			lastplayer = player;
		}

		//上家
		player = whoInHistory[1];	// 是谁出的牌
		playedCards.clear();
		for (unsigned _ = 0; _ < history[i].up.size(); _++) // 循环枚举这个人出的所有牌
		{
			int card = history[i].up[_]; // 这里是出的一张牌
			Remaining.erase(remove(Remaining.begin(), Remaining.end(), card), Remaining.end());
			cardstatus[card] = 1;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			playedCards.push_back(card);
		}
		whatTheyPlayed_Combo[player].push_back(CardCombo(playedCards.begin(), playedCards.end()));
		whatTheyPlayed[player].push_back(playedCards); // 记录这段历史
		cardRemaining[player] -= history[i].up.size();
		if (history[i].up.empty()) {
			howManyPass++;
			for (int ii = 0; ii <= 2 && ii != player; ++ii)
			{

				if (ii < 2)
				{
					CardCombo temp(playedCards.begin(), playedCards.end());
					if (temp.comboType != CardComboType::PASS)
					{
						maynotHave[player][(int)temp.comboType] = true; break;
					}

				}
				else
				{

					passPlayer[cnt] = player;
					cnt++;
				}
			}
		}
		else
		{
			lastValidCombo = CardCombo(playedCards.begin(), playedCards.end());
			lastplayer = player;
		}



		if (howManyPass == 2)
			lastValidCombo = CardCombo();

		if (i < turn - 1)
		{
			// 恢复自己曾经出过的牌
			vector<Card> playedCards;
			for (unsigned _ = 0; _ < response[i].size(); _++) // 循环枚举自己出的所有牌
			{
				int card = response[i][_]; // 这里是自己出的一张牌
				myCards.erase(card);	// 从自己手牌中删掉
				mycounts[card2level(card)]--;
				cardstatus[card] = 1;//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				playedCards.push_back(card);
			}
			whatTheyPlayed_Combo[myPosition].push_back(CardCombo(playedCards.begin(), playedCards.end()));
			whatTheyPlayed[myPosition].push_back(playedCards); // 记录这段历史
			cardRemaining[myPosition] -= response[i].size();
			CardCombo tempp(playedCards.begin(), playedCards.end());
			for (int ii = 1; ii <= cnt; ++ii)
				maynotHave[passPlayer[ii]][(int)tempp.comboType] = true;
		}
	}
	for (int _ = 0; _ < 54; ++_)//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	{
		if (cardstatus[_] == 0)
		{
			Remaining.push_back(_);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}
	}

}

/**
* 输出叫分0,1,2,3
*/
int bid(int value) {
	return value;
}

/**
* 输出打牌决策，begin是迭代器起点，end是迭代器终点
* CARD_ITERATOR是Card（即short）类型的迭代器
*/
template <typename CARD_ITERATOR>
int* play(CARD_ITERATOR begin, CARD_ITERATOR end)
{
	vector<int>myResponse;
	for (; begin != end; begin++) {
		int temp = int(*begin);
		myResponse.push_back(temp);
	}
	response.push_back(myResponse);
	int length = response.size();
	int* ans = new int[length];
	for (int i = 0; i < length; ++i) {
		ans[i] = myResponse[i];
	}
	return ans;
}


int bidValue = 0;//我的叫分决策
const int INI = 1000;
bool cmp1(CardCombo* a, CardCombo* b) {
	if (b->comboType == CardComboType::ROCKET) {

		return 1;
	}
	if (a->comboType == CardComboType::ROCKET) {

		return 0;
	}
	if (a->comboType == CardComboType::BOMB) {
		if (b->comboType == CardComboType::BOMB) {
			return a->comboLevel < b->comboLevel;
		}
		else {
			return 0;
		}
	}
	if (b->comboType == CardComboType::BOMB) {
		return 1;
	}
	if (a->comboType == b->comboType) {
		return a->comboLevel < b->comboLevel;
	}
	if (a->comboType < CardComboType::STRAIGHT && b->comboType < CardComboType::STRAIGHT) {
		if (a->comboLevel > 9 || b->comboLevel > 9) {
			return a->comboLevel < b->comboLevel;
		}
		if (a->comboType > CardComboType::PAIR && b->comboType > CardComboType::PAIR) {
			return a->comboLevel < b->comboLevel;
		}
	}
	return a->comboType > b->comboType;
}
bool cmp2(CardCombo* a, CardCombo* b) {
	if (b->comboType == CardComboType::ROCKET || b->comboType == CardComboType::BOMB) {

		return 1;
	}
	if (a->comboType == CardComboType::ROCKET || a->comboType == CardComboType::BOMB) {

		return 0;
	}
	if (a->comboType == b->comboType) {
		if (myPosition != landlordPosition && cardRemaining[landlordPosition] <= 2)
			return a->comboLevel > b->comboLevel;
		if (myPosition == landlordPosition && (myPosition != 0 && cardRemaining[0] == 1 || myPosition != 1 && cardRemaining[1] == 1 || myPosition != 2 && cardRemaining[2] == 1))
			return a->comboLevel > b->comboLevel;
		return a->comboLevel < b->comboLevel;

		//农民先出对，地主留对
	}
	else {
		if (myPosition != landlordPosition)
		{
			if (cardRemaining[landlordPosition] == 2)
			{
				if (b->comboType == CardComboType::PAIR)
					return true;
				if (a->comboType == CardComboType::PAIR)
					return false;
			}
			return a->comboType > b->comboType;
		}
		else
		{
			if (b->comboType == CardComboType::SINGLE && a->comboType == CardComboType::PAIR) {
				if (myPosition != 0 && cardRemaining[0] == 1 || myPosition != 1 && cardRemaining[1] == 1 || myPosition != 2 && cardRemaining[2] == 1)
					return true;
				return false;
			}
			if (a->comboType == CardComboType::SINGLE && b->comboType == CardComboType::PAIR) {
				if (myPosition != 0 && cardRemaining[0] == 1 || myPosition != 1 && cardRemaining[1] == 1 || myPosition != 2 && cardRemaining[2] == 1)
					return false;
				return true;
			}
			return a->comboType > b->comboType;
		}
	}
}
bool cmp3(CardCombo* a, CardCombo* b) {
	if (b->comboType == CardComboType::ROCKET) {
		return 1;
	}
	if (a->comboType == CardComboType::ROCKET) {

		return 0;
	}
	if (a->comboType == CardComboType::BOMB) {
		if (b->comboType == CardComboType::BOMB) {
			return a->comboLevel < b->comboLevel;
		}
		else {
			return 0;
		}
	}
	if (b->comboType == CardComboType::BOMB) {
		return 1;
	}
	if (a->comboType == b->comboType) {
		return a->comboLevel < b->comboLevel;
	}
	return a->comboType > b->comboType;

}
vector<CardCombo*> May;
bool ifToWin(vector<CardCombo*> t) {
	May.clear();
	sort(t.begin(), t.end(), cmp3);
	int cnt = 0;
	vector<CardCombo*> Beaten;
	for (vector<CardCombo*>::iterator it = t.begin(); it != t.end(); ++it) {
		if (ifICanBeat(*(*it))) {
			cnt++;
			Beaten.push_back(*it);
		}
		else {
			for (vector<CardCombo*>::iterator _it = Beaten.begin(); _it != Beaten.end(); ++_it) {
				if ((*_it)->canBeBeatenBy(**it)) {
					cnt--;
					May.push_back(*_it);
					Beaten.erase(_it);
					break;
				}
			}
		}
	}
	if (cnt < 2) {
		return 1;
	}
	else {
		return 0;
	}
}
int Score(vector<CardCombo*> p) {
	if (!myCards.size()) {
		return 5000;
	}
	int s = INI;
	for (vector<CardCombo*>::iterator it_combo = p.begin(); it_combo != p.end(); ++it_combo) {

		if (((*it_combo)->comboType == CardComboType::STRAIGHT && (*it_combo)->cards.size() > 7 || (*it_combo)->comboLevel == MAX_STRAIGHT_LEVEL)) {
			s += 10;//长顺或盖帽顺加分
			continue;
		}
		if (((*it_combo)->comboType == CardComboType::STRAIGHT2)) {
			s += 10;//双顺加分
			continue;
		}
		s += -MAX_STRAIGHT_LEVEL + (*it_combo)->comboLevel;
		vector<CardCombo*>::iterator it = it_combo + 1;
		for (; it != p.end(); ++it) {
			if ((*it_combo)->canBeBeatenBy(**it) && ((*it)->comboLevel - (*it_combo)->comboLevel > min(4, MAX_STRAIGHT_LEVEL - (*it_combo)->comboLevel))) {
				s += 10;//顺子回手
			}
		}
	}
	int S[MAX_STRAIGHT_LEVEL + 1] = { 0 };
	//找飞机和炸弹
	short avai = 0;//可以带走的单/双牌
	for (Level i = 0; i < 11; i++) {
		if (mycounts[i] == 3) {
			avai++;
			S[i] = (-MAX_LEVEL + i) * 0.3;//不合理（飞机坠毁）

			if (mycounts[i + 1] == 3) {
				s += 40;//有飞机加分
				avai++;
				i++;
				if (mycounts[i + 1] == 3) {
					s += 40;//大飞机
					avai++;
					i++;
				}
			}
			else {
				for (Level j = i + 1; j < 12; j++) {
					if (mycounts[j] == 3 && j - i > 3) {
						if (j + 1 < 12 && mycounts[j + 1] != 3)
							s += 10;//三带有回手，不合理（飞机）
					}
				}
			}
		}
		else if (mycounts[i] == 4) {
			S[i] = 50 + i * 2;//炸弹加分
		}
	}
	//特判火箭和炸弹2
	if (mycounts[level_joker] && mycounts[level_JOKER]) {
		s += 80;
	}
	if (mycounts[12] == 4) {
		s += 50;
	}
	for (Level i = 0; i < 12; i++) {
		switch (mycounts[i]) {
		case 0:S[i] = 0; break;
		case 1:S[i] = -2 * MAX_LEVEL + i; if (S[i] < 0 && avai) { S[i] = 0; avai--; } break;//可带走，不减分
		case 2:S[i] = -MAX_LEVEL + i * 2; if (S[i] < 0 && avai) { S[i] = 0; avai--; } break;
		default:break;
		}
		s += S[i];
	}
	//判断2和大小王
	s += mycounts[12] * 5 + mycounts[13] * 10 + mycounts[14] * 15;
	return s;
}//后期以出牌多为先！！！！！！！！！！！！！！！！！

//叫分策略
void Bid_Strategy(vector<CardCombo*> t, int k) {
	//先确定顺子与双顺(找出所有的顺子组合，并对每一种组合求总分，按照最高分叫地主）
	if (bidValue == 3) {
		return;
	}
	for (Level i = k; i <= MAX_STRAIGHT_LEVEL - 2; i++) {//
		if (mycounts[i] > 1 && mycounts[i + 1] > 1 && mycounts[i + 2] > 1) {
			mycounts[i] -= 2;
			mycounts[i + 1] -= 2;
			mycounts[i + 2] -= 2;
			vector<CardCombo*> t1 = t;
			t1.push_back(new CardCombo(CardComboType::STRAIGHT2, i, i + 2));
			Bid_Strategy(t1, 0);
			Level j = i + 3;
			while (mycounts[j] > 1 && j <= MAX_STRAIGHT_LEVEL) {
				mycounts[j] -= 2;
				vector<CardCombo*> t2 = t;
				t1.push_back(new CardCombo(CardComboType::STRAIGHT2, i, j));
				Bid_Strategy(t1, 0);
				j++;
			}
			for (int k = i; k < j; k++) {
				mycounts[k] += 2;
			}
		}
	}
	for (Level i = k; i < MAX_STRAIGHT_LEVEL - 4; i++) {
		if (mycounts[i] && mycounts[i + 1] && mycounts[i + 2] && mycounts[i + 3] && mycounts[i + 4]) {
			mycounts[i] -= 1;
			mycounts[i + 1] -= 1;
			mycounts[i + 2] -= 1;
			mycounts[i + 3] -= 1;
			mycounts[i + 4] -= 1;
			vector<CardCombo*> t1 = t;
			t1.push_back(new CardCombo(CardComboType::STRAIGHT, i, i + 4));
			Bid_Strategy(t1, i);
			Level j = i + 3;
			while (mycounts[j] && j <= MAX_STRAIGHT_LEVEL) {
				mycounts[j] -= 1;
				vector<CardCombo*> t2 = t;
				t1.push_back(new CardCombo(CardComboType::STRAIGHT, i, j));
				Bid_Strategy(t1, i);
				j++;
			}
			for (int k = i; k < j; k++) {
				mycounts[k] += 1;
			}
		}
	};
	int s = Score(t);
	bidValue = min(3, max(bidValue, (s - 800) / 100));
}
int MAX = 0;
vector<CardCombo*> BestCombo;
void FindBestCombo(vector<CardCombo*> t, int k) {
	//先确定顺子与双顺(找出所有的顺子组合，并对每一种组合求总分，按照最高分叫地主）
	for (Level i = k; i <= MAX_STRAIGHT_LEVEL - 2; i++) {//
		if (mycounts[i] > 1 && mycounts[i + 1] > 1 && mycounts[i + 2] > 1) {
			mycounts[i] -= 2;
			mycounts[i + 1] -= 2;
			mycounts[i + 2] -= 2;
			vector<CardCombo*> t1 = t;
			t1.push_back(new CardCombo(CardComboType::STRAIGHT2, i, i + 2));
			FindBestCombo(t1, 0);
			Level j = i + 3;
			while (mycounts[j] > 1 && j <= MAX_STRAIGHT_LEVEL) {
				mycounts[j] -= 2;
				vector<CardCombo*> t2 = t;
				t2.push_back(new CardCombo(CardComboType::STRAIGHT2, i, j));
				FindBestCombo(t2, 0);
				j++;
			}
			for (int k = i; k < j; k++) {
				mycounts[k] += 2;//恢复原状
			}
		}
	}
	for (Level i = k; i <= MAX_STRAIGHT_LEVEL - 4; i++) {
		if (mycounts[i] && mycounts[i + 1] && mycounts[i + 2] && mycounts[i + 3] && mycounts[i + 4]) {
			mycounts[i] --;
			mycounts[i + 1] --;
			mycounts[i + 2] --;
			mycounts[i + 3] --;
			mycounts[i + 4] --;
			vector<CardCombo*> t1 = t;
			t1.push_back(new CardCombo(CardComboType::STRAIGHT, i, i + 4));
			FindBestCombo(t1, i);
			Level j = i + 5;
			while (mycounts[j] && j <= MAX_STRAIGHT_LEVEL) {
				mycounts[j] --;
				vector<CardCombo*> t2 = t;
				t2.push_back(new CardCombo(CardComboType::STRAIGHT, i, j));
				FindBestCombo(t2, i);
				j++;
			}
			for (int k = i; k < j; k++) {
				mycounts[k] ++;//恢复原状
			}
		}
	};
	int s = Score(t);
	if (MAX < s) {
		MAX = s;
		BestCombo = t;
	}
}
CardCombo BestSolve;
int MAX_CHOICE = 0;


void Split_Combo() {
	for (vector<CardCombo*>::iterator it_combo = BestCombo.begin(); it_combo != BestCombo.end(); ++it_combo) {
		for (vector<Card>::iterator it_card = (*it_combo)->cards.begin(); it_card != (*it_combo)->cards.end(); ++it_card) {
			mycounts[*it_card]--;
			set<Card>::iterator tmp = myCards.lower_bound(4 * (*it_card));
			*it_card = *tmp;
			myCards.erase(tmp);
		}
	}
	vector<vector<Card>> avai;
	for (Level i = 0; i < 12; i++) {
		if (mycounts[i] == 3) {
			mycounts[i] = 0;
			i++;
			if (mycounts[i] == 3 && i < 12) {
				mycounts[i] = 0;
				i++;
				if (mycounts[i] == 3 && i < 12) {
					mycounts[i] = 0;
					vector<Card> Tri;
					set<Card>::iterator it_card = myCards.lower_bound((i - 2) * 4);
					for (int j = 0; j < 9; j++) {
						Card t = *it_card;
						Tri.push_back(t);
						++it_card;
						myCards.erase(t);
					}
					avai.push_back(Tri);
					i++;
				}
				else {
					vector<Card> Tri;
					set<Card>::iterator it_card = myCards.lower_bound((i - 2) * 4);
					for (int j = 0; j < 6; j++) {
						Card t = *it_card;
						Tri.push_back(t);
						++it_card;
						myCards.erase(t);

					}
					avai.push_back(Tri);
				}
			}
			else {
				vector<Card> Tri;
				set<Card>::iterator it_card = myCards.lower_bound((i - 1) * 4);
				for (int j = 0; j < 3; j++) {
					Card t = *it_card;
					Tri.push_back(t);
					++it_card;
					myCards.erase(t);
				}
				avai.push_back(Tri);
			}
		}
		else if (mycounts[i] == 4) {
			mycounts[i] = 0;
			vector<Card> Bomb;
			for (int j = 0; j < 4; j++) {
				Bomb.push_back(i * 4 + j);
				myCards.erase(i * 4 + j);
			}
			BestCombo.push_back(new CardCombo(Bomb.begin(), Bomb.end()));
		}
	}
	//特判ROCKET和炸弹2
	if (mycounts[12] == 3) {
		mycounts[12] = 0;
		vector<Card> Tri;
		set<Card>::iterator it_card = myCards.lower_bound(12 * 4);
		for (int j = 0; j < 3; j++) {
			Card t = *it_card;
			Tri.push_back(t);
			++it_card;
			myCards.erase(t);
		}
		avai.push_back(Tri);
	}
	if (mycounts[12] == 4) {
		mycounts[12] = 0;
		myCards.erase(48);
		myCards.erase(49);
		myCards.erase(50);
		myCards.erase(51);
		vector<Card> t = { 48,49,50,51 };
		BestCombo.push_back(new CardCombo(t.begin(), t.end()));
	}
	if (mycounts[level_joker] && mycounts[level_JOKER]) {
		mycounts[level_joker] = 0;
		mycounts[level_JOKER] = 0;
		myCards.erase(52);
		myCards.erase(53);
		vector<Card> t = { 52,53 };
		BestCombo.push_back(new CardCombo(t.begin(), t.end()));
	}
	int s;
	for (Level i = 0; i < 12 && avai.size(); i++) {
		switch (mycounts[i]) {
		case 0: break;
		case 1:s = -MAX_STRAIGHT_LEVEL + i; if (s < 0 && avai.size()) {
			mycounts[i] = 0;
			set<Card>::iterator it_card = myCards.lower_bound(i * 4);
			avai[0].push_back(*it_card);
			myCards.erase(it_card);//带走
			if (avai[0].size() / 3 == avai[0].size() % 3) {
				BestCombo.push_back(new CardCombo(avai[0].begin(), avai[0].end())); avai.erase(avai.begin());//填满则装入
			}
		} break;//带走成为Combo
		case 2:s = -MAX_LEVEL + i * 2; if (s < 0 && avai.size()) {//对子小于J时判断是否入内
			int j = 0;
			unsigned short size_t = avai.size();
			while (j < size_t && avai[j].size() % 3) { j++; }
			if (j != size_t) {
				switch (avai[j].size() / 3) {
				case 2:for (int k = i + 1; k < 8; k++) {
					if (mycounts[k] == 2 && k - i < 4) {//同为对子且差距较小
						mycounts[i] = 0;
						mycounts[k] = 0;
						set<Card>::iterator it_card = myCards.lower_bound(i * 4);
						Card t1 = *it_card;
						avai[j].push_back(t1);
						++it_card;
						myCards.erase(t1);
						avai[j].push_back(*it_card);
						myCards.erase(it_card);//Level=i的对子填入
						set<Card>::iterator it_card1 = myCards.lower_bound(k * 4);
						Card t2 = *it_card1;
						avai[j].push_back(t2);
						++it_card1;
						myCards.erase(t2);
						avai[j].push_back(*it_card1);
						myCards.erase(it_card1);//Level=k的对子填入
						BestCombo.push_back(new CardCombo(avai[j].begin(), avai[j].end())); avai.erase(avai.begin() + j);
						break;
					}
				}break;
				case 1:
					if (size_t > j + 1 && avai[j].size() == avai[j + 1].size() && avai[j][0] + 2 < avai[j + 1][0]) {//考虑回手
						for (int k = i + 1; k < 8; k++) {
							if (mycounts[k] == 2 && k - i < 5) {
								mycounts[i] = 0;
								mycounts[k] = 0;
								set<Card>::iterator it_card = myCards.lower_bound(i * 4);
								Card t1 = *it_card;
								avai[j].push_back(t1);
								++it_card;
								myCards.erase(t1);
								avai[j].push_back(*it_card);
								myCards.erase(it_card);
								set<Card>::iterator it_card1 = myCards.lower_bound(k * 4);
								Card t2 = *it_card1;
								avai[j + 1].push_back(t2);
								++it_card1;
								myCards.erase(t2);
								avai[j + 1].push_back(*it_card1);
								myCards.erase(it_card1);//与飞机同理
								BestCombo.push_back(new CardCombo(avai[j].begin(), avai[j].end())); avai.erase(avai.begin() + j);
								BestCombo.push_back(new CardCombo(avai[j].begin(), avai[j].end())); avai.erase(avai.begin() + j);//一起放入
								break;
							}
						}
					}
					else {//无回手
						int cnt = 1;
						for (int j = i + 1; j < 12; j++) {
							if (mycounts[j] == 2) {
								if (j < 7) {
									cnt++;
								}
								else {
									cnt--;
								}
							}
						}
						if (cnt) {
							mycounts[i] = 0;
							set<Card>::iterator it_card = myCards.lower_bound(i * 4);
							Card t1 = *it_card;
							avai[j].push_back(t1);
							it_card++;
							myCards.erase(t1);
							avai[j].push_back(*it_card);
							myCards.erase(it_card);
							BestCombo.push_back(new CardCombo(avai[j].begin(), avai[j].end())); avai.erase(avai.begin() + j);
							break;
						}
					}
				default:break;
				}
			}
		} break;
		default:break;
		}
	}
	for (unsigned int i = 0; i < avai.size(); i++) {
		if (avai[i].size() % 3 == 0) {
			BestCombo.push_back(new CardCombo(avai[i].begin(), avai[i].end()));
		}
		else {//飞机未填满
			while (avai[i].size() % 3) {
				myCards.insert(avai[i].back());
				mycounts[card2level(avai[i].back())]++;
				avai[i].pop_back();
			}
			vector<Level> cnt[2];
			for (int j = 0; j <= MAX_LEVEL; j++) {
				if (mycounts[j])
					cnt[mycounts[j] - 1].push_back(j);
				if (cnt[1].size() == 2) {//对子先满
					mycounts[cnt[1][0]] = 0;
					mycounts[cnt[1][1]] = 0;
					set<Card>::iterator it_card = myCards.lower_bound(cnt[1][0] * 4);
					Card t1 = *it_card;
					avai[i].push_back(t1);
					++it_card;
					myCards.erase(t1);
					avai[i].push_back(*it_card);
					myCards.erase(it_card);
					set<Card>::iterator it_card1 = myCards.lower_bound(cnt[1][1] * 4);
					Card t2 = *it_card1;
					avai[i].push_back(t2);
					++it_card1;
					myCards.erase(t2);
					avai[i].push_back(*it_card1);
					myCards.erase(it_card1);
					BestCombo.push_back(new CardCombo(avai[i].begin(), avai[i].end())); avai.erase(avai.begin() + i);
					break;
				}
				if (cnt[0].size() == 2) {//单牌先满
					mycounts[cnt[0][0]] = 0;
					mycounts[cnt[0][1]] = 0;
					set<Card>::iterator it_card = myCards.lower_bound(cnt[0][0] * 4);
					Card t1 = *it_card;
					avai[i].push_back(t1);
					++it_card;
					myCards.erase(t1);
					avai[i].push_back(*it_card);
					myCards.erase(it_card);
					BestCombo.push_back(new CardCombo(avai[i].begin(), avai[i].end())); avai.erase(avai.begin() + i);
					break;
				}
			}
			while (avai.size()) {
				BestCombo.push_back(new CardCombo(avai[0].begin(), avai[0].end())); avai.erase(avai.begin());
			}
		}
	}
	//剩下的牌为剩余的单牌、对子及2和王(未成火箭）
	for (Level i = 0; i < MAX_LEVEL - 1; i++) {//除大王外
		switch (mycounts[i]) {
		case 0:break;
		case 1: {mycounts[i] = 0; set<Card>::iterator it_card = myCards.lower_bound(i * 4); BestCombo.push_back(new CardCombo(*it_card)); myCards.erase(*it_card); break; }
		case 2: {mycounts[i] = 0;
			set<Card>::iterator it_card = myCards.lower_bound(i * 4);
			Card t = *it_card; it_card++;
			BestCombo.push_back(new CardCombo(t, *it_card));
			myCards.erase(*it_card); myCards.erase(t); break; }
		default:break;
		}
	}
	if (mycounts[level_JOKER]) {
		mycounts[level_JOKER] = 0;
		myCards.erase(53);
		BestCombo.push_back(new CardCombo(53));
	}
	return;
}
void backwards() {
	for (vector<CardCombo*>::iterator it_combo = BestCombo.begin(); it_combo != BestCombo.end(); ++it_combo) {
		for (vector<Card>::iterator it_card = (*it_combo)->cards.begin(); it_card != (*it_combo)->cards.end(); ++it_card) {
			mycounts[card2level(*it_card)]++;
			myCards.insert(*it_card);
		}
	}
	return;
}
void GenerateBestCombo(CardCombo t) {
	BestCombo.clear();
	MAX = 0;
	for (vector<Card>::iterator it_card = t.cards.begin(); it_card != t.cards.end(); ++it_card) {
		myCards.erase(*it_card);
		mycounts[card2level(*it_card)]--;
	}
	//先确定顺子与双顺(找出所有的顺子组合，并对每一种组合求总分，按照最高分叫地主）
	vector<CardCombo*> t1;
	FindBestCombo(t1, 0);
	Split_Combo();
	if (t.comboType != CardComboType::PASS && !ifICanBeat(t)) {
		MAX += 20;
		if (ifToWin(BestCombo)) {
			MAX += 500;
		}
	}
	if (t.comboType == CardComboType::PASS) {//农民要地主的牌，地主要所有人的牌
		if (MAX > MAX_CHOICE + 30 * (lastplayer == landlordPosition || myPosition == landlordPosition || cardRemaining[myPosition] < 4)
			+ 50 * ((lastplayer == landlordPosition || myPosition == landlordPosition) && cardRemaining[lastplayer] <= 2)) {
			MAX_CHOICE = MAX;
			BestSolve = t;
		}
	}
	else if (MAX > MAX_CHOICE) {
		MAX_CHOICE = MAX;
		BestSolve = t;
	}
	backwards();
	for (vector<Card>::iterator it_card = t.cards.begin(); it_card != t.cards.end(); ++it_card) {
		myCards.insert(*it_card);
		mycounts[card2level(*it_card)]++;
	}
	return;
}

bool judgebomb(CardCombo& b) {
	BestCombo.clear();
	MAX = 0;
	for (vector<Card>::iterator it_card = b.cards.begin(); it_card != b.cards.end(); ++it_card) {
		myCards.erase(*it_card);
		mycounts[card2level(*it_card)]--;
	}
	vector<CardCombo*> t;
	FindBestCombo(t, 0);
	Split_Combo();
	bool flag = ifToWin(BestCombo);
	for (vector<Card>::iterator it_card = b.cards.begin(); it_card != b.cards.end(); ++it_card) {
		myCards.insert(*it_card);
		mycounts[card2level(*it_card)]++;
	}
	return flag;
}
CardCombo& Play_Strategy() {
	vector<CardCombo*> t;
	FindBestCombo(t, 0);
	Split_Combo();
	if (ifToWin(BestCombo)) {
		if (May.size()) {
			return *May[0];
		}
		else {
			return *BestCombo[0];
		}
	}
	else {
		sort(BestCombo.begin(), BestCombo.end(), cmp1);
		vector<CardCombo*>::iterator it_combo = BestCombo.begin();
		if ((*it_combo)->comboType == CardComboType::STRAIGHT2 || (*it_combo)->cards.size() > 10) {
			return *BestCombo[0];
		}
		if ((*it_combo)->comboType > CardComboType::STRAIGHT) {
			if ((*it_combo)->comboLevel < 9)
				return *BestCombo[0];
			else
				++it_combo;
		}
		for (; it_combo != BestCombo.end(); ++it_combo) {
			if ((*it_combo)->comboType == CardComboType::STRAIGHT) {
				if (myPosition == landlordPosition && ifICanBeat(**it_combo) || ((*it_combo)->cards.size() > 7 || (*it_combo)->comboLevel == MAX_STRAIGHT_LEVEL)) {//1处加要不起的tag
					return *BestCombo[0];
				}
				else {//若要的起考虑回手
					for (vector<CardCombo*>::iterator it = it_combo + 1; it != BestCombo.end(); ++it) {
						if ((*it)->cards.size() == (*it_combo)->cards.size() && ((*it)->comboLevel - (*it_combo)->comboLevel > min(4, MAX_STRAIGHT_LEVEL - (*it_combo)->comboLevel))) {//有回手
							return *BestCombo[0];
						}
						if ((*it)->comboType != CardComboType::STRAIGHT) {
							break;
						}
					}
				}
			}
			else if ((*it_combo)->comboType >= CardComboType::TRIPLET) {
				if (myPosition == landlordPosition && ifICanBeat(**it_combo)) {//加要不起的tag
					return *BestCombo[0];
				}
				else {//若要的起考虑回手
					for (vector<CardCombo*>::iterator it = it_combo + 1; it != BestCombo.end(); ++it) {
						if ((*it)->comboType == (*it_combo)->comboType) {
							if ((*it)->comboLevel > 4)//有回手
								return *BestCombo[0];
						}
						else {
							break;//无回手
						}
					}
				}
			}
			else {
				break;//到单双牌了
			}
		}
		unsigned short i = 0;
		while ((*(BestCombo.begin() + i))->comboType > CardComboType::STRAIGHT && BestCombo.begin() + i + 1 != BestCombo.end()) {
			i++;
		}
		if ((*(BestCombo.begin() + i))->comboType >= CardComboType::PLANE)
			return **(BestCombo.begin() + i);
		else
			return *BestCombo[0];
	}
}
template <typename CARD_ITERATOR>
void CardCombo::GenerateNotPassValid(CARD_ITERATOR begin, CARD_ITERATOR end) const
{

	// 然后先看一下是不是火箭，是的话就过

	if (comboType == CardComboType::ROCKET) {
		BestSolve = CardCombo();
		return;
	}
	// 现在打算从手牌中凑出同牌型的牌
	auto deck = vector<Card>(begin, end); // 手牌
	short counts[MAX_LEVEL + 1] = {};

	unsigned short kindCount = 0;

	// 先数一下手牌里每种牌有多少个
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
		// 找到了合适的主牌，那么从牌呢？
		// 如果手牌的种类数不够，那从牌的种类数就不够，也不行
		if (kindCount < packs.size())
			goto failure;
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
		for (Level i = 1; comboLevel + i <= MAX_LEVEL; i++) // 增大多少//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		{
			for (int j = 0; j < mainPackCount && packs[j].level + i <= MAX_LEVEL; j++)
			{
				int level = packs[j].level + i;

				// 各种连续牌型的主牌不能到2，非连续牌型的主牌不能到小王，单张的主牌不能超过大王
				if ((comboType == CardComboType::SINGLE && level > MAX_LEVEL) ||
					(isSequential && level > MAX_STRAIGHT_LEVEL) ||
					(comboType != CardComboType::SINGLE && !isSequential && level >= level_joker))
					goto next;

				// 如果手牌中这种牌不够，就不用继续增了
				if (counts[level] < packs[j].count)
					goto next;
			}

			{

				// 好终于可以了
				// 计算每种牌的要求数目吧
				short requiredCounts[MAX_LEVEL + 1] = {}, requiredCounts1[MAX_LEVEL + 1] = {};
				vector<Card> zhupai;
				for (int j = 0; j < mainPackCount; j++)
				{
					requiredCounts[packs[j].level + i] = packs[j].count;
				}
				memcpy(requiredCounts1, requiredCounts, sizeof(requiredCounts));
				for (Card c : deck)
				{
					Level level = card2level(c);
					if (requiredCounts1[level])
					{
						zhupai.push_back(c);
						requiredCounts1[level]--;
					}
				}

				int m = packs.size() - mainPackCount;
				vector<Card> solve = zhupai;
				if (m > 0)
				{
					vector<Level> congpai;
					for (Level k = 0; k <= MAX_LEVEL; k++)
					{
						if (requiredCounts[k] || counts[k] < packs[mainPackCount].count)
							continue;
						requiredCounts[k] = packs[mainPackCount].count;
						congpai.push_back(k);
					}
					if (congpai.size() < m)
						goto next;

					short selectCounts[MAX_LEVEL + 1] = { 0 };
					int B[22] = { 0 }, n = congpai.size();

					//if(m==1){

					//}
					// 赋初始序列
					for (int ii = 0; ii < m; ii++)
					{
						B[ii] = ii;
					}

					while (B[0] <= n - m) // 最高位值不超过(N-M)
					{

						// 目前B数组中即为一种组合
						solve.clear();
						solve = zhupai;
						memcpy(selectCounts, requiredCounts, sizeof(requiredCounts));
						for (int r = 0; r < m; r++)
						{
							selectCounts[congpai[B[r]]] *= -1;
						}
						for (Card c : deck)
						{
							Level level = card2level(c);
							if (selectCounts[level] < 0)
							{
								solve.push_back(c);
								selectCounts[level]++;
							}
						}
						GenerateBestCombo(CardCombo(solve.begin(), solve.end()));


						// 生成下一种组合序列
						int k = m - 1;
						++B[k];
						while (B[k] > n - m + k && k >= 0)// 对应位超过最大值
						{
							k--;
							if (k >= 0)
								++B[k];
						}

						// 发生进位后，后续位在前一位基础上加1
					}
				}
				else
				{
					GenerateBestCombo(CardCombo(solve.begin(), solve.end()));
				}
			}

		next:; // 再增大
		}
		if (BestSolve.comboType == CardComboType::PASS) {
			goto failure;
		}
		else { GenerateBestCombo(CardCombo()); goto failure; }
	}
failure:
	for (Level i = 0; i < level_joker; i++)
		if (counts[i] == 4 && (comboType != CardComboType::BOMB || i > packs[0].level)) // 如果对方是炸弹，能炸的过才行
		{
			// 还真可以啊……
			Card bomb[] = { Card(i * 4), Card(i * 4 + 1), Card(i * 4 + 2), Card(i * 4 + 3) };
			CardCombo tmp = CardCombo(bomb, bomb + 4);
			if (judgebomb(tmp)) {
				BestSolve = tmp;
			}
			else {
				return;
			}
		}
	// 有没有火箭？
	if (counts[level_joker] + counts[level_JOKER] == 2)
	{
		Card rocket[] = { card_joker, card_JOKER };
		CardCombo tmp = CardCombo(rocket, rocket + 2);
		if (judgebomb(tmp)) {
			BestSolve = tmp;
		}
		else {
			return;
		}

	}
}

//判断是否合法
template <typename CARD_ITERATOR>
bool checkValid(CARD_ITERATOR begin, CARD_ITERATOR end) {
	CardCombo solve(begin, end);
	if ((BestSolve.comboType != CardComboType::INVALID) ||// 是合法牌
		(lastValidCombo.comboType != CardComboType::PASS && BestSolve.comboType == CardComboType::PASS) ||// 在上家没过牌的时候过牌
		(lastValidCombo.comboType != CardComboType::PASS && lastValidCombo.canBeBeatenBy(BestSolve)) ||// 在上家没过牌的时候出打得过的牌
		(lastValidCombo.comboType == CardComboType::PASS && BestSolve.comboType != CardComboType::INVALID))// 在上家过牌的时候出合法牌
		return true;
	else
		return false;

}

//true叫 false不叫
bool bid_decide() {
	stage = Stage::BIDDING;
	bid_read();
	// 做出决策
	vector<CardCombo*> p;
	Bid_Strategy(p, 0);
	unsigned int i = 0;
	for (; i < bidInput.size(); i++) {
		/*if (bidValue <= bidInput[i]) {*/
		if (bidInput[i] == 1) {
			return false;
			break;
		}
	}
	if (i == bidInput.size())
		return bid(bidValue) == 3 ? true : false;
}

int* play_decide()
{
	srand(time(nullptr));
	stage == Stage::PLAYING;
	play_read();


	// 做出决策
	if (lastValidCombo.comboType <= CardComboType::PASS) {
		CardCombo myAction = Play_Strategy();
		return play(myAction.cards.begin(), myAction.cards.end());
	}
	else {
		lastValidCombo.GenerateNotPassValid(myCards.begin(), myCards.end());

		//// 是合法牌
		//assert(BestSolve.comboType != CardComboType::INVALID);

		//assert(
		//	// 在上家没过牌的时候过牌
		//	(lastValidCombo.comboType != CardComboType::PASS && BestSolve.comboType == CardComboType::PASS) ||
		//	// 在上家没过牌的时候出打得过的牌
		//	(lastValidCombo.comboType != CardComboType::PASS && lastValidCombo.canBeBeatenBy(BestSolve)) ||
		//	// 在上家过牌的时候出合法牌
		//	(lastValidCombo.comboType == CardComboType::PASS && BestSolve.comboType != CardComboType::INVALID));

		//// 决策结束，输出结果（你只需修改以上部分）

		return play(BestSolve.cards.begin(), BestSolve.cards.end());
	}

}

#endif
