#pragma once

#include "Board.h"



class Snake {

public:
	Snake(const Location& loc);
	void MoveBy(const Location& delta_loc);
	Location GetNextHeadLocation(const Location& delta_loc) const;
	void Grow();
	void Draw(Board& board) const;
	bool IsInTileExceptEnd(const Location& target) const;
	bool IsInTile(const Location& target) const;

	

private:
	class Segment {
		public:
			void InitHead(const Location& loc);
			void InitBody(Color c);
			void Follow(const Segment& next);
			void MoveBy(const Location& delta_loc);
			void Draw(Board& board) const;
			const Location& GetLocation() const;

		private:
			Location loc;
			Color c;

	};
	static constexpr Color headColor = Colors::Yellow;
	static constexpr int nSegmentsMax = 100;
	Segment segments[nSegmentsMax];
	int segments_curr = 1;
	
};
