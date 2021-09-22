#pragma once

#include "Board.h"



class Snake {

public:
	Snake(const Location& loc);
	void MoveBy(const Location& delta_loc);
	void Grow();
	void Draw(Board& board) const;
	

private:
	class Segment {
		public:
			void InitHead(const Location& loc);
			void InitBody();
			void Follow(const Segment& next);
			void MoveBy(const Location& delta_loc);
			void Draw(Board& board) const;

		private:
			Location loc;
			Color c;

	};
	static constexpr Color headColor = Colors::Yellow;
	static constexpr Color bodyColor = Colors::Green;
	static constexpr int nSegmentsMax = 100;
	Segment segments[nSegmentsMax];
	int segments_curr = 1;
	
};
