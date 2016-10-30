#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct board_info
{
    int session_id;
    int slot_id;
    long reg_time;
    long uptime;
    char hw_version[32];
    char sw_version[32];
};
typedef struct board_info board_info_t;


struct master_info
{
    int max_board_num;
    int reg_board_num;
    board_info_t **boards;
};
typedef struct master_info master_info_t;

master_info_t *
master_info_init(void)
{
	master_info_t *mif = NULL;
	mif = calloc(1, sizeof(master_info_t));
	mif->max_board_num = 128;
	mif->reg_board_num = 0;
	mif->boards = calloc(128, sizeof(board_info_t *));
	return mif;
}


int 
board_info_cmp(const void *a, const void *b)
{
	/* ascending order */

	board_info_t ** aa = (board_info_t **)a;
	board_info_t ** bb = (board_info_t **)b;

	if ((*aa)->slot_id > (*bb)->slot_id) {
		return 1;
	} else if((*aa)->slot_id == (*bb)->slot_id) {
		return 0;
	} else {
		return -1;
	}
}

int 
ret_boards_adjust(master_info_t *mif, int index)
{
	while (index < mif->reg_board_num) {
		mif->boards[index] = mif->boards[index + 1];
		index++;
	}
	return 0;
}

int
reg_boards_sort(master_info_t *mif)
{
	qsort(mif->boards, mif->reg_board_num, sizeof(board_info_t *), board_info_cmp);
	return 0;
}

int 
reg_boards_bsearch(master_info_t *mif, int slot_id) 
{
    int left = 0;
    int right = mif->reg_board_num - 1;
    while (left <= right) {
        int mid = left + ((right - left) >> 1);
        if (mif->boards[mid]->slot_id == slot_id) {
            return mid;
        }
        else if (mif->boards[mid]->slot_id > slot_id) {
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return -1;
}

board_info_t * 
reg_boards_search(master_info_t *mif, int slot_id)
{
	int index = 0;

	index = reg_boards_bsearch(mif, slot_id);
	if (index < 0) {
		return NULL;
	}

	return mif->boards[index];
}

int
reg_boards_add(master_info_t *mif, board_info_t *bif)
{
	mif->boards[mif->reg_board_num] = bif;
	mif->reg_board_num++;
	reg_boards_sort(mif);
	return 0;
}

int 
reg_boards_del(master_info_t *mif, int slot_id)
{
	board_info_t *t = NULL;
	int index = reg_boards_bsearch(mif, slot_id);
	
	if (index < 0) {
		return -1;
	}

	t = mif->boards[index];
	if (t != NULL) {
		free(t);
		mif->boards[index] = NULL;
		ret_boards_adjust(mif, index);
		mif->reg_board_num--;
		return 0;
	}
	return -1;
}

void 
reg_board_print(master_info_t *mif)
{
	int i;
	for (i = 0; i < mif->reg_board_num; i++) {
		printf("%d\t", mif->boards[i]->slot_id);
	}
	printf("end\n");
}

board_info_t *
simple_creat_board(int x)
{
	board_info_t * t = calloc(1, sizeof(board_info_t));
	t->slot_id = x;
	return t;
}

int main(int argc, char const *argv[])
{

	board_info_t * b1 , *b2, *b3, *b4;
	master_info_t * mif = master_info_init();


	b1 = simple_creat_board(11);
	b2 = simple_creat_board(22);
	b3 = simple_creat_board(33);
	b4 = simple_creat_board(44);


	printf("b1:%p\n", b1);
	printf("b2:%p\n", b2);
	printf("b3:%p\n", b3);
	printf("b4:%p\n", b4);


	reg_boards_add(mif, b2);
	reg_board_print(mif);
	reg_boards_add(mif, b3);
	reg_board_print(mif);
	reg_boards_add(mif, b4);
	reg_board_print(mif);
	reg_boards_add(mif, b1);
	reg_board_print(mif);	


	reg_boards_del(mif, 33);
	reg_board_print(mif);

	board_info_t * i = reg_boards_search(mif, 2);

	if (i != NULL)
		printf("%d\n", i->slot_id);
    
	return 0;
}





