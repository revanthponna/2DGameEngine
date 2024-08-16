
#include "MonsterChase.hpp"
#include "HelperFuncs.hpp"

DynamicQueue<Monster> MonsterChase::vActiveMonsterQueue        {};
DynamicQueue<Monster> MonsterChase::vInactiveMonsterQueue      {};
Player				  MonsterChase::vPlayer                    {};
char				  MonsterChase::sNameArray[sNameArraySize] {"abc"};
UInt64				  MonsterChase::sRunningNumber             {0};	// starts as 1 and increases by 1 after each monster creation.

void
MonsterChase::CreateNewMonster () noexcept
{
		UInt8     numdigit;
		Monster * newmonster;

	// name for the new monster.

	memcpy (sNameArray, MONSTER_STR, sizeof(MONSTER_STR));

	numdigit = StringUtils::UInt2Str (sRunningNumber, (((char *) sNameArray) + sizeof(MONSTER_STR)), sNameArraySize - sizeof(MONSTER_STR) - 1);

	// is there an inactive monster available for re-use?
	newmonster = vInactiveMonsterQueue.Pick ();
	
	if (!newmonster)	// if not, let us allocate new one.
		newmonster = new Monster ();

	Vector2D vec = { GenRandomNum((Int32) 0, (Int32) 3000000), GenRandomNum((Int32) 0, (Int32) 3000000) };
	newmonster->SetCoord(vec);

	newmonster->SetName (sNameArray, sizeof(MONSTER_STR) + numdigit);	// set the name

	vActiveMonsterQueue.Post (newmonster);	// post it to active queue.

	if (sRunningNumber != _UI64_MAX)
		++sRunningNumber;
	else
		sRunningNumber = 0;		// 64-bit unsigned integer limit reached. so we wrap it around.
}

void
MonsterChase::DestroyMonster () noexcept
{
		Monster * destroymonster;

	destroymonster = vActiveMonsterQueue.Pick ();

	if (destroymonster)
		vInactiveMonsterQueue.Post (destroymonster);	// post it to inactive queue for re-use.
}

bool
MonsterChase::PrintAndMoveMonsters (tNode<Monster> * /*pPrevMonster*/, tNode<Monster> * pCurMonster, void * pContext) noexcept
{
		Monster * monster;

	monster = pCurMonster->uData;

	cout << endl;

	monster->GetName ()->Print ();

	//printf (" is at [%d, %d]\n", monster->GetCoord ().GetXCoord (), monster->GetCoord ().GetYCoord ());

	GLib::Sprite* bad = (GLib::Sprite *) pContext;

	float x = (float)monster->GetCoord().GetXCoord();
	float y = (float)monster->GetCoord().GetYCoord();

	GLib::Point2D	off = { 0.0f, 0.0f };

	off.x += x/20000.0f;
	off.y += y/20000.0f;

	GLib::Render(*bad, off, 0.0f, 0.0f);

	monster->Move ();

	return true;
}
