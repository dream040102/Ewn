#ifndef GAME_H
#define GAME_H

using namespace std;

struct Player
{
	// position for a chess: (x, y)
	// x and y are between 0 ~ 4, and 5 is for non-existence
	// each x and y are stored by 6's multiples
	// (x6, y6) (x5, y5) ... (x1, y1) use 6 ^ (2*6) = 2,176,782,336
	unsigned int chs_pos;					  // 0 to 4,294,967,295

	void assign(const int& chs_index, const int& x, const int& y);
	void move_x_one_unit(const int& chs_index, const int& direct);
	void move_y_one_unit(const int& chs_index, const int& direct);
};

class Game
{
private:
	Player player_A, player_B;
	Player* player_ptr;

public:
	//Including initializing the positions
	Game();
	//Copy constructor
	Game(const Game& game);
	//Set board
	Game(const bool& is_A_first, const int& chs_pos_A, const int& chs_pos_B);
	//Copy operator
	void operator=(const Game& game);

	//Directly set the board
	void set_board(char board[5][5], int is_switch);
	//Print the board
	void print_board();
	//Roll the dice
	int roll_dice();
	//Input the result of dice and return how many chessmen the player can move
	int count_movable_chs(const int& dice);
	//Check if the chessman is out of the board
	bool check_in_board(const Movement& mvmt);
	//Synchronize the chessman and the board
	void set_chs_on_board(const Chess& chs);
	//Get the symbol of the chess originally on the board
	char get_symbol_on_board(const int& x, const int& y);
	//update_game_status the board(have a movement), chsNum, playerA/playerB. Return 0(game continues), 1(A wins), 2(B wins)
	int update_game_status(const Movement& mvmt);
	//Change is_switch and the pointer of the player
	void switch_player();
	//Print the position and existence of the both players
	void print_status();

	Chess get_movable_chs(const int& k);
	Chess get_cur_chs_list(const int& k);
	Chess get_chs_list(const bool is_switch_, const int& k);
	int get_exist_bitwise();
	bool get_is_switch();
};

#endif
