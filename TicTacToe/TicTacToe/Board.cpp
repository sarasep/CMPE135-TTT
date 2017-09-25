#include "Board.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

board::board()
{
	for (int i = 0; i < 9; i++)
	{
		boardAr[i] = { ' ' };
	}
	userPosition = 0;
	KB_code = 0; //user key input
	history[11] = {0}; //0-8: input history, 9:first player 0/1, 10: winner 0/1
	historyPtr = history;

	srand(time(NULL)); //randomly selects who goes first
	if (rand() % 2 == 0)
	{
		player = 'O'; //player1=O ,  player2=X
	}
	else
	{
		player = 'X';
		history[9] = 1;
	}

	try
	{
		ofile.open("gameHistory.txt", fstream::app);
	}
	catch (ofstream::failure e)
	{
		cerr << "Fail to write to file" << endl;
	}
}

/*Checks the win condition of that player's turn and returns a boolean*/
bool board::winCondition() const
{
	switch (userPosition)
	{
	case 0:
		if ((boardAr[1]==player && boardAr[2]==player)||(boardAr[3] == player && boardAr[6] == player)||(boardAr[4] == player && boardAr[8] == player))
		{
			return true;
		}
		break;
	case 1:
		if ((boardAr[0] == player && boardAr[2] == player)||(boardAr[4] == player && boardAr[7] == player))
		{
			return true;
		}
		break;
	case 2:
		if ((boardAr[1] == player && boardAr[0] == player) || (boardAr[5] == player && boardAr[8] == player) || (boardAr[4] == player && boardAr[6] == player))
		{
			return true;
		}
		break;
	case 3:
		if ((boardAr[4] == player && boardAr[5] == player) || (boardAr[0] == player && boardAr[6] == player))
		{
			return true;
		}
		break;
	case 4:
		if ((boardAr[3] == player && boardAr[5] == player) || (boardAr[1] == player && boardAr[7] == player))
		{
			return true;
		}
		break;
	case 5:
		if ((boardAr[2] == player && boardAr[8] == player) || (boardAr[3] == player && boardAr[4] == player))
		{
			return true;
		}
		break;
	case 6:
		if ((boardAr[0] == player && boardAr[3] == player) || (boardAr[7] == player && boardAr[8] == player) || (boardAr[4] == player && boardAr[2] == player))
		{
			return true;
		}
		break;
	case 7:
		if ((boardAr[1] == player && boardAr[4] == player) || (boardAr[6] == player && boardAr[8] == player))
		{
			return true;
		}
		break;
	case 8:
		if ((boardAr[6] == player && boardAr[7] == player) || (boardAr[2] == player && boardAr[5] == player) || (boardAr[0] == player && boardAr[4] == player))
		{
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

/*Checks if the board is full*/
bool board::isFull() const
{
	for (int i = 0; i < 9; i++)
	{
		if (boardAr[i] == ' ')
		{
			return false;
		}
	}
	return true;
}

/*Prints the board to the console*/
void board::printBoard() const
{
	system("cls"); //clears the console screen

	if (player == 'O')
	{
		cout << "Player One's Turn" << endl;
	}
	else
	{
		cout << "Player Two's Turn" << endl;
	}

	for (int i = 0; i < 9; i++)
	{
		cout << " ";
		if (i == userPosition) //highlight the cursor
		{
			SetConsoleTextAttribute(hConsole, 128);
			cout << boardAr[i];
			SetConsoleTextAttribute(hConsole, 7);
		}
		else
		{
			cout << boardAr[i];
		}

		if (i != 2 && i != 5 && i != 8)
		{
			cout << " |";
		}
		if (i == 2 || i == 5)
		{
			cout << endl << "-----------" << endl;
		}
	}
	cout << endl;
}

/*Prints the History to the console*/
void board::printHistory() const
{
	for (int i = 0; i < 11; i++)
	{
		cout << history[i];
	}
	cout << endl;
}

/*Keeps track of the user inputs*/
void board::userController()
{
	this->printBoard();
	while (KB_code != KB_ESCAPE)
	{
		if (_kbhit())
		{
			KB_code = _getch();
			switch (KB_code)
			{
			case KB_RIGHT:
				if (userPosition != 2 && userPosition != 5 && userPosition != 8)
				{
					userPosition++;
				}
				printBoard();
				break;
			case KB_LEFT:
				if (userPosition != 0 && userPosition != 3 && userPosition != 6)
				{
					userPosition--;
				}
				printBoard();
				break;
			case KB_UP:
				if (userPosition != 0 && userPosition != 1 && userPosition != 2)
				{
					userPosition -= 3;
				}
				printBoard();
				break;
			case KB_DOWN:
				if (userPosition != 6 && userPosition != 7 && userPosition != 8)
				{
					userPosition += 3;
				}
				printBoard();
				break;
			case '\r':
				if (boardAr[userPosition] == ' ')
				{
					boardAr[userPosition] = player;
					pushHistory();
					if(winCondition()||isFull())
					{
						gameOver();
					}
					else
					{
						if (player == 'O')
						{
							player = 'X';
						}
						else
						{
							player = 'O';
						}
					}
					printBoard();
				}
				break;
			default:
				break;
			}
		}
	}
}

/*Determines who the winner is or if it is a draw*/
void board::gameOver()
{
	KB_code = KB_ESCAPE;
	if (winCondition())
	{
		if (player == 'O')
		{
			cout << "Player One wins!" << endl;
		}
		else
		{
			cout << "Player Two wins!" << endl;
			history[10] = 1;
		}
		this->historyStream();
	}
	else if (isFull() && !winCondition())
	{
		cout << "Draw!" << endl;
		history[10] = 2;
		this->historyStream();
	}
}

/*Keeps track of history
  Position starts at 1-9*/
void board::pushHistory()
{
	*historyPtr = userPosition+1;
	*historyPtr++;
}

void board::historyStream()
{
	for (int i = 0; i < 11; i++)
	{
		ofile << history[i];
	}
	ofile << endl;
}