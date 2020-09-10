/*
 * Very simple pop-up using ncurses form and menu library (not CDK).
 *
 * The buttons are made from items and the fields are made from... well fields.
 *
 * How to run:
 *	g++ TUI.cpp -lmenu -lform -lncurses -o out
 *
 */

// Depending on your OS you might need to remove 'ncurses/' from the include path.
#include <form.h>
#include <menu.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "cirBuff.h"
#include <iostream>


WINDOW *win_body, *win_form, *win_menu;
FORM *form;
FIELD **fields;
MENU *menu;
ITEM **items;
bool is_on_button; 


void new_popup(int rows, int cols, int posy, int posx,const char **buttons,
		int nb_buttons,const char **requests, int nb_fields)
{
	int i, cury = 0, curx = 1, tmp;
	WINDOW *inner;

	win_body = newwin(rows, cols, posy, posx);
	assert(win_body != NULL);
	box(win_body, 0, 0);

	items = (ITEM **)calloc(nb_buttons + 1, sizeof(ITEM *));
	assert(items);

	for (i = 0; i < nb_buttons; i++) {
		items[i] = new_item(buttons[i], "");
		assert(items[i] != NULL);
	}

	items[i] = NULL;
	menu = new_menu(items);
	win_menu = derwin(win_body, 3, cols-2, 1, 1);
	assert(menu != NULL && win_menu != NULL);
	box(win_menu, 0, 0);
	set_menu_win(menu, win_menu);
	set_menu_format(menu, 1, nb_buttons);
	tmp = menu->fcols * (menu->namelen + menu->spc_rows);
	tmp--;
	inner = derwin(win_menu, 1, tmp, 1, (cols-3-tmp)/2);
	set_menu_sub(menu, inner);
	set_menu_mark(menu, "");

	fields = (FIELD **)calloc(nb_fields + 1, sizeof(FIELD *)); 
	assert(fields);

	

	for (i = 0; i < nb_fields; i++) 
	{
		fields[i] = new_field(1, 10, cury, curx, 0, 0);
		assert(fields[i] != NULL);
		set_field_buffer(fields[i], 0, requests[i]);


		if (i % 2 == 1) 
		{
			cury = cury+1;
			curx = 1;
			field_opts_on(fields[i], O_ACTIVE);
			field_opts_on(fields[i], O_EDIT);
			set_field_back(fields[i], A_UNDERLINE); 
		}
	       	else 
		{
			curx = 20;
			field_opts_off(fields[i], O_ACTIVE);
			field_opts_off(fields[i], O_EDIT);
		}
	}
	
	fields[i] = NULL;
	form = new_form(fields);
	assert(form != NULL);
	win_form = derwin(win_body, rows-17, cols-16, 5, 5);
	box(win_form, 0, 0);
	assert(form != NULL && win_form != NULL);
	set_form_win(form, win_form);
	inner = derwin(win_form, form->rows+1, form->cols+1, 1, 1);
	assert(inner != NULL);
	set_form_sub(form, inner);

	assert(post_form(form) == E_OK);
	assert(post_menu(menu) == E_OK);
	is_on_button = true;
	pos_menu_cursor(menu);
}

void delete_popup(void)
{
	int i;

	unpost_form(form);
	unpost_menu(menu);

	for (i = 0; fields[i] != NULL; i++) {
		free_field(fields[i]);
	}

	for (i = 0; items[i] != NULL; i++) {
		free_item(items[i]);
	}

	free_menu(menu);
	free_form(form);
	delwin(win_form);
	delwin(win_menu);
	delwin(win_body);
}

int copy(char * destination, char * source)
{
    int i;
    for(i = 0; source[i] != '\0'; i++)
    {
        destination[i] = source[i];
    }
    return 0;
}


/*
 * Actions for 'return' on a button
 */

