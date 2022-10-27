#include <cstdlib>
#include <ctime>
#include <iostream>
#include <Windows.h>
#include <iomanip>

using namespace std;

#define MAX_STONE 5
#define MAX_HEAP 12

struct stone_heap {
  int stone;
  stone_heap * next;
};

typedef stone_heap * list;

list root = NULL;
int Sum = 0;
int taken;
int index;
int number_heap;
int stones_in_heap = 0;
int count_valid_heap;
int random_order(); // decides who plays first: 1-computer, 0-user
void input_heapNumber(int & number_heap); // creates number of stone heaps
// these two functions below create randomly number of stones in each heap
void create_input(list & root, int number_heap);
void insert(list & root, int x);
void status(list root); // shows the status after each turn
bool check_heap(list root, int index); // checks if the choosen heap is valid or not
int sum_stone(list root); // calculates total stones
void your_turn(list & root);
// these four functions below are used for computer's turn
int find_sumNim(list root); // caculates the current sum Nim of the game
bool special_case(list root); // check wheather current status is special or not
void findSolution(list & root, int sumNim);
void computer_turn(list root);
void menu(); // show menu

int main() {
  int choice;
  while (true) {
    system("cls");
    cout << "---NIM GAME---" << endl;
    menu();
    do {
      cout << "Your choice: " << endl;
      cin >> choice;
    } while (choice < 1 || choice > 3);
    
    if (choice == 3) break;
    switch (choice) {
    case 1:
      cout << "There are n stone heaps, each heap has some stones. " <<
        endl;
      cout << "You and computer take turn to play the game by choosing a 
      heap, " << endl;
      cout << "and then take at least one stone from that heap, " <<
        endl;
      cout << "you can take all stones in the heap if you want. " <<
        endl;
      cout << "Who takes the last stone is the loser. Good luck!" <<
        endl;
      system("pause");
      break;
    case 2:
      list root = NULL;
      input_heapNumber(number_heap);
      count_valid_heap = number_heap;
      create_input(root, number_heap);
      status(root);
      Sum = sum_stone(root); // Sum is the total stones at the begining
      srand(time(NULL));
      if (random_order()) // computerfirst
      {
        cout << "Computer goes first" << endl;
        while (1) {
          cout << "Computer's turn" << endl;
          computer_turn(root);
          if (Sum == 0) {
            cout << "You win" << endl;
            break;
          }
          status(root);
          cout << "Your turn" << endl;
          your_turn(root);
          if (Sum == 0) {
            cout << "You lose" << endl;
            break;
          }
          status(root);
        }
      } else {
        cout << "You go first" << endl;
        while (1) {
          cout << "Your turn" << endl;
          your_turn(root);
          if (Sum == 0) {
            cout << "You lose" << endl;
            break;
          }
          status(root);
          cout << "Computer's turn" << endl;
          computer_turn(root);
          if (Sum == 0) {
            cout << "You win" << endl;
            break;
          }
          status(root);
        }
      }
      delete[] root;
      system("pause");
      break;
    }
  }
  return 0;
}

int random_order() {
  return rand() % 2;
}

void input_heapNumber(int & number_heap) {
  do {
    cout << "Input number of stone heaps: ";
    cin >> number_heap;
  } while (number_heap < 0 || number_heap > MAX_HEAP);
}

void insert(list & root, int x) {
  list p, before = NULL, after = NULL;
  p = new stone_heap;
  ( * p).stone = x;
  after = root;
  while (after != NULL) {
    before = after;
    after = ( * after).next;
  };
  ( * p).next = after;
  if (root == after) root = p;
  else( * before).next = p;
}

void status(list root) {
  cout << "\nStatus: " << endl;
  cout << "-----------------------------------------" << endl;
  for (int i = 1; i <= number_heap; i++) cout << "HEAP " << i << "\t";
  cout << endl;
  list p = root;
  while (p != NULL) {
    cout << ( * p).stone << "\t";
    p = ( * p).next;
  };
  cout << endl;
  cout << "-----------------------------------------";
  cout << endl;
}

void create_input(list & root, int number_heap) {
  int x;
  srand(time(NULL));
  for (int i = 0; i < number_heap; i++) {
    x = 1 + rand() % (MAX_STONE);
    insert(root, x);
  }
}

bool check_heap(list root, int index) {
  if (index < 1 || index > number_heap) return false;
  list temp = root;
  for (int i = 1; i < index; i++) temp = ( * temp).next;
  stones_in_heap = ( * temp).stone;
  if (stones_in_heap == 0) return false;
  return true;
}

void choosen_heap(list & root, int index, int taken) {
  list p = root;
  for (int i = 1; i < index; i++) p = ( * p).next;
  ( * p).stone -= taken;
  if (( * p).stone == 0) count_valid_heap--;
}

int sum_stone(list root) {
  int Sum = 0;
  list p = root;
  while (p != NULL) {
    Sum += ( * p).stone;
    p = ( * p).next;
  };
  return Sum;
}

void your_turn(list & root) {
  do {
    cout << "Choose the heap: ";
    cin >> index;
  } while (check_heap(root, index) == false);
  do {
    cout << "Number of stones you want to take: ";
    cin >> taken;
  } while (taken > stones_in_heap || taken < 1);
  Sum -= taken;
  choosen_heap(root, index, taken);
}

int find_sumNim(list root) {
  int sumNim = 0;
  list p;
  p = root;
  while (p != NULL) {
    sumNim ^= ( * p).stone;
    p = ( * p).next;
  }
  return sumNim;
}

bool special_case(list root) {
  int count_none1_stone_heap = 0;
  list p;
  p = root;
  while (p != NULL) {
    if (( * p).stone > 1) count_none1_stone_heap++;
    p = ( * p).next;
  }
  if (count_none1_stone_heap != 1) return false;
  else return true;
}

void findSolution(list & root, int sumNim) {
  list p;
  p = root;
  while (((( * p).stone ^ sumNim) >= ( * p).stone)) {
    p = ( * p).next;
  }
  if (special_case(root)) {
    if (count_valid_heap % 2 == 1) taken = ( * p).stone - 1;
    else taken = ( * p).stone;
  } else taken = ( * p).stone - (( * p).stone ^ sumNim);
  ( * p).stone -= taken;
  if (( * p).stone == 0) count_valid_heap--;
  Sum -= taken;
}

void computer_turn(list root) {
  int sumNim = find_sumNim(root);
  if (sumNim == 0) {
    srand(time(NULL));
    do {
      index = rand() % (number_heap + 1);
    } while (check_heap(root, index) == false);
    do {
      taken = rand() % (stones_in_heap + 1);
    } while (taken == 0);
    Sum -= taken;
    choosen_heap(root, index, taken);
  } else findSolution(root, sumNim);
}

void menu() {
  cout << "1. Instruction" << endl;
  cout << "2. Play" << endl;
  cout << "3. Exit" << endl;
}