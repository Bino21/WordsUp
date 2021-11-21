/*
 * This file is part of WordsUp program.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <random>       
#include <chrono>       
#include "Word.h"
#include "Label.h"
#include "RoundedRectangle.h"
#include "Letter.h"

/**
* \brief Creates an exercise where you need to collect a word from mixed letters
* 
* Controls item positions, processes events, and displays items
*/
class CollectWords : public sf::Drawable
{	
public:
	CollectWords();
	void setWidth(int width);

	/**
	* \brief Ñreates a new word from mixed letters
	* 
	* _shufleWord contains the letters that make up the word. The function clears _shufleWord
	*	from previous letters, creates new letters there and sets their position
	* 
	* @param[in] newWord set the word to be collected from the letters
	*/
	void newShufleWord(const Word& newWord);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void onClick(float x, float y);

	/// Checks that the word is typed correctly
	void wordAudit();
	bool answerIsCorrect() const;
	bool isAnswered() const;

private:
	Word _word;
	Label _question;
	Label _result;
	Label _hint;
	RoundedRectangle _rRectTop;
	RoundedRectangle _rRectDown;
	std::vector<Letter*> _shufleWord;
	int _countSortPositionLetters;
	bool _isCorrect;
	bool _answerIsGiven;
	int _maximumWidth;
};