cirBuff<uint32_t> * driver_buttons(ITEM *item,cirBuff<uint32_t> * ptrCircle)	
{
	const char *name = item_name(item);
	int i;

		
	
	move(20, 0);
        clrtoeol();
	
	char* what ;
	char* what2 ;

        if (name == "CREATE BUFFER")
        {	
                ptrCircle->~cirBuff(); // explicit destruction
		what = field_buffer(fields[5], 0);
		what2 = field_buffer(fields[3], 0);

                new (ptrCircle) cirBuff<uint32_t>(std::stoi(what));
		mvprintw(LINES -8  ,20 , "BUFFER     SIZE = %zu  ", ptrCircle->size() );
		mvprintw(LINES -10 ,20 , "BUFFER CAPACITY = %zu  ", ptrCircle->capacity() );
        	mvprintw(LINES -5 , 2 ,"                                                                                                                       				");
		mvprintw(LINES -2  , 2 , "Buffer created                                    ");

	}

	else if ( name == "PUSH BACK")
        {
                what = field_buffer(fields[1], 0);
                ptrCircle->push_back(std::stoi(what));
                mvprintw(LINES -2 , 2 ,"Pushed back value = %s                          ", what);
	}
        else if (name == "PUSH FRONT")
        {
                what = field_buffer(fields[1], 0);
                ptrCircle->push_front(std::stoi(what));
                mvprintw(LINES-2 , 2 ,"Pushed front value = %s                          ", what);
        }
        else if (name == "RESET BUFFER")
        {
                if (ptrCircle->size()==0)//sempty())
                {
                        mvprintw(LINES-2, 2, "No need to reset, buffer is already empty");
                }
                else
                {
                        mvprintw(LINES-2, 2, "Buffer reset to empty                    ");
                        ptrCircle->reset_buffer();
			mvprintw(LINES-8 ,20 , "BUFFER     SIZE = %zu  ", ptrCircle->size() );
                }
		mvprintw(LINES -5 , 2 ,"                                                                                                                       				");
        }
        else
        {
                mvprintw(LINES-2, 2, "Need to add this                                 ");
        }

	//results printing
	mvprintw(LINES -8 ,20 , "BUFFER     SIZE = %zu  ", ptrCircle->size() );
        mvprintw(LINES -10 ,20 , "BUFFER CAPACITY = %zu  ", ptrCircle->capacity() );

	for (int m = 0; m < ptrCircle->size(); m++)
	{
		mvprintw(LINES -5 , 2+4*m , "%i",ptrCircle->buffer[m]); 
	}		
	refresh();
	return ptrCircle;
}


/*
 * When you want to change between the form and the buttons
 */

void switch_to_buttons(void)
{
	// Those 2 lines allow the field buffer to be set
	form_driver(form, REQ_PREV_FIELD);
	form_driver(form, REQ_NEXT_FIELD);

	menu_driver(menu, REQ_FIRST_ITEM);
	is_on_button = true;
	set_menu_fore(menu, A_REVERSE); // "show" the button
}

cirBuff<uint32_t> * driver(int ch, cirBuff<uint32_t> * ptrCircle)
{
switch (ch) 
	{
		case KEY_DOWN:
			if (is_on_button)
			{
				is_on_button = false;
                                set_menu_fore(menu, A_NORMAL); // "hide" the button
			}
			else if (form->current == fields[form->maxfield-1])
                        {
                                switch_to_buttons();
                        }
			else
                        {
                                form_driver(form, REQ_NEXT_FIELD);
                        }
			break;

		case KEY_UP:
			if (is_on_button) 
			{	
				switch_to_buttons();
			}
			else if (form->current == fields[1])
                        {
                                switch_to_buttons();
                        }
			else
                        {
                                form_driver(form, REQ_PREV_FIELD);
                        }
                        break;

		case KEY_LEFT:
			if (is_on_button)
			{
				menu_driver(menu, REQ_LEFT_ITEM);
			}
			else
			{
				form_driver(form, REQ_PREV_CHAR);
			}
			break;

		case KEY_RIGHT:
			if (is_on_button)
			{
				menu_driver(menu, REQ_RIGHT_ITEM);
			}
			else
			{
				form_driver(form, REQ_NEXT_CHAR);
			}
			break;

		case 10:
			if (!is_on_button)
			{
				switch_to_buttons();
			}
			else
			{
				driver_buttons(current_item(menu), ptrCircle);
			}
			break;
                // Delete the char before cursor
                case KEY_BACKSPACE:
                case 127:
			if (!is_on_button)
			{
				form_driver(form, REQ_DEL_PREV);
			}
                        break;

                // Delete the char under the cursor
                case KEY_DC:
			if (!is_on_button)
			{
				form_driver(form, REQ_DEL_CHAR);
			}
                        break;


		default:
			if (!is_on_button)
			{
				form_driver(form, ch);
			}
			break;

	}

	if (is_on_button)
	{
		pos_menu_cursor(menu);
	}
	else
	{
		pos_form_cursor(form);
	}
	wrefresh(win_body);
        return ptrCircle;
}


int main()
{
	const char *buttons[] = { "CREATE BUFFER",
		"RESET BUFFER",
		"PUSH BACK",
		"PUSH FRONT",
		"READ FROM FILE",
		"WRITE TO FILE"
       	};
	const char *requests[] = { "Value:", "edit value", "Type:", "edit type","BufferSize:","edit size","Path:","edit path" };
	int ch;

	initscr();
	noecho();
	cbreak();
	keypad(stdscr, TRUE);
	
	// initialize default buffer
	cirBuff<uint32_t> circle(8);
	// Declare pointer to a class.
	cirBuff<uint32_t> *ptrCircle = &circle;


	new_popup(24, COLS-2, 10, 1, buttons, 6, requests,8 );
	refresh();
	wrefresh(win_body);
	wrefresh(win_form);
	wrefresh(win_menu);

	while ((ch = getch()) != KEY_F(1))

		ptrCircle = driver(ch, ptrCircle);
	delete_popup();
	endwin();

	return 0;
}
