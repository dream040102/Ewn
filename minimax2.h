#ifndef MINIMAX2_H
#define MINIMAX2_H

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <time.h>
#include <string>
#include "game.h"
#include "minimax.h"

class Minimax2: public Minimax
{
public:
	Minimax2() {
		HEIGHT = 3;
		SIMU_TIMES = 180;
	}
	~Minimax2() {}
	int minimax(Game& cur_game, int height) {
		// check if end;
		if (height == 0) {
			return evaluate_simulation(cur_game);
		}
		int best_val;
		int chs_val_list[6];
		int lookahead_cnt = 0;

		bool is_ai_side;
		int cur_win;
		char cur_symbol;
		if (cur_game.get_is_switch() == this->ai_side) {
			// ai's cur_side		=> the largest, the better
			is_ai_side = true;
			best_val = -1e9;
			cur_win = this->ai_side ? 2 : 1;
			cur_symbol = this->ai_symbol;
			for (int i=0; i<6; i++)
				chs_val_list[i] = -1e9;
		}
		else {
			// opponent's cur_side	=> the smallest, the better
			is_ai_side = false;
			best_val = 1e9;
			cur_win = this->ai_side ? 1 : 2;
			cur_symbol = this->ai_side? '1' : 'A';
			for (int i=0; i<6; i++)
				chs_val_list[i] = 1e9;
		}

		// ai's cur_side, find the max
		// opponent's cur_side, find the min
		for (int dice = 0; dice < 6; dice++) {
			Chess cur_chs = cur_game.get_cur_chs_list(dice);
			if (cur_chs.exist) {
				int chs_index = cur_chs.symbol - cur_symbol;
				for (int direct = 0; direct < 3; direct++) {
					Movement mvmt(chs_index, direct);
					if (cur_game.check_in_board(mvmt)) {
						Game child_game = cur_game;
						int game_status = child_game.update_game_status(mvmt);
						// check if the game ends
						if (game_status == cur_win) return evaluate_feature(child_game);
						else if (game_status != 0) continue;
						child_game.switch_player();

						// return child value.
						int child_val = minimax(child_game, height-1);
						if (is_ai_side) {
							best_val = max(best_val, child_val);
						}
						else {
							best_val = min(best_val, child_val);
						}
					}
					else {
					}
				}
				chs_val_list[dice] = best_val;

				// if there is lookahead_cnt, compare those values of the non-existing with the existing
				// check if the current value is bigger than the previous one.
				if (lookahead_cnt) {
					int offset;
					if ((is_ai_side && best_val>chs_val_list[dice-1]) || (!is_ai_side && best_val<chs_val_list[dice-1])) {
						for (int j = 1; j <= lookahead_cnt; j++) {
							offset = dice - j;
							chs_val_list[offset] = best_val;
						}
					}
					lookahead_cnt = 0;
				}
			}
			else {
				// this chess can't move, use other's value
				lookahead_cnt++;
				if (dice - lookahead_cnt < 0) {
					// there's no chess exists before the selected one
					continue;
				}
				else {
					chs_val_list[dice] = chs_val_list[dice - lookahead_cnt];
				}
			}
		}

		// return the sum of the best values in the chs_val_list (chance node)
		best_val = 0;
		for(int i = 0; i < 6; i++)
			best_val += chs_val_list[i];

		return best_val;
	}
	int simulation(Game simu_game) {
		char cur_symbol;
		int game_status = 0, ai_win = this->ai_side ? 2 : 1;
		if (simu_game.get_is_switch() == this->ai_side)
			cur_symbol = this->ai_symbol;
		else
			cur_symbol = this->ai_side? '1' : 'A';

		while (game_status == 0) {
			Movement available_mvmt_list[6];
			int available_mvmt_cnt = 0;

			int simu_dice = rand() % 6;
			int next_move_cnt = simu_game.count_movable_chs(simu_dice);

			// find all avaible move (1 or 2 chess, 3 directions)
			for (int i = 0; i < next_move_cnt; i++) {
				int chs_index = simu_game.get_movable_chs(i).symbol - (simu_game.get_is_switch() == 0? '1' : 'A');
				for (int direct = 0; direct < 3; direct++) {
					Movement tmp_mvmt(chs_index, direct);
					if (simu_game.check_in_board(tmp_mvmt)) {
						available_mvmt_list[available_mvmt_cnt] = tmp_mvmt;
						available_mvmt_cnt ++;
					}
				}
			}
			// randomly pick a move.
			Movement next_mvmt = available_mvmt_list[rand() % available_mvmt_cnt];
			game_status = simu_game.update_game_status(next_mvmt);
			//simu_game.print_status();
			//cout << "Status:" << game_status << endl;
			
			// check game status => if the game keep going, switch the player.
			if (game_status == 0) simu_game.switch_player();
			else break;
		}
		// return the result of game.
		return (game_status == ai_win? 1 : 0);
	}
};

#endif