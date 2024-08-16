
#include "Character.hpp"

void
Character::MoveUp (Character & pCharacter) noexcept
{
	pCharacter.vCoord += sVecYPOint;
}

void
Character::MoveDown (Character & pCharacter) noexcept
{
	pCharacter.vCoord -= sVecYPOint;
}

void
Character::MoveLeft (Character & pCharacter) noexcept
{
	pCharacter.vCoord -= sVecXPOint;
}

void
Character::MoveRight (Character & pCharacter) noexcept
{
	pCharacter.vCoord += sVecXPOint;
}

void
Character::MoveUpRight (Character & pCharacter) noexcept
{
	MoveUp (pCharacter);
	MoveRight (pCharacter);
}

void
Character::MoveDownRight (Character & pCharacter) noexcept
{
	MoveDown (pCharacter);
	MoveRight (pCharacter);
}

void
Character::MoveUpLeft (Character & pCharacter) noexcept
{
	MoveUp (pCharacter);
	MoveLeft (pCharacter);
}

void
Character::MoveDownLeft (Character & pCharacter) noexcept
{
	MoveDown (pCharacter);
	MoveLeft (pCharacter);
}

/*** Player ***/

Player::Player () noexcept : Character ()
{

}

Player::Player (const char * pName, UInt64 pLen) noexcept : Character (pName, pLen, 0, 0)
{
}

Player::~Player () noexcept
{
}

void
Player::Move (char i_VKeyID) noexcept
{
	// move the player based on the key pressed.
	switch (i_VKeyID) {

		case 'a':
		case 'A':

			MoveLeft (*this);
			break;

		case 'd':
		case 'D':

			MoveRight (*this);
			break;

		case 'w':
		case 'W':

			MoveUp (*this);
			break;

		case 's':
		case 'S':

			MoveDown (*this);
			break;

		default:

			printf ("\nWrong input. Please try again.\n");
	}
}

/*** Monster ***/

Monster::Monster () noexcept : Character ()
{

}

Monster::Monster (const char * pName, UInt64 pLen) noexcept : Character (pName, pLen, (Int8) GenRandomNum (-127, 127), (Int8) GenRandomNum (-127, 127))
{
}


Monster::~Monster () noexcept
{
}

void
Monster::Move () noexcept
{
	// AI movement:
	// randomizing the movement of the Monster.
	// It moves either straight (up, down, left or right) by one position or diagnolly (up-right, down-right, up-left, down-left) by one position.

	sMove[GenRandomNum(0, 7)] (*this);
}
