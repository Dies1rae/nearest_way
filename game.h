#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <queue>

#include "common.h"

using namespace std;

class Screen {
public:
	Screen(const Point& max_res = Point{MAX_X, MAX_Y}) : max_scr_res_(max_res), field_(0) {
		this->field_.resize((size_t)this->max_scr_res_.x_);
		for(auto& col : this->field_) {
			col.resize((size_t)this->max_scr_res_.y_, FIELDDOT);
		}
	}

	bool init() {
		return !this->field_.empty() && !this->field_[0].empty();
	}
	
	void rePrintScr(void) const {
		cout << "\033[0;0H";
		for(const auto& col : this->field_) {
			for(const auto& row : col) {
				cout << row;
			}
			cout << '\n';
		}
	}

	bool putChar(const Point& p, const unsigned char& ch) {
		if(p.x_ >=  max_scr_res_.x_ || p.y_ >= max_scr_res_.y_ || p.x_ < 0 || p.y_ < 0) {
			cerr << "Coord error\n";
			return false;		
		}
		this->field_[p.x_][p.y_] = ch;
		return true;
	}

	bool removeChar(const Point& p) {
		this->putChar(p, FIELDDOT);
	}

    vector<vector<unsigned char>> getField(void) const {
		return this->field_;
	}
	
	const Point getScrRes(void) const {
		return this->max_scr_res_;
	}
private:
	Point max_scr_res_ {20, 100};
	vector<vector<unsigned char>> field_;
};

class Game {
public:
	enum algotype : short {
		NEUMANN = 0,
		MOORE = 1,
	};
	
	Game(Screen& scr) : scr_(scr),  start_end_({0, 0}, {0, 0}), calc_way_(0), way_(0) {
		this->calc_way_.resize(this->scr_.getField().size());
		for(auto& el : this->calc_way_) {
			el.resize(this->scr_.getField()[0].size());
		}
		this->refreshWay();
	}
	
	void putEnd(const Point& p) {
	   for(size_t x = 0; x < this->scr_.getField().size(); ++x) {
			for(size_t y = 0; y < this->scr_.getField()[x].size(); ++y) {
				if( this->scr_.getField()[x][y] == (unsigned char)WEND ) {
					Point remove(x, y);
					this->scr_.removeChar(remove);
					break;
				}
			}
		}
		this->scr_.putChar(p, WEND);
		this->start_end_.second = p;
	}
	
	void putStart(const Point& p) {
		for(size_t x = 0; x < this->scr_.getField().size(); ++x) {
			for(size_t y = 0; y < this->scr_.getField()[x].size(); ++y) {
				if( this->scr_.getField()[x][y] == (unsigned char)WSTART ) {
					Point remove(x, y);	
					this->scr_.removeChar(remove);
					break;
				}
			}
		}
		this->scr_.putChar(p, WSTART);
		this->start_end_.first = p;
	}
	
	void putWall(const Point& p) {
		if(this->scr_.getField()[p.x_][p.y_] == WALL) {
			this->putDot(p);
			return;
		}
		this->scr_.putChar(p, WALL);
	}
	
	void putDot(const Point& p) {
		this->scr_.removeChar(p);
	}

	void run(void) {
		while(true) {
			this->calculateWay();
			this->traceBack();
			this->markWayOnScr();
			this->scr_.rePrintScr();
			this->userMove();
		}
	}

	void calculateWay(void) {
		this->refreshWay();
		this->way_.clear();
	    switch(this->type_) {
		case algotype::MOORE:
			//TODO
		default:
			//ERROR BASICALLY
			break;
		case algotype::NEUMANN:
			queue<Point> hsh;
			hsh.push(this->start_end_.first);
			
			while(!hsh.empty()) {
				Point tmp = hsh.front();
				hsh.pop();
				
				//check LEFT
				if(tmp.x_ - 1 >= 0) {
					if(this->calc_way_[tmp.x_ - 1][tmp.y_] == -1) {
						this->calc_way_[tmp.x_ - 1][tmp.y_] = this->calc_way_[tmp.x_][tmp.y_] + 1;
						hsh.push({tmp.x_ - 1, tmp.y_});
						if(this->calc_way_[tmp.x_ - 1][tmp.y_] == WEND) {
							this->IsFound_ = true;
						}
					}
				}
				//check RIGHT
				if(tmp.x_ + 1 < this->calc_way_.size()) {
					if(this->calc_way_[tmp.x_ + 1][tmp.y_] == -1) {
						this->calc_way_[tmp.x_ + 1][tmp.y_] = this->calc_way_[tmp.x_][tmp.y_] + 1;
						hsh.push({tmp.x_ + 1, tmp.y_});
						if(this->calc_way_[tmp.x_ + 1][tmp.y_] == WEND) {
							this->IsFound_ = true;
						}
					}
				}
				//UP
				if(tmp.y_ - 1 >= 0) {
					if(this->calc_way_[tmp.x_][tmp.y_ - 1] == -1) {
						this->calc_way_[tmp.x_][tmp.y_ - 1] = this->calc_way_[tmp.x_][tmp.y_] + 1;
						hsh.push({tmp.x_, tmp.y_ - 1});
						if(this->calc_way_[tmp.x_][tmp.y_ - 1] == WEND) {
							this->IsFound_ = true;
						}
					}
				}
				//DOWN
				if(tmp.y_ + 1 < this->calc_way_[0].size()) {
					if(this->calc_way_[tmp.x_][tmp.y_ + 1] == -1) {
						this->calc_way_[tmp.x_][tmp.y_ + 1] = this->calc_way_[tmp.x_][tmp.y_] + 1;
						hsh.push({tmp.x_, tmp.y_ + 1});
						if(this->calc_way_[tmp.x_][tmp.y_ + 1] == WEND) {
							this->IsFound_ = true;
						}
					}
				}
			}
			break;
		}
	}

