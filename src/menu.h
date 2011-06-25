#ifndef MENU_HEADER_FILE__
#define MENU_HEADER_FILE__

#define ACTION QUIT 1
#define ACTION_MISSION 2

#define BUTTON_PLAY 1
#define BUTTON_QUIT 2
#define BUTTON_PREV_MISSION 3
#define BUTTON_NEXT_MISSION 4


// Returns one of the above actions
int Main_Menu(int *mission_number);



// This function selects the next/previous mission
// It checks to make sure the mission files exist.
// The new mission number is returned on success and -1 on failure
int New_Mission(int next_prev);


// This function displays the description of a mission on the
// menu screen
int Display_Mission(int mission_number);


// This function draws the important buttons and keyboard
// cursor on the screen
// The "selected" parameter tells us which button is currently
// highlighted
int Draw_Main_Menu(int selected);

#endif