	void traceBack(void) {
		if(this->IsFound_) {
			this->way_.clear();
			queue<Point> hsh;
			hsh.push(this->start_end_.second);
			while(!hsh.empty()) {
				Point tmp = hsh.front();
			    Point min_coord = tmp;
				int min = this->calc_way_[tmp.x_][tmp.y_];
				hsh.pop();
				
				//check LEFT
				if(tmp.x_ - 1 >= 0 && this->calc_way_[tmp.x_ - 1][tmp.y_] < min) {
					min = this->calc_way_[tmp.x_ - 1][tmp.y_];
					min_coord = {tmp.x_ - 1, tmp.y_};
				}
				//check RIGHT
				if(tmp.x_ + 1 < this->calc_way_.size() && this->calc_way_[tmp.x_ + 1][tmp.y_] < min) {
					min = this->calc_way_[tmp.x_ + 1][tmp.y_];
					min_coord = {tmp.x_ + 1, tmp.y_};
				}
				//UP
				if(tmp.y_ - 1 >= 0  && this->calc_way_[tmp.x_][tmp.y_ - 1] < min) {
					min = this->calc_way_[tmp.x_][tmp.y_ - 1];
					min_coord = {tmp.x_, tmp.y_ - 1};
				}
				//DOWN
				if(tmp.y_ + 1 < this->calc_way_[0].size() && this->calc_way_[tmp.x_][tmp.y_ + 1] < min) {
					min = this->calc_way_[tmp.x_][tmp.y_ + 1];
					min_coord = {tmp.x_, tmp.y_ + 1};
				}

				this->way_.push_back(min_coord);
				hsh.push(min_coord);
		
				if(min_coord.x_ == this->start_end_.first.x_ && min_coord.y_ == this->start_end_.first.y_) {
					break;
				}
			}
		}
	}
	
	void userMove(void) {
		this->IsFound_ = false;
		Point tmp;
		cin >> tmp.x_ >> tmp.y_;
		while(tmp.x_ >= this->scr_.getScrRes().x_ || tmp.y_ >= this->scr_.getScrRes().y_ || tmp.x_ < 0 || tmp.y_ < 0 || tmp.x_ > 1000 || tmp.y_ > 1000) {
			cerr << "coord error\n";
			tmp.x_ = -1;
			tmp.y_ = -1;
			cin >> tmp.x_ >> tmp.y_;
		}
		unsigned char ch;
		cin >> ch;
		switch(ch) {
		default:
			cerr << "Sym error\n";
			cin >> ch;
			break;
		case FIELDDOT:
			this->putDot(tmp);
			break;
		case WSTART:
			this->putStart(tmp);
			break;
		case WEND:
			this->putEnd(tmp);
			break;
		case WALL:
			this->putWall(tmp);
			break;
		}
	}
private:
	void refreshWay(void) {
		for(size_t x = 0; x < this->scr_.getField().size(); ++x) {
			for(size_t y = 0; y < this->scr_.getField()[x].size(); ++y) {
				switch(this->scr_.getField()[x][y]) {
				default:
				case FIELDDOT:
				case WTRACK:
					this->scr_.putChar({x, y}, FIELDDOT);
					this->calc_way_[x][y] = -1;
					break;
				case WSTART:
					this->calc_way_[x][y] = 0;
					break;
				case WEND:
					this->calc_way_[x][y] = 50000;
					break;
				case WALL:
				    this->calc_way_[x][y] = 60000;
					break;
				}
			}
		}
	}

	void markWayOnScr(void) {
		this->refreshWay();
		for(const auto& point : this->way_) {
			//this->calc_way_[point.x_][point.y_] = 0; DEBUG
			this->scr_.putChar(point, WTRACK);
			this->scr_.putChar(this->start_end_.first, WSTART);
		}
	}
	
	bool IsFound_ = false;
	algotype type_ = algotype::NEUMANN;
	pair<Point, Point> start_end_ {{0, 0}, {0, 0}};
	std::vector<Point> way_;
	std::vector<std::vector<int>> calc_way_;
	Screen& scr_;
};

#endif	//GAME_H
